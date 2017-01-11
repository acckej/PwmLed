#include "ErrorHandlingHelper.h"
#include <cstdarg>
#include <cstdio>

void ErrorHandlingHelper::HandleError(char* message, ...)
{
	char buffer[256];
	va_list args;
	va_start(args, message);
	vsnprintf(buffer, 256, message, args);
	va_end(args);

	//throw Exception(buffer);
	ErrorHandler(buffer);
}
