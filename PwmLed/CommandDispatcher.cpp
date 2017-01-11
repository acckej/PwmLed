#include "CommandDispatcher.h"
#include "Command.h"
#include "ErrorHandlingHelper.h"
#include "CommandResult.h"
#include <Arduino.h>

CommandDispatcher::CommandDispatcher(GetSysInfoDelegate getSysInfo,
	ApplyColorProgramDelegate applyColorProgram,
	ApplySpeedColorProgramDelegate applySpeedColorProgram,
	GetCurrentSpeedColorProgramDelegate getSpeedColorProgram,
	DataEntityFactoryBase* dataEntityFactory)
{
	_getSysInfo = getSysInfo;
	_applyColorProgram = applyColorProgram;
	_applySpeedColorProgram = applySpeedColorProgram;
	_getSpeedColorProgram = getSpeedColorProgram;
	_dataEntityFactory = dataEntityFactory;
}

CommandDispatcher::~CommandDispatcher()
{
}

CommandResult* CommandDispatcher::ReceivePacket(char* packet) const
{
	CommandResult* result;
	unsigned char commandId;

	Command cmd(packet, _dataEntityFactory);

	commandId = cmd.GetCommandId();

	switch (commandId)
	{
	case GetSystemInformationCommandId:
	{
		return GetResponse(_getSysInfo(), commandId);
	}
	case GetSpeedColorProgramCommandId:
	{
		return GetResponse(_getSpeedColorProgram(), commandId);
	}
	case UploadSpeedColorProgramCommandId:
	{
		auto data = cmd.GetData();
		_applySpeedColorProgram(data);
	}
	break;
	case UploadColorProgramCommandId:
	{
		auto data = cmd.GetData();
		_applyColorProgram(data);
	}
	break;
	default:
	{
		ErrorHandlingHelper::HandleError("Unrecognized commandId: %i", commandId);
	}
	}

	result = GetResponse(commandId);

	return result;
}

CommandResult* CommandDispatcher::GetResponse(SerializableEntityBase* data, unsigned char commandId)
{
	if (data == nullptr)
	{
		ErrorHandlingHelper::HandleError("Get sys info response cannot be null");
	}

	auto dataLength = data->GetDataSize();
	auto buf = new char[dataLength];

	data->WriteDataToBuffer(buf);

	return new CommandResult(commandId, buf, dataLength, false);
}

CommandResult* CommandDispatcher::GetResponse(unsigned char commandId)
{
	return new CommandResult(commandId);
}


