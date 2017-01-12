#pragma once

#include "DeserializableEntityBase.h"
#include "CommandConstants.h"
#include "SerializableEntityBase.h"

class ColorProgramStep : public DeserializableEntityBase, public SerializableEntityBase
{
public:
	ColorProgramStep();
	~ColorProgramStep();

	void FillFromBuffer(char* buffer) override;

	char* ShiftBuffer(char * buffer);

	ArduinoInt GetDelay() const;
	unsigned char Red() const;
	unsigned char Green() const;
	unsigned char Blue() const;

	ArduinoSize GetDataSize() override;
	void WriteDataToBuffer(char* buffer) override;

private:
	ArduinoInt _delay;
	unsigned char _red;
	unsigned char _green;
	unsigned char _blue;
};

