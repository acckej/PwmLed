#include "Command.h"
#include "ErrorHandlingHelper.h"
#include "DataEntityFactoryBase.h"
#include "DataSerializationHelper.h"


Command::Command(char* buffer, DataEntityFactoryBase* dataEntityFactory)
{
	if (buffer == nullptr)
	{
		ErrorHandlingHelper::HandleError("Command data buffer is empty");
	}

	if (dataEntityFactory == nullptr)
	{
		ErrorHandlingHelper::HandleError("Data entity factory pointer is null");
	}

	auto tempPtr = buffer;
	_commandId = DataSerializationHelper<unsigned char>::GetDataFromArray(&tempPtr);
	_data = dataEntityFactory->CreateFromBuffer(tempPtr);
}

Command::~Command()
{
}

DeserializableEntityBase* Command::GetData() const
{
	return _data;
}

unsigned char Command::GetCommandId() const
{
	return _commandId;
}
