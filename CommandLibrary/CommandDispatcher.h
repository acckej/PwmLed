#pragma once

#include "SerializableEntityBase.h"
#include "DataEntityFactoryBase.h"
#include "CommandResult.h"

typedef SerializableEntityBase* (*GetSysInfoDelegate)();
typedef void (*ApplyColorProgramDelegate)(DeserializableEntityBase*);
typedef void (*ApplySpeedColorProgramDelegate)(DeserializableEntityBase*);
typedef SerializableEntityBase* (*GetCurrentSpeedColorProgramDelegate)();
typedef SerializableEntityBase* (*GetCurrentColorColorProgramDelegate)();

class CommandDispatcher
{
public:
	CommandDispatcher(GetSysInfoDelegate getSysInfo,
		ApplyColorProgramDelegate applyColorProgram,
		ApplySpeedColorProgramDelegate applySpeedColorProgram,
		GetCurrentSpeedColorProgramDelegate getSpeedColorProgram,
		DataEntityFactoryBase* dataEntityFactory,
		GetCurrentColorColorProgramDelegate getColorProgram);

	~CommandDispatcher();

	CommandResult* ReceivePacket(char* packet) const;
private:
	GetSysInfoDelegate _getSysInfo;
	ApplyColorProgramDelegate _applyColorProgram;
	ApplySpeedColorProgramDelegate _applySpeedColorProgram;
	GetCurrentSpeedColorProgramDelegate _getSpeedColorProgram;
	GetCurrentColorColorProgramDelegate _getColorProgram;

	DataEntityFactoryBase* _dataEntityFactory;

	static CommandResult* GetResponse(SerializableEntityBase* data, unsigned char commandId);
	static CommandResult* GetResponse(unsigned char commandId);
};

