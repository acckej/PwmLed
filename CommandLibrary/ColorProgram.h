#pragma once
#include "ColorProgramStep.h"
#include "DeserializableEntityBase.h"
#include "Constants.h"
#include "SerializableEntityBase.h"

#define NumberOfStepsMax 32

class ColorProgram : public DeserializableEntityBase, public SerializableEntityBase
{
public:
	ColorProgram();
	~ColorProgram();

	ColorProgramStep GetNextStep();

	ArduinoInt GetNumberOfSteps() const;
	bool IsLastStep() const;

	void Reset();

	void FillFromBuffer(char* buffer) override;

	ArduinoSize GetDataSize() override;
	void WriteDataToBuffer(char* buffer) override;

private:
	void Dispose() const;

	ColorProgramStep** _program;
	ArduinoInt _numberOfSteps;
	ArduinoInt _stepsCounter;
};

