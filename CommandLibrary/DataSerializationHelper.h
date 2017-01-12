#include "CommandConstants.h"

#ifdef Test
#include <cstring>
#endif

#ifdef Arduino
#include <Arduino.h>
#endif

#pragma once
template <class T> class DataSerializationHelper
{
public:
	static char* WriteDataToArray(char* array, T value);	

	static T GetDataFromArray(char** array);

	static char* WriteStringToArray(char* array, int size, const char* string);
};

template <class T>
char* DataSerializationHelper<T>::WriteDataToArray(char* array, T value)
{
	T* tmp = reinterpret_cast<T*>(array);
	*tmp = value;
	return reinterpret_cast<char*>(++tmp);
}

template <class T>
T DataSerializationHelper<T>::GetDataFromArray(char** array)
{	
	T result = **(reinterpret_cast<T**>(array));
	*array+= sizeof(T);
	return result;
}

template <class T>
char* DataSerializationHelper<T>::WriteStringToArray(char* array, int size, const char* string)
{
	memcpy(array, string, size);
	return array + size;
}


