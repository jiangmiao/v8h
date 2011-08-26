#ifndef V8H_SYSTEM_H
#define V8H_SYSTEM_H

#include "v8h.h"
namespace v8h
{
	class System
	{
	    public:
		static const char *dirname(const char *filename);
		static V8H_FUNCTION(forceGC);
		static Handle<Value> require(const char *filename);
		static V8H_FUNCTION(_require);
		static V8H_GETTER(get_system_error);
		static V8H_GETTER(get_system_errno);
		static V8H_FUNCTION(puts);
		static V8H_FUNCTION(print);
		static Handle<Object> init(Handle<Object> c);
	};
}
#endif
