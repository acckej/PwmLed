#pragma once
class DeserializableEntityBase
{
public:
	DeserializableEntityBase();
	virtual ~DeserializableEntityBase();

	virtual void FillFromBuffer(char* buffer);
};

