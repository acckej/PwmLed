#pragma once

#include "DeserializableEntityBase.h"

class DataEntityFactoryBase;

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

