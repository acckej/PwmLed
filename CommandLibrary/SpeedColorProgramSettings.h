#pragma once

#include "DeserializableEntityBase.h"
#include "SerializableEntityBase.h"
#include "Constants.h"

class SpeedColorProgramSettings : public DeserializableEntityBase, public SerializableEntityBase
{
public:
	SpeedColorProgramSettings();
	~SpeedColorProgramSettings();

	void FillFromBuffer(char* buffer) override;

	virtual ArduinoSize GetDataSize() override;
	virtual void WriteDataToBuffer(char* buffer) override;

	ArduinoDouble GetDistance() const;
	ArduinoDouble GetTopSpeed() const;

	ArduinoInt GetNotMovingDelay() const;
	ArduinoInt GetColorChangePeriod() const;
	ArduinoInt GetBlinkDelay() const;
	ArduinoInt GetIdleDelay() const;

	ArduinoDouble GetMuGreen() const;
	ArduinoDouble GetMuRed() const;
	ArduinoDouble GetMuBlue() const;

	ArduinoDouble GetSigmaGreen() const;
	ArduinoDouble GetSigmaRed() const;
	ArduinoDouble GetSigmaBlue() const;

	void SetData(ArduinoDouble distance,
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
		ArduinoDouble sigmaBlue);

private:
	ArduinoDouble _distance;
	ArduinoDouble _topSpeed;
	
	ArduinoInt _notMovingDelay;
	ArduinoInt _colorChangePeriod;
	ArduinoInt _blinkDelay;
	ArduinoInt _idleDelay;

	ArduinoDouble _muGreen;
	ArduinoDouble _muRed;
	ArduinoDouble _muBlue;

	ArduinoDouble _sigmaGreen;
	ArduinoDouble _sigmaRed;
	ArduinoDouble _sigmaBlue;
};

