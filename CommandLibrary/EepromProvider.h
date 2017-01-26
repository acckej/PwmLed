#pragma once

#include "CommandConstants.h"

class EepromProvider
{
#ifdef Test
	static unsigned char _buffer[1024];
#endif

public:
#ifdef Test
	static unsigned char* GetBuffer();
#endif

	static unsigned char Read(ArduinoInt index);

	static void Write(ArduinoInt index, unsigned char value);
};

