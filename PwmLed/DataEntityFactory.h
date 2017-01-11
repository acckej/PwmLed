#pragma once

#include "DataEntityFactoryBase.h"

class DataEntityFactory : public DataEntityFactoryBase
{
public:
	virtual ~DataEntityFactory()
	{
	}

	DeserializableEntityBase* CreateFromBuffer(char* buffer) override;
};

