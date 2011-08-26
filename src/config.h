#ifndef V8H_CONFIG_H
#define V8H_CONFIG_H

#include "v8h.h"

namespace v8h
{
	class Config
	{
	    public:
		static V8H_FUNCTION(constructor);
		static Handle<Function> create();
		static V8H_DECLARE_NEW_INSTANCE();
		static V8H_DECLARE_GLOBAL();
	};
}

#endif
