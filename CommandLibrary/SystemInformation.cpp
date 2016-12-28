#include "SystemInformation.h"
#include "DataSerializationHelper.h"

SystemInformation::SystemInformation(ArduinoDouble voltage, ArduinoDouble speed)
{
	_speed = speed;
	_batteryVoltage = voltage;
}

SystemInformation::SystemInformation(): _batteryVoltage(0), _speed(0)
{
}

ArduinoSize SystemInformation::GetDataSize()
{
	const auto result = sizeof(SystemInformationPacketId)
		+ sizeof(ArduinoSize)
		+ sizeof(_speed) 
		+ sizeof(_batteryVoltage);

	return result;
}

void SystemInformation::WriteDataToBuffer(char* buffer)
{
	buffer = DataSerializationHelper<unsigned char>::WriteDataToArray(buffer, SystemInformationPacketId);
	buffer = DataSerializationHelper<ArduinoSize>::WriteDataToArray(buffer, GetDataSize());
	buffer = DataSerializationHelper<ArduinoDouble>::WriteDataToArray(buffer, _speed);
	DataSerializationHelper<ArduinoDouble>::WriteDataToArray(buffer, _batteryVoltage);
}

void SystemInformation::UpdateData(ArduinoDouble voltage, ArduinoDouble speed)
{
	_batteryVoltage = voltage;
	_speed = speed;
}
