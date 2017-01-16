#include "EepromHelper.h"
#include "Constants.h"
#include <EEPROM.h>

ColorProgram* EepromHelper::SaveColorProgramToEeprom(DeserializableEntityBase* entity)
{
	auto data = reinterpret_cast<ColorProgram*>(entity);
	auto size = data->GetDataSize();

	if (size > 0)
	{
		char buffer[512];
		data->WriteDataToBuffer(buffer);

		for (int i = 0; i < size; i++)
		{
			EEPROM.write(i + COLOR_PROGRAM_EEPROM_SEGMENT, buffer[i]);
		}

		EEPROM.write(COLORPROGRAM_FLAG_ADDRESS, 1);

		return data;
	}

	return nullptr;
}

SpeedColorProgramSettings* EepromHelper::SaveSpeedColorSettingsToEeprom(DeserializableEntityBase* entity)
{
	auto data = reinterpret_cast<SpeedColorProgramSettings*>(entity);
	auto size = data->GetDataSize();

	if (size > 0)
	{
		char buffer[512];
		data->WriteDataToBuffer(buffer);

		for (int i = 0; i < size; i++)
		{
			EEPROM.write(i + SPEED_COLOR_EEPROM_SEGMENT, buffer[i]);
		}

		EEPROM.write(SPEEDCOLOR_FLAG_ADDRESS, 1);

		return data;
	}
	
	return nullptr;
}

SpeedColorProgramSettings* EepromHelper::RestoreSpeedColorFromEeprom()
{
	auto size = EEPROMReadInt(SPEEDCOLOR_FLAG_ADDRESS);

	if(size == 0)
	{
		return nullptr;
	}

	char buffer[512];
	
	for(int i = 0; i < size; i++)
	{
		buffer[i] = EEPROM.read(SPEED_COLOR_EEPROM_SEGMENT + 1);
	}

	auto result = new SpeedColorProgramSettings();
	result->FillFromBuffer(buffer);
	return result;
}

ColorProgram* EepromHelper::RestoreColorProgramFromEeprom()
{
	auto size = EEPROMReadInt(COLORPROGRAM_FLAG_ADDRESS);

	if (size == 0)
	{
		return nullptr;
	}

	char buffer[512];

	for (int i = 0; i < size; i++)
	{
		buffer[i] = EEPROM.read(COLOR_PROGRAM_EEPROM_SEGMENT + 1);
	}

	auto result = new ColorProgram();
	result->FillFromBuffer(buffer);
	return result;
}

bool EepromHelper::IsColorProgramSaved()
{
	auto size = EEPROMReadInt(COLORPROGRAM_FLAG_ADDRESS);
	return size > 0;
}

bool EepromHelper::IsSpeedColorSaved()
{
	return EEPROMReadInt(SPEEDCOLOR_FLAG_ADDRESS) > 0;
}

int EepromHelper::EEPROMReadInt(int address)
{
	long two = EEPROM.read(address);
	long one = EEPROM.read(address + 1);

	//Return the recomposed int by using bitshift.
	return (two << 0 & 0xFF) + (one << 8 & 0xFFFF);
}

void EepromHelper::EEPROMWriteInt(int address, int value)
{
	unsigned char two = (value & 0xFF);
	unsigned char one = ((value >> 8) & 0xFF);

	EEPROM.write(address, two);
	EEPROM.write(address + 1, one);
}



