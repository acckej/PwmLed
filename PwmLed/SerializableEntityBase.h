#pragma once
#include "CommandConstants.h"

class SerializableEntityBase
{
public:
	SerializableEntityBase();
	virtual ~SerializableEntityBase();

	virtual ArduinoSize GetDataSize();
	virtual void WriteDataToBuffer(char* buffer);
};

