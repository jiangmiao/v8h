#ifndef V8H_ERROR_H
#define V8H_ERROR_H


#include <v8.h>
#include <stdarg.h>

namespace v8h
{
	using namespace v8;
	Handle<Value> raise(const char *format, ...);

}

#endif
