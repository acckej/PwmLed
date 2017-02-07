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
	Serial.begin(9600);

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
	//BTserial.begin(9600);
	BTserial.begin(38400);
	ErrorHandlingHelper::ErrorHandler = HandleError;

	_currentSystemInformation = new SystemInformation(0, 0);
}

void loop()
{	
	ReceiveCommand();

	digitalWrite(BLINK_PIN, HIGH);
	delay(500);
	digitalWrite(BLINK_PIN, LOW);
	delay(500);
	/*ReceiveCommand();

	float voltage = GetVoltage();
	
	if(voltage <= THRESHOLD_VOLTAGE)
	{		
		LowVoltageBlink();
	}
	else
	{		
		Work();
	}

	*/
	//work();	
}


void Test2()
{
	delay(1000);
	//digitalWrite(BLINK_PIN, HIGH);

	ErrorHandlingHelper::HandleError("test message");	

	delay(1000);
	//digitalWrite(BLINK_PIN, LOW);
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
	/*CommandResult result(ErrorCommandId, message, strlen(message), true);
	auto buffer = new char[result.GetDataSize()];
	result.WriteDataToBuffer(buffer);
	BTserial.write(buffer, result.GetDataSize());
		
	Serial.write(buffer, result.GetDataSize());

	delete buffer;
	resetFunc();*/
}

void ApplySpeedColorProgram(DeserializableEntityBase* entity)
{
	auto updated = EepromHelper::SaveSpeedColorSettingsToEeprom(entity);

	Serial.println("asc");
	UpdateSpeedColorSettings(updated);

	if (_currentSpeedColorProgram != nullptr)
	{
		delete _currentSpeedColorProgram;
	}

	Serial.println("upd2");
	_currentSpeedColorProgram = updated;
}

void UpdateSpeedColorSettings(SpeedColorProgramSettings* updated)
{
	if (updated != nullptr)
	{
		Serial.println("upd");
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

		auto packet = DebugHelper::HexChar(static_cast<char*>(buffer), size);
		Serial.println(packet);
		delete packet;

		if(size > 0)
		{				
			auto result = _dispatcher.ReceivePacket(buffer);

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

void LowVoltageBlink()
{
	if(_blinkHigh == 255)
	{
		_blinkHigh = 0;
	}
	else
	{
		_blinkHigh = 255;
	}

	analogWrite(RED_PIN, _blinkHigh);
	analogWrite(GREEN_PIN, 0);
	analogWrite(BLUE_PIN, 0);

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

		analogWrite(RED_PIN, step.Red());
		analogWrite(GREEN_PIN, step.Green());
		analogWrite(BLUE_PIN, step.Blue());

		delay(step.GetDelay());

	} while (!_currentColorProgram->IsLastStep());
}

void GetSpeedAndColor()
{
	unsigned long currentTime = millis();
	//unsigned long period = currentTime - _timestamp;
	_timestamp = currentTime;

	//_currentSpeed = period / 1000 * _value * DISTANCE;	
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

		delay(IDLE_DELAY);

		analogWrite(RED_PIN, 0);
		analogWrite(GREEN_PIN, 0);
		analogWrite(BLUE_PIN, 0);
	}

	delay(_colorChangePeriod);
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
	double sigmaGreen = sqrt(_sigmaGreen);
	double sigmaRed = sqrt(_sigmaRed);
	double sigmaBlue = sqrt(_sigmaBlue);

	double range = 255 - threshold;
	
	int red = threshold + range * (1 - Gaussian(sigmaRed, _muRed, currentSpeed));
	int green = threshold + range * Gaussian(sigmaGreen, _muGreen, currentSpeed);
	int blue = threshold + range * Gaussian(sigmaBlue, _muBlue, currentSpeed);

	if (blue > 255)
	{
		blue = 255;
	}
	else if (blue < 0)
	{
		blue = 0;
	}
	if (red > 255)
	{
		red = 255;
	}
	else if (red < 0)
	{
		red = 0;
	}
	if (green > 255)
	{
		green = 255;
	}
	else if (green < 0)
	{
		green = 0;
	}

	analogWrite(RED_PIN, red);
	analogWrite(GREEN_PIN, green);
	analogWrite(BLUE_PIN, blue);
}


double Gaussian(double sigma, double mu, double x)
{
	auto result = 1 / sigma * sqrt(2 * PI) * exp(-1 * pow(x - mu, 2) / 2 * pow(sigma, 2));

	return result;
}


