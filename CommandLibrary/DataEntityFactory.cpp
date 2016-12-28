#include "DataEntityFactory.h"
#include "DataSerializationHelper.h"
#include "SpeedColorProgramSettings.h"
#include "ErrorHandlingHelper.h"
#include "ColorProgram.h"

DeserializableEntityBase* DataEntityFactory::CreateFromBuffer(char* buffer)
{
	auto tempPtr = buffer;
	auto packetId = DataSerializationHelper<unsigned char>::GetDataFromArray(&tempPtr);
	DeserializableEntityBase* result = nullptr;

	switch (packetId)
	{
		case SpeedColorProgramPacketId:
		{
			result = new SpeedColorProgramSettings();
		}
		break;
		case ColorProgramPacketId:
		{
			result = new ColorProgram();
		}
		break;
		case EmptyDataPacketId:
		{
			result = nullptr;
		}
		break;
		default:
		{
			ErrorHandlingHelper::HandleError("Unrecognized packet id: %i", packetId);
		}
	}

	if (result != nullptr)
	{
		result->FillFromBuffer(buffer);
	}

	return result;
}
