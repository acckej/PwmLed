#pragma once
#include "../PwmLed.UnitTests/TestHelper.h"

class SerializableEntityBase
{
public:
	SerializableEntityBase();
	virtual ~SerializableEntityBase();

	virtual ArduinoSize GetDataSize();
	virtual void WriteDataToBuffer(char* buffer);
};

