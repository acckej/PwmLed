#include "Exception.h"



Exception::Exception(char* message)
{
	_message = message;
}

Exception::~Exception()
{
}

char* Exception::GetMessage() const
{
	return _message;
}
