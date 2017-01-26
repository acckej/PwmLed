#include "EepromHelper.h"
#include "CommandConstants.h"
#include "EepromProvider.h"

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
			EepromProvider::Write(i + COLOR_PROGRAM_EEPROM_SEGMENT, buffer[i]);
		}

		EEPROMWriteInt(COLORPROGRAM_FLAG_ADDRESS, size);

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
			EepromProvider::Write(i + SPEED_COLOR_EEPROM_SEGMENT, buffer[i]);
		}

		EEPROMWriteInt(SPEEDCOLOR_FLAG_ADDRESS, size);

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
		buffer[i] = EepromProvider::Read(SPEED_COLOR_EEPROM_SEGMENT + i);
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
		buffer[i] = EepromProvider::Read(COLOR_PROGRAM_EEPROM_SEGMENT + i);
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

int EepromHelper::EEPROMReadInt(ArduinoInt address)
{
	long two = EepromProvider::Read(address);
	long one = EepromProvider::Read(address + 1);

	//Return the recomposed int by using bitshift.
	return (two << 0 & 0xFF) + (one << 8 & 0xFFFF);
}

void EepromHelper::EEPROMWriteInt(ArduinoInt address, ArduinoInt value)
{
	unsigned char two = (value & 0xFF);
	unsigned char one = ((value >> 8) & 0xFF);

	EepromProvider::Write(address, two);
	EepromProvider::Write(address + 1, one);
}



