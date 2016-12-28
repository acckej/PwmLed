//#include <LiquidCrystal_I2C.h>
#include "Constants.h"


volatile unsigned long _lastUpdate = 0;
volatile unsigned long _debounceDelay = 100;
volatile unsigned long _msBetweenTicks;

unsigned long _timestamp;

double _currentSpeed;

unsigned char _blinkHigh = 0;
boolean _signalHigh = true;

void setup() 
{	
	//Serial.begin(9600);

	attachInterrupt(0, update, RISING);
}

void loop()
{	
	/*union 
	{
		double d;
		char bytes[sizeof(double)];
	} u;*/		
	
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
	_currentSpeed = _timestamp - _lastUpdate > NOT_MOVING_DELAY ?
		0 :
		DISTANCE / static_cast<double>(static_cast<double>(_msBetweenTicks) / MS_COEF) / static_cast<double>(2000);

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

	delay(COLOR_CHANGE_PERIOD);
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
	double muGreen = TOP_SPEED / 2;
	double muRed = TOP_SPEED / 4;
	double muBlue = 0;

	double sigmaGreen = sqrt(TOP_SPEED / 4);
	double sigmaRed = sqrt(TOP_SPEED / 4);
	double sigmaBlue = sqrt(TOP_SPEED / 2);

	double range = 255 - threshold;
	
	int red = threshold + range * (1 - gaussian(sigmaRed, muRed, currentSpeed));
	int green = threshold + range * gaussian(sigmaGreen, muGreen, currentSpeed);
	int blue = threshold + range * gaussian(sigmaBlue, muBlue, currentSpeed);

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
	double result = 1 / sigma * sqrt(2 * PI) * exp(-1 * pow(x - mu, 2) / 2 * pow(sigma, 2));

	return result;
}


