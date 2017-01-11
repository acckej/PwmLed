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

