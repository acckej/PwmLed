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

SoftwareSerial BTserial(BT_RX_PIN, BT_TX_PIN); // RX | TX

volatile unsigned long _lastUpdate = 0;
volatile unsigned long _debounceDelay = 100;
volatile unsigned long _msBetweenTicks;

unsigned long _timestamp;

double _currentSpeed;

unsigned char _blinkHigh = 0;
boolean _signalHigh = true;

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
CommandDispatcher _dispatcher(GetSysInfo, ApplyColorProgram, ApplySpeedColorProgram, GetCurrentSpeedColorProgram, &_factory);

enum Mode { SpeedColorMode, ColorProgramMode};
Mode _currentMode = SpeedColorMode;

void(*resetFunc) (void) = nullptr;

void setup() 
{	
	//Serial.begin(9600);

	attachInterrupt(0, update, RISING);
	BTserial.begin(9600);
	ErrorHandlingHelper::ErrorHandler = HandleError;
}

void loop()
{	
	float voltage = getVoltage();
	//*
	if(voltage <= THRESHOLD_VOLTAGE)
	{		
		lowVoltageBlink();
	}
	else
	{		
		work();
	}
	//*/

	setDutyBlink();
	////work();	
}

SerializableEntityBase* GetSysInfo()
{
	float voltage = getVoltage();
	auto result = new SystemInformation(voltage, _currentSpeed);
	return result;
}

void ApplyColorProgram(DeserializableEntityBase* entity)
{
	EepromHelper::SaveColorProgramToEeprom(entity);
	_currentMode = ColorProgramMode;	
}

void HandleError(char *message)
{
	BTserial.write(message);
	resetFunc();
}

void ApplySpeedColorProgram(DeserializableEntityBase* entity)
{
	EepromHelper::SaveSpeedColorSettingsToEeprom(entity);
	_currentMode = SpeedColorMode;
}

SerializableEntityBase* GetCurrentSpeedColorProgram()
{
	return nullptr;
}

void ReceiveCommand()
{
	if(BTserial.available())
	{
		char buffer[READ_BUFFER_SIZE];
		auto size = BTserial.readBytes(buffer, READ_BUFFER_SIZE);

		if(size > 0)
		{
			auto result = _dispatcher.ReceivePacket(buffer);

			if(result != nullptr)
			{
				auto sendBuf = new char[result->GetDataSize()];
				result->WriteDataToBuffer(sendBuf);

				delete result;
				BTserial.write(sendBuf, result->GetDataSize());
				delete sendBuf;
			}
		}
	}
}


void setDutyBlink()
{
	if (_signalHigh)
	{
		_signalHigh = false;
		digitalWrite(BLINK_PIN, HIGH);
	}
	else
	{
		_signalHigh = true;
		digitalWrite(BLINK_PIN, LOW);
	}
}

void lowVoltageBlink()
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

float getVoltage()
{
	int reading = analogRead(VOLTAGE_PIN);
	return static_cast<float>(reading) / VOLTAGE_DELIMITER;
}

void work()
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
		setColor(_currentSpeed, 2);		
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

void update()
{
	unsigned long current = millis();
	unsigned long diff = current - _lastUpdate;

	if (diff >= DEBOUNCE_DELAY)
	{
		_lastUpdate = current;
		_msBetweenTicks = diff;
	}
}

void setColor(double currentSpeed, double threshold)
{
	double sigmaGreen = sqrt(_sigmaGreen);
	double sigmaRed = sqrt(_sigmaRed);
	double sigmaBlue = sqrt(_sigmaBlue);

	double range = 255 - threshold;
	
	int red = threshold + range * (1 - gaussian(sigmaRed, _muRed, currentSpeed));
	int green = threshold + range * gaussian(sigmaGreen, _muGreen, currentSpeed);
	int blue = threshold + range * gaussian(sigmaBlue, _muBlue, currentSpeed);

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

	/*
	Serial.print(_currentSpeed, DEC);
	Serial.print(" ");
	Serial.print(red, DEC);
	Serial.print(" ");
	Serial.print(green, DEC);
	Serial.print(" ");
	Serial.print(blue, DEC);
	Serial.println();
	//*/
}


double gaussian(double sigma, double mu, double x)
{
	auto result = 1 / sigma * sqrt(2 * PI) * exp(-1 * pow(x - mu, 2) / 2 * pow(sigma, 2));

	return result;
}


