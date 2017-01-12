#include "ErrorHandlingHelper.h"
#include "CommandConstants.h"

#ifdef Test
#include <cstdarg>
#include <cstdio>
#endif

#ifdef Arduino
#include <Arduino.h>
#endif

ErrorHandlingDelegate ErrorHandlingHelper::ErrorHandler;

void ErrorHandlingHelper::HandleError(char* message, ...)
{
	char buffer[256];
	va_list args;
	va_start(args, message);
	vsnprintf(buffer, 256, message, args);
	va_end(args);

	ErrorHandler(buffer);
}
