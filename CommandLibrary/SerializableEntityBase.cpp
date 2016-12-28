#include "SerializableEntityBase.h"



SerializableEntityBase::SerializableEntityBase()
{
}


SerializableEntityBase::~SerializableEntityBase()
{
}

ArduinoSize SerializableEntityBase::GetDataSize()
{
	return 0;
}

void SerializableEntityBase::WriteDataToBuffer(char* buffer)
{
}
