#pragma once
#include "SerializableEntityBase.h"


class CommandResult : public SerializableEntityBase
{
public:
	CommandResult(unsigned char commandId, const char* message, ArduinoSize messageLength, bool hasError);
	CommandResult(unsigned char commandId);
	~CommandResult();

	ArduinoSize GetDataSize() override;
	void WriteDataToBuffer(char* buffer) override;

	char GetCommandId() const;

	const char* GetMessage() const;
	ArduinoSize GetMessageLength() const;

	bool HasError() const;

private:
	unsigned char _commandId;
	bool _hasError;
	const char* _message;
	ArduinoSize _messageLength;
};

