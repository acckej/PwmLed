#include "ErrorHandlingHelper.h"
#include <cstdarg>
#include <cstdio>
#include <exception>


void ErrorHandlingHelper::HandleError(char* message, ...)
{
	char buffer[256];
	va_list args;
	va_start(args, message);
	vsprintf_s(buffer, message, args);
	va_end(args);

	throw std::exception(buffer);
}
