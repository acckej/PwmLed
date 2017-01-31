#include "stdafx.h"
#include "TestHelper.h"
#include "../CommandLibrary/DataSerializationHelper.h"


char* TestHelper::GetFilledColorProgram()
{
	auto buf = new char[ProgramSize];
	auto tempBuf = buf;

	tempBuf = DataSerializationHelper<unsigned char>::WriteDataToArray(tempBuf, ColorProgramPacketId);
	tempBuf = DataSerializationHelper<ArduinoInt>::WriteDataToArray(tempBuf, StepsCount);

	for(auto i = 0; i < StepsCount; i++)
	{
		tempBuf = DataSerializationHelper<ArduinoInt>::WriteDataToArray(tempBuf, 1000 + 100 * i); // delay
		tempBuf = DataSerializationHelper<unsigned char>::WriteDataToArray(tempBuf, 100 + 10 * i); // red
		tempBuf = DataSerializationHelper<unsigned char>::WriteDataToArray(tempBuf, 120 + 10 * i); // green
		tempBuf = DataSerializationHelper<unsigned char>::WriteDataToArray(tempBuf, 130 + 10 * i); // blue
	}

	return buf;
}

std::string TestHelper::HexStringFromByteArray(char* array, int dataLength)
{
	std::stringstream ss;
	for (int i = 0; i < dataLength; ++i)
	{
		ss << std::hex << static_cast<int>(array[i]);
	}
	std::string mystr = ss.str();

	return mystr;
}

std::string TestHelper::HexStr(unsigned char *data, int len)
{
	std::string s(len * 2, ' ');
	for (int i = 0; i < len; ++i) 
	{
		s[2 * i] = Hexmap[(data[i] & 0xF0) >> 4];
		s[2 * i + 1] = Hexmap[data[i] & 0x0F];
	}
	return s;
}

char* TestHelper::HexChar(unsigned char* data, int len)
{
	char* s = new char[len * 2  + 1];
	s[len * 2] = 0;
	for (int i = 0; i < len; ++i)
	{
		s[2 * i] = Hexmap[(data[i] & 0xF0) >> 4];
		s[2 * i + 1] = Hexmap[data[i] & 0x0F];
	}
	return s;
}

void TestHelper::Hex2bin(const char* src, char* target)
{
	while (*src && src[1])
	{
		*(target++) = Char2Byte(*src) * 16 + Char2Byte(src[1]);
		src += 2;
	}
}

//unsigned char* HexToChar(const char* hexstr)
//{
//	size_t len = strlen(hexstr);
//	if (len % 2 != 0)
//	{
//		return nullptr;
//	}
//
//	size_t final_len = len / 2;
//	unsigned char* chrs = static_cast<unsigned char*>(malloc((final_len + 1) * sizeof(*chrs)));
//	for (size_t i = 0, j = 0; j < final_len; i += 2, j++)
//	{
//		chrs[j] = (hexstr[i] % 32 + 9) % 25 * 16 + (hexstr[i + 1] % 32 + 9) % 25;
//	}
//
//	chrs[final_len] = '\0';
//	return chrs;
//}

unsigned char TestHelper::Char2Byte(char input)
{
	if (input >= '0' && input <= '9')
		return input - '0';
	if (input >= 'A' && input <= 'F')
		return input - 'A' + 10;
	if (input >= 'a' && input <= 'f')
		return input - 'a' + 10;

	throw std::invalid_argument("Invalid input string");
}
