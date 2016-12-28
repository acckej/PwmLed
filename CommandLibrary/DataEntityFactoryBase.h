#pragma once

#include "DeserializableEntityBase.h"

class DataEntityFactoryBase
{
protected:
	~DataEntityFactoryBase()
	{
	}

public:
	virtual DeserializableEntityBase* CreateFromBuffer(char* buffer);
};

