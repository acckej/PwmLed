#pragma once
class Exception
{
public:
	Exception(char *message);
	~Exception();

	char* GetMessage() const;

private: 
	char* _message;
};

class ErrorHandler
{
	static Exception* _currentError;
public:
	static Exception* GetCurrentError();

	static void CleanErrors();

	static void ThrowException(char* message);
};

