#include <stdarg.h>
#include "error.h"

namespace v8h
{
	using namespace v8;
	Handle<Value> raise(const char *format, ...)
	{
		va_list args;
		va_start(args, format);
		static char buffer[512];
		vsprintf(buffer, format, args);
		va_end(args);
		return ThrowException(Exception::Error(String::New(buffer)));
	}

}
