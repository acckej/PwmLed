#pragma once

#include "Constants.h"
#include "SerializableEntityBase.h"

class SystemInformation : public SerializableEntityBase
{
public:
	SystemInformation(ArduinoDouble voltage, ArduinoDouble speed);
	SystemInformation();
	
	ArduinoSize GetDataSize() override;
	void WriteDataToBuffer(char* buffer) override;

	void UpdateData(ArduinoDouble voltage, ArduinoDouble speed);
private:
	ArduinoDouble _batteryVoltage;
	ArduinoDouble _speed;
};

