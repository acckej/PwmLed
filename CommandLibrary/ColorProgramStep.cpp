#include "ColorProgramStep.h"
#include "DataSerializationHelper.h"


ColorProgramStep::ColorProgramStep(): _delay(0), _red(0), _green(0), _blue(0)
{
}


ColorProgramStep::~ColorProgramStep()
{
}

void ColorProgramStep::FillFromBuffer(char* buffer)
{	
	auto tempBuffer = buffer;
	_delay = DataSerializationHelper<ArduinoInt>::GetDataFromArray(&tempBuffer);
	_red = DataSerializationHelper<unsigned char>::GetDataFromArray(&tempBuffer);
	_green = DataSerializationHelper<unsigned char>::GetDataFromArray(&tempBuffer);
	_blue = DataSerializationHelper<unsigned char>::GetDataFromArray(&tempBuffer);
}

char* ColorProgramStep::ShiftBuffer(char* buffer)
{
	return buffer + GetDataSize();
}

ArduinoInt ColorProgramStep::GetDelay() const
{
	return _delay;
}

unsigned char ColorProgramStep::Red() const
{
	return _red;
}

unsigned char ColorProgramStep::Green() const
{
	return _green;
}

unsigned char ColorProgramStep::Blue() const
{
	return _blue;
}

ArduinoSize ColorProgramStep::GetDataSize()
{
	const auto dataSize = sizeof(_delay)
		+ sizeof(_blue)
		+ sizeof(_green)
		+ sizeof(_red);

	return dataSize;
}

void ColorProgramStep::WriteDataToBuffer(char* buffer)
{
	auto tempBuf = buffer;
	tempBuf = DataSerializationHelper<ArduinoInt>::WriteDataToArray(tempBuf, _delay);
	tempBuf = DataSerializationHelper<unsigned char>::WriteDataToArray(tempBuf, _red);
	tempBuf = DataSerializationHelper<unsigned char>::WriteDataToArray(tempBuf, _green);
	DataSerializationHelper<unsigned char>::WriteDataToArray(tempBuf, _blue);
}
