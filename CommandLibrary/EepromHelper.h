#pragma once

#include "DeserializableEntityBase.h"
#include "SpeedColorProgramSettings.h"
#include "ColorProgram.h"

class EepromHelper
{
public:
	static ColorProgram* SaveColorProgramToEeprom(DeserializableEntityBase* entity);
	static SpeedColorProgramSettings* SaveSpeedColorSettingsToEeprom(DeserializableEntityBase* entity);

	static SpeedColorProgramSettings* RestoreSpeedColorFromEeprom();
	static ColorProgram* RestoreColorProgramFromEeprom();

	static bool IsColorProgramSaved();
	static bool IsSpeedColorSaved();
private:
	static int EEPROMReadInt(ArduinoInt address);
	static void EEPROMWriteInt(ArduinoInt address, ArduinoInt value);
};

