#include "ColorProgram.h"
#include "DataSerializationHelper.h"
#include "ErrorHandlingHelper.h"


ColorProgram::ColorProgram(): _program(nullptr), _numberOfSteps(0), _stepsCounter(0)
{
}


ColorProgram::~ColorProgram()
{
	Dispose();
}

ColorProgramStep ColorProgram::GetNextStep()
{
	if(_program == nullptr)
	{
		ErrorHandlingHelper::HandleError("No color program defined");
	}

	if(_stepsCounter + 1 > _numberOfSteps)
	{
		_stepsCounter = 0;
	}	

	return *_program[_stepsCounter++];
}

ArduinoInt ColorProgram::GetNumberOfSteps() const
{
	return _numberOfSteps;
}

bool ColorProgram::IsLastStep() const
{
	return _stepsCounter == _numberOfSteps - 1;
}

void ColorProgram::Reset()
{
	_stepsCounter = 0;
}

void ColorProgram::FillFromBuffer(char* buffer)
{	
	auto tempBuffer = buffer;
	auto packetId = DataSerializationHelper<unsigned char>::GetDataFromArray(&tempBuffer);

	if(packetId != ColorProgramPacketId)
	{
		ErrorHandlingHelper::HandleError("Invalid packet id %i, expected ColorProgramPacketId", packetId);
	}

	_numberOfSteps = DataSerializationHelper<ArduinoInt>::GetDataFromArray(&tempBuffer);

	if(_numberOfSteps < 0 || _numberOfSteps > NumberOfStepsMax)
	{
		ErrorHandlingHelper::HandleError("Invalid number of steps id %i", _numberOfSteps);
	}

	Dispose();

	_program = new ColorProgramStep*[_numberOfSteps];

	for(auto i = 0; i < _numberOfSteps; i++)
	{
		auto step = new ColorProgramStep();
		_program[i] = step;
		step->FillFromBuffer(tempBuffer);
		tempBuffer = step->ShiftBuffer(tempBuffer);
	}

	_stepsCounter = 0;
}

ArduinoSize ColorProgram::GetDataSize()
{
	return _numberOfSteps == 0 ?
		sizeof(_numberOfSteps) + sizeof(ColorProgramPacketId) :
		sizeof(_numberOfSteps) + sizeof(ColorProgramPacketId) + _numberOfSteps * _program[0]->GetDataSize();
}

void ColorProgram::WriteDataToBuffer(char* buffer)
{
	auto tempBuf = buffer;

	tempBuf = DataSerializationHelper<unsigned char>::WriteDataToArray(tempBuf, ColorProgramPacketId);
	tempBuf = DataSerializationHelper<ArduinoInt>::WriteDataToArray(tempBuf, _numberOfSteps);

	for (auto i = 0; i < _numberOfSteps; i++)
	{
		auto step = _program[i];
		step->WriteDataToBuffer(tempBuf);
		tempBuf = step->ShiftBuffer(tempBuf);
	}
}

void ColorProgram::Dispose() const
{
	if (_program != nullptr)
	{
		for (auto i = 0; i < _numberOfSteps; i++)
		{
			delete _program[i];
		}

		delete[] _program;
	}
}
