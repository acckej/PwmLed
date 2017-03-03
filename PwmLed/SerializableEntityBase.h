#pragma once
#include "DataSizeProvider.h"

class SerializableEntityBase : public DataSizeProvider
{
public:
	SerializableEntityBase();
	virtual ~SerializableEntityBase();

	virtual void WriteDataToBuffer(char* buffer);
};

