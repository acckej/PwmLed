#pragma once
#include "CommandConstants.h"

class DataSizeProvider
{
public:
	DataSizeProvider();
	virtual ~DataSizeProvider();

	virtual ArduinoSize GetDataSize();
};

