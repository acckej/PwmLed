#include "CommandResult.h"
#include "Constants.h"
#include "DataSerializationHelper.h"


CommandResult::CommandResult(unsigned char commandId, const char* message, ArduinoSize messageLength, bool hasError)
{
	_commandId = commandId;
	_hasError = hasError;
	_message = message;
	_messageLength = messageLength;
}

CommandResult::CommandResult(unsigned char commandId)
{
	_commandId = commandId;
	_hasError = false;
	_message = nullptr;
	_messageLength = 0;
}

CommandResult::~CommandResult()
{
	if (_message != nullptr && !_hasError)
	{
		delete _message;
	}
}

ArduinoSize CommandResult::GetDataSize()
{
	const auto result = sizeof(CommandResultPacketId)
		+ sizeof(ArduinoSize)
		+ sizeof(_commandId)
		+ sizeof(_hasError)
		+ sizeof(_messageLength);

	return result +_messageLength;
}

void CommandResult::WriteDataToBuffer(char* buffer)
{
	auto tempBuffer = buffer;
	tempBuffer = DataSerializationHelper<unsigned char>::WriteDataToArray(tempBuffer, CommandResultPacketId);
	tempBuffer = DataSerializationHelper<ArduinoSize>::WriteDataToArray(tempBuffer, GetDataSize());
	tempBuffer = DataSerializationHelper<unsigned char>::WriteDataToArray(tempBuffer, _commandId);
	tempBuffer = DataSerializationHelper<bool>::WriteDataToArray(tempBuffer, _hasError);
	tempBuffer = DataSerializationHelper<ArduinoSize>::WriteDataToArray(tempBuffer, _messageLength);

	if (_message != nullptr && _messageLength > 0)
	{
		DataSerializationHelper<const char*>::WriteStringToArray(tempBuffer, _messageLength, _message);
	}
}

char CommandResult::GetCommandId() const
{
	return _commandId;
}

const char* CommandResult::GetMessage() const
{
	return _message;
}

ArduinoSize CommandResult::GetMessageLength() const
{
	return _messageLength;
}

bool CommandResult::HasError() const
{
	return _hasError;
}
