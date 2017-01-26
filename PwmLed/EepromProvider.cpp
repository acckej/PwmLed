#include "EepromProvider.h"

#ifdef Arduino

#include "EEPROM.h"

#endif


#ifdef Test

unsigned char EepromProvider::_buffer[1024];

unsigned char* EepromProvider::GetBuffer()
{
	return _buffer;
}

unsigned char EepromProvider::Read(ArduinoInt index)
{
	return _buffer[index];
}

void EepromProvider::Write(ArduinoInt index, unsigned char value)
{
	_buffer[index] = value;
}

#endif


#ifdef Arduino

unsigned char EepromProvider::Read(ArduinoInt index)
{
	return EEPROM.read(index);
}

void EepromProvider::Write(ArduinoInt index, unsigned char value)
{
	EEPROM.write(index, value);
}

#endif
