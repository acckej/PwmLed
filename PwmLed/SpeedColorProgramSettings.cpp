#include "SpeedColorProgramSettings.h"
#include "DataSerializationHelper.h"
#include "ErrorHandlingHelper.h"

SpeedColorProgramSettings::SpeedColorProgramSettings()
{
	_distance = Distance;
	_muBlue = MuBlue;
	_sigmaBlue = SigmaBlue;
	_sigmaRed = SigmaRed;
	_sigmaGreen = SigmaGreen;
	_muGreen = MuGreen;
	_muRed = MuRed;
	_notMovingDelay = NotMovingDelay;
	_colorChangePeriod = ColorChangePeriod;
	_blinkDelay = BlinkDelay;
	_topSpeed = TopSpeed;
	_idleDelay = IdleDelay;
}


SpeedColorProgramSettings::~SpeedColorProgramSettings()
{
}

void SpeedColorProgramSettings::FillFromBuffer(char* buffer)
{
	unsigned char packetId = *buffer;

	if(packetId != SpeedColorProgramPacketId)
	{
		ErrorHandlingHelper::HandleError("Invalid packet id: %i", packetId);
	}

	buffer += sizeof(SpeedColorProgramPacketId);
	ArduinoSize dataSize = *buffer;
	auto expected = GetDataSize();

	if(dataSize != expected)
	{
		ErrorHandlingHelper::HandleError("Invalid data size: %i, expected %i", packetId, expected);
	}

	buffer += sizeof(expected);
	_distance = DataSerializationHelper<ArduinoDouble>::GetDataFromArray(&buffer);
	_blinkDelay = DataSerializationHelper<ArduinoInt>::GetDataFromArray(&buffer);
	_colorChangePeriod = DataSerializationHelper<ArduinoInt>::GetDataFromArray(&buffer);
	_idleDelay = DataSerializationHelper<ArduinoInt>::GetDataFromArray(&buffer);
	_muBlue = DataSerializationHelper<ArduinoDouble>::GetDataFromArray(&buffer);
	_muGreen = DataSerializationHelper<ArduinoDouble>::GetDataFromArray(&buffer);
	_muRed = DataSerializationHelper<ArduinoDouble>::GetDataFromArray(&buffer);
	_notMovingDelay = DataSerializationHelper<ArduinoInt>::GetDataFromArray(&buffer);
	_sigmaBlue = DataSerializationHelper<ArduinoDouble>::GetDataFromArray(&buffer);
	_sigmaGreen = DataSerializationHelper<ArduinoDouble>::GetDataFromArray(&buffer);
	_sigmaRed = DataSerializationHelper<ArduinoDouble>::GetDataFromArray(&buffer);
	_topSpeed = DataSerializationHelper<ArduinoDouble>::GetDataFromArray(&buffer);
}

ArduinoSize SpeedColorProgramSettings::GetDataSize()
{
	const auto result = sizeof(SpeedColorProgramPacketId)
		+ sizeof(ArduinoSize)
		+ sizeof(_distance)
		+ sizeof(_blinkDelay)
		+ sizeof(_colorChangePeriod)
		+ sizeof(_idleDelay)
		+ sizeof(_muBlue)
		+ sizeof(_muGreen)
		+ sizeof(_muRed)
		+ sizeof(_notMovingDelay)
		+ sizeof(_sigmaBlue)
		+ sizeof(_sigmaGreen)
		+ sizeof(_sigmaRed)
		+ sizeof(_topSpeed);

	return result;
}

void SpeedColorProgramSettings::WriteDataToBuffer(char* buffer)
{
	buffer = DataSerializationHelper<unsigned char>::WriteDataToArray(buffer, SpeedColorProgramPacketId);
	buffer = DataSerializationHelper<ArduinoSize>::WriteDataToArray(buffer, GetDataSize());
	buffer = DataSerializationHelper<ArduinoDouble>::WriteDataToArray(buffer, _distance);
	buffer = DataSerializationHelper<ArduinoInt>::WriteDataToArray(buffer, _blinkDelay);
	buffer = DataSerializationHelper<ArduinoInt>::WriteDataToArray(buffer, _colorChangePeriod);
	buffer = DataSerializationHelper<ArduinoInt>::WriteDataToArray(buffer, _idleDelay);
	buffer = DataSerializationHelper<ArduinoDouble>::WriteDataToArray(buffer, _muBlue);
	buffer = DataSerializationHelper<ArduinoDouble>::WriteDataToArray(buffer, _muGreen);
	buffer = DataSerializationHelper<ArduinoDouble>::WriteDataToArray(buffer, _muRed);
	buffer = DataSerializationHelper<ArduinoInt>::WriteDataToArray(buffer, _notMovingDelay);
	buffer = DataSerializationHelper<ArduinoDouble>::WriteDataToArray(buffer, _sigmaBlue);
	buffer = DataSerializationHelper<ArduinoDouble>::WriteDataToArray(buffer, _sigmaGreen);
	buffer = DataSerializationHelper<ArduinoDouble>::WriteDataToArray(buffer, _sigmaRed);
	DataSerializationHelper<ArduinoDouble>::WriteDataToArray(buffer, _topSpeed);
}

ArduinoDouble SpeedColorProgramSettings::GetDistance() const
{
	return _distance;
}

ArduinoDouble SpeedColorProgramSettings::GetTopSpeed() const
{
	return _topSpeed;
}

ArduinoInt SpeedColorProgramSettings::GetNotMovingDelay() const
{
	return _notMovingDelay;
}

ArduinoInt SpeedColorProgramSettings::GetColorChangePeriod() const
{
	return _colorChangePeriod;
}

ArduinoInt SpeedColorProgramSettings::GetBlinkDelay() const
{
	return _blinkDelay;
}

ArduinoInt SpeedColorProgramSettings::GetIdleDelay() const
{
	return _idleDelay;
}

ArduinoDouble SpeedColorProgramSettings::GetMuGreen() const
{
	return _muGreen;
}

ArduinoDouble SpeedColorProgramSettings::GetMuRed() const
{
	return _muRed;
}

ArduinoDouble SpeedColorProgramSettings::GetMuBlue() const
{
	return _muBlue;
}

ArduinoDouble SpeedColorProgramSettings::GetSigmaGreen() const
{
	return _sigmaGreen;
}

ArduinoDouble SpeedColorProgramSettings::GetSigmaRed() const
{
	return _sigmaRed;
}

ArduinoDouble SpeedColorProgramSettings::GetSigmaBlue() const
{
	return _sigmaBlue;
}

void SpeedColorProgramSettings::SetData(ArduinoDouble distance,
	ArduinoDouble topSpeed,
	ArduinoInt notMovingDelay,
	ArduinoInt colorChangePeriod,
	ArduinoInt blinkDelay, 
	ArduinoInt idleDelay,
	ArduinoDouble muRed,
	ArduinoDouble muGreen,
	ArduinoDouble muBlue,
	ArduinoDouble sigmaRed,
	ArduinoDouble sigmaGreen,
	ArduinoDouble sigmaBlue)
{
	_distance = distance;
	_topSpeed = topSpeed;
	_notMovingDelay = notMovingDelay;
	_colorChangePeriod = colorChangePeriod;
	_blinkDelay = blinkDelay;
	_idleDelay = idleDelay;
	_muRed = muRed;
	_muGreen = muGreen;
	_muBlue = muBlue;
	_sigmaRed = sigmaRed;
	_sigmaGreen = sigmaGreen;
	_sigmaBlue = sigmaBlue;
}
