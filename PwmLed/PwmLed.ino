//#include <LiquidCrystal_I2C.h>
#include "Constants.h"
#include "CommandConstants.h"
#include <SoftwareSerial.h>
#include "CommandDispatcher.h"
#include "DataEntityFactory.h"
#include "ErrorHandlingHelper.h"
#include "SystemInformation.h"
#include "EepromHelper.h"
#include "EEPROM.h"
#include "DebugHelper.h"

#include "DataSerializationHelper.h"

SoftwareSerial BTserial(BT_RX_PIN, BT_TX_PIN); // RX | TX

volatile unsigned long _lastUpdate = 0;
volatile unsigned long _debounceDelay = 100;
volatile unsigned long _msBetweenTicks;

unsigned long _timestamp;

double _currentSpeed;

unsigned char _blinkHigh = 0;
int _notMovingDelay = NotMovingDelay;
int _colorChangePeriod = ColorChangePeriod;
int _blinkDelay = BlinkDelay;
int _idleDelay = IdleDelay;

double _distance = Distance;
double _topSpeed = TopSpeed;

double _muRed = MuRed;
double _muGreen = MuGreen;
double _muBlue = MuBlue;

double _sigmaRed = SigmaRed;
double _sigmaGreen = SigmaGreen;
double _sigmaBlue = SigmaBlue;

DataEntityFactory _factory;
CommandDispatcher _dispatcher(GetSysInfo, ApplyColorProgram, ApplySpeedColorProgram, GetCurrentSpeedColorProgram, &_factory, GetColorProgram);

enum Mode { SpeedColorMode, ColorProgramMode};
Mode _currentMode = SpeedColorMode;

ColorProgram* _currentColorProgram;
SystemInformation* _currentSystemInformation;
SpeedColorProgramSettings* _currentSpeedColorProgram;

void(*resetFunc) (void) = nullptr;

void setup() 
{	
	//Serial.begin(9600);

	auto savedSpeedColor = EepromHelper::RestoreSpeedColorFromEeprom();
	UpdateSpeedColorSettings(savedSpeedColor);
	if(savedSpeedColor != nullptr)
	{
		_currentSpeedColorProgram = savedSpeedColor;
	}
	else
	{
		_currentSpeedColorProgram = new SpeedColorProgramSettings();
		_currentSpeedColorProgram->SetData(_distance,
			_topSpeed,
			_notMovingDelay,
			_colorChangePeriod,
			_blinkDelay,
			_idleDelay,
			_muRed,
			_muGreen,
			_muBlue,
			_sigmaRed,
			_sigmaGreen,
			_sigmaBlue);
	}

	_currentColorProgram = EepromHelper::RestoreColorProgramFromEeprom();

	attachInterrupt(0, Update, RISING);
	BTserial.begin(38400);
	ErrorHandlingHelper::ErrorHandler = HandleError;

	_currentSystemInformation = new SystemInformation(0, 0);
}

void loop()
{	
	ReceiveCommand();

	float voltage = GetVoltage();
	
	if(voltage <= THRESHOLD_VOLTAGE)
	{		
		LowVoltageBlink();
	}
	else
	{		
		Work();
	}	

	/*digitalWrite(13, HIGH);
	delay(1000);
	digitalWrite(13, LOW);
	delay(1000);*/
}

SerializableEntityBase* GetSysInfo()
{	
	float voltage = GetVoltage();
	_currentSystemInformation->UpdateData(voltage, _currentSpeed);
	return _currentSystemInformation;
}

SerializableEntityBase* GetColorProgram()
{
	return _currentColorProgram;
}

void ApplyColorProgram(DeserializableEntityBase* entity)
{
	auto program = EepromHelper::SaveColorProgramToEeprom(entity);
	_currentColorProgram = program;
	_currentMode = ColorProgramMode;
}

void HandleError(char *message)
{		
	CommandResult result(ErrorCommandId, message, strlen(message), true);
	auto buffer = new char[result.GetDataSize()];
	result.WriteDataToBuffer(buffer);
	BTserial.write(buffer, result.GetDataSize());	

	delete buffer;
	resetFunc();
}

void ApplySpeedColorProgram(DeserializableEntityBase* entity)
{
	auto updated = EepromHelper::SaveSpeedColorSettingsToEeprom(entity);
	UpdateSpeedColorSettings(updated);

	if (_currentSpeedColorProgram != nullptr)
	{
		delete _currentSpeedColorProgram;
	}
	
	_currentSpeedColorProgram = updated;
}

void UpdateSpeedColorSettings(SpeedColorProgramSettings* updated)
{
	if (updated != nullptr)
	{		
		_distance = updated->GetDistance();
		_sigmaBlue = updated->GetSigmaBlue();
		_sigmaGreen = updated->GetSigmaGreen();
		_sigmaRed = updated->GetSigmaRed();
		_blinkDelay = updated->GetBlinkDelay();
		_colorChangePeriod = updated->GetColorChangePeriod();
		_muBlue = updated->GetMuBlue();
		_muRed = updated->GetMuRed();
		_muGreen = updated->GetMuGreen();
		_notMovingDelay = updated->GetNotMovingDelay();
		_topSpeed = updated->GetTopSpeed();

		_currentMode = SpeedColorMode;
	}
}

SerializableEntityBase* GetCurrentSpeedColorProgram()
{
	return _currentSpeedColorProgram;
}

void ReceiveCommand()
{
	if(BTserial.available())
	{
		char buffer[READ_BUFFER_SIZE];
		auto size = BTserial.readBytes(buffer, READ_BUFFER_SIZE);

		/*auto packet = DebugHelper::HexChar(static_cast<char*>(buffer), size);
		Serial.println(packet);
		delete packet;*/

		if(size > 0)
		{				
			auto result = _dispatcher.ReceivePacket(buffer, size);

			if(result != nullptr)
			{
				auto sendBuf = new char[result->GetDataSize()];
				result->WriteDataToBuffer(sendBuf);
				auto sz = result->GetDataSize();
				BTserial.write(sendBuf, sz);
				
				delete result;
				delete sendBuf;
			}
		}
	}
}

bool CommandAvailable()
{
	return BTserial.available();
}

void LowVoltageBlink()
{
	if(_blinkHigh == 255)
	{
		_blinkHigh = 0;
		digitalWrite(13, HIGH);
	}
	else
	{
		_blinkHigh = 255;
		digitalWrite(13, LOW);
	}	

	analogWrite(RED_PIN, _blinkHigh);
	analogWrite(GREEN_PIN, 0);
	analogWrite(BLUE_PIN, 0);

	if(CommandAvailable())
	{
		TurnLightOff();
		return;
	}

	delay(BLINK_DELAY);
}

float GetVoltage()
{
	auto reading = analogRead(VOLTAGE_PIN);
	return static_cast<float>(reading) / VOLTAGE_DELIMITER;
}

void Work()
{
	if (_currentMode == SpeedColorMode)
	{		
		GetSpeedAndColor();
	}
	else
	{		
		PlayColorProgram();
	}
}

void PlayColorProgram()
{
	if(_currentColorProgram == nullptr || _currentColorProgram->GetNumberOfSteps() == 0)
	{
		return;
	}

	do
	{
		auto step = _currentColorProgram->GetNextStep();

		if(CommandAvailable())
		{
			TurnLightOff();
			return;
		}		

		SetCorrectedColor(step.Red(), step.Green(), step.Blue());

		delay(step.GetDelay());

	} while (!_currentColorProgram->IsLastStep());
}

void SetCorrectedColor(double redSrc, double greenSrc, double blueSrc)
{
	auto red = redSrc * COLOR_CORRECTION_RED;
	auto green = greenSrc * COLOR_CORRECTION_GREEN;
	auto blue = blueSrc * COLOR_CORRECTION_BLUE;

	auto max = red;
	if (green > max)
	{
		max = green;
	}
	if (blue > max)
	{
		max = blue;
	}

	auto coef = max == 0 ? 0 : 255 / max;

	red = red * coef;
	green = green * coef;
	blue = blue * coef;

	/*Serial.println(static_cast<int>(red));
	Serial.println(static_cast<int>(green));
	Serial.println(static_cast<int>(blue));*/

	analogWrite(RED_PIN, static_cast<int>(red));
	analogWrite(GREEN_PIN, static_cast<int>(green));
	analogWrite(BLUE_PIN, static_cast<int>(blue));
}

void GetSpeedAndColor()
{
	auto currentTime = millis();
	_timestamp = currentTime;
	
	_currentSpeed = _timestamp - _lastUpdate > _notMovingDelay ?
		0 :
		_distance / static_cast<double>(static_cast<double>(_msBetweenTicks) / MS_COEF) / static_cast<double>(2000);	

	if (_currentSpeed > 0)
	{		
		SetColor(_currentSpeed, COLOR_THRESHOLD);
	}
	else
	{
		analogWrite(RED_PIN, 255);
		analogWrite(GREEN_PIN, 255);
		analogWrite(BLUE_PIN, 0);

		if(CommandAvailable())
		{
			TurnLightOff();
			return;
		}

		delay(IDLE_DELAY);

		TurnLightOff();
	}

	delay(_colorChangePeriod);
}

void TurnLightOff()
{
	analogWrite(RED_PIN, 0);
	analogWrite(GREEN_PIN, 0);
	analogWrite(BLUE_PIN, 0);
}

void Update()
{
	unsigned long current = millis();
	unsigned long diff = current - _lastUpdate;

	if (diff >= DEBOUNCE_DELAY)
	{
		_lastUpdate = current;
		_msBetweenTicks = diff;
	}
}

void SetColor(double currentSpeed, double threshold)
{
	auto sigmaGreen = sqrt(_sigmaGreen);
	auto sigmaRed = sqrt(_sigmaRed);
	auto sigmaBlue = sqrt(_sigmaBlue);

	auto range = 255 - threshold;
	
	auto red = threshold + range * (1 - Gaussian(sigmaRed, _muRed, currentSpeed));
	auto green = threshold + range * Gaussian(sigmaGreen, _muGreen, currentSpeed);
	auto blue = threshold + range * Gaussian(sigmaBlue, _muBlue, currentSpeed);

	
	if (blue < 0)
	{
		blue = 0;
	}
	if (red < 0)
	{
		red = 0;
	}
	if (green < 0)
	{
		green = 0;
	}

	SetCorrectedColor(red, green, blue);
}


double Gaussian(double sigma, double mu, double x)
{
	auto result = 1 / sigma * sqrt(2 * PI) * exp(-1 * pow(x - mu, 2) / 2 * pow(sigma, 2));

	return result;
}


