#include "DebugHelper.h"
#include "EEPROM.h"

bool CleanedEeprom = false;

char* DebugHelper::HexChar(char* data, int len)
{
	char* s = new char[len * 2 + 1];
	s[len * 2] = 0;
	for (int i = 0; i < len; ++i)
	{
		s[2 * i] = Hexmap[(data[i] & 0xF0) >> 4];
		s[2 * i + 1] = Hexmap[data[i] & 0x0F];
	}
	return s;
}

void DebugHelper::CleanEeprom()
{
	if(CleanedEeprom)
	{
		return;
	}

	for(auto i = 0; i < 1024; i++)
	{
		EEPROM.write(i, 0);
	}

	CleanedEeprom = true;
}
