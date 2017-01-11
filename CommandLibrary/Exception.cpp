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

Exception* ErrorHandler::GetCurrentError()
{
	return _currentError;
}

void ErrorHandler::CleanErrors()
{
	if(_currentError != nullptr)
	{
		delete _currentError;
	}
}

void ErrorHandler::ThrowException(char* message)
{
	CleanErrors();

	_currentError = new Exception(message);
}
