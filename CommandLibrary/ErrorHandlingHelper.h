#pragma once

typedef void(*ErrorHandlingDelegate)(char*);

class ErrorHandlingHelper
{
public:
	static void HandleError(char* message, ...);

	static ErrorHandlingDelegate ErrorHandler;
};




