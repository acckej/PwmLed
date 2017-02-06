#include "CommandDispatcher.h"
#include "Command.h"
#include "ErrorHandlingHelper.h"
#include "CommandResult.h"

#ifdef Test
#include <cstring>
#endif

#ifdef Arduino
#include <Arduino.h>
#endif

CommandDispatcher::CommandDispatcher(GetSysInfoDelegate getSysInfo, 
	ApplyColorProgramDelegate applyColorProgram, 
	ApplySpeedColorProgramDelegate applySpeedColorProgram,
	GetCurrentSpeedColorProgramDelegate getSpeedColorProgram,
	DataEntityFactoryBase* dataEntityFactory,
	GetCurrentColorColorProgramDelegate getColorProgram)
{
	_getSysInfo = getSysInfo;
	_applyColorProgram = applyColorProgram;
	_applySpeedColorProgram = applySpeedColorProgram;
	_getSpeedColorProgram = getSpeedColorProgram;
	_dataEntityFactory = dataEntityFactory;
	_getColorProgram = getColorProgram;
}

CommandDispatcher::~CommandDispatcher()
{
}

CommandResult* CommandDispatcher::ReceivePacket(char* packet) const
{
	CommandResult* result;
	unsigned char commandId ;

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
	case GetColorProgramCommandId:
	{
		auto program = _getColorProgram();
		if (program == nullptr)
		{
			auto msg = "No color program defined";
			return new CommandResult(GetColorProgramCommandId, msg, strlen(msg), true);
		}
		return GetResponse(program, commandId);
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


