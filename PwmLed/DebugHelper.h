#pragma once

const char Hexmap[] = { '0', '1', '2', '3', '4', '5', '6', '7',
'8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

class DebugHelper
{
public:
	static char* HexChar(char *data, int len);

	static void CleanEeprom();
};
