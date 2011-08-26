#ifndef V8H_CONSOLE_H
#define V8H_CONSOLE_H

#include "v8h.h"

namespace v8h
{
	using namespace v8;

	class Console
	{
	    public:
		static V8H_FUNCTION(constructor);
		static Handle<Function> create();
		static V8H_DECLARE_NEW_INSTANCE();
		static V8H_DECLARE_GLOBAL();

	};
}

#endif
