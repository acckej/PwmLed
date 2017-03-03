#pragma once
#include "DataSizeProvider.h"

class DeserializableEntityBase : public DataSizeProvider
{
public:
	DeserializableEntityBase();
	virtual ~DeserializableEntityBase();

	virtual void FillFromBuffer(char* buffer);
};

