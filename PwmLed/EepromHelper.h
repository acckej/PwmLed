#pragma once

#include "DeserializableEntityBase.h"
#include "SpeedColorProgramSettings.h"
#include "ColorProgram.h"

class EepromHelper
{
public:
	static void SaveColorProgramToEeprom(DeserializableEntityBase* entity);
	static void SaveSpeedColorSettingsToEeprom(DeserializableEntityBase* entity);

	static SpeedColorProgramSettings* RestoreSpeedColorFromEeprom();
	static ColorProgram* RestoreColorProgramFromEeprom();

	static bool IsColorProgramSaved();
	static bool IsSpeedColorSaved();
private:
	static int EEPROMReadInt(int address);
	static void EEPROMWriteInt(int address, int value);
};

