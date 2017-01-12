#pragma once

#include "DeserializableEntityBase.h"
#include "DataEntityFactoryBase.h"


class Command 
{
public:
	Command(char* buffer, DataEntityFactoryBase* dataEntityFactory);
	~Command();

	DeserializableEntityBase* GetData() const;

	unsigned char GetCommandId() const;

private:
	DeserializableEntityBase* _data;
	unsigned char _commandId;
};

