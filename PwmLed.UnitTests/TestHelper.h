#pragma once
#include "../CommandLibrary/Constants.h"
#include <string>

const char Hexmap[] = { '0', '1', '2', '3', '4', '5', '6', '7',
'8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

class TestHelper
{
public:
	static const ArduinoInt StepsCount = 2;
	
	static const int StepSize = sizeof(ArduinoInt)
		+ sizeof(unsigned char) * 3;

	static const int ProgramSize = StepSize * StepsCount
		+ sizeof(unsigned char)
		+ sizeof(ArduinoInt);

	static char* GetFilledColorProgram();

	static std::string HexStringFromByteArray(char* array, int dataLength);

	static std::string HexStr(unsigned char *data, int len);	

	static void Hex2bin(const char* src, char* target);
private:
	static unsigned char Char2Byte(char input);
};

