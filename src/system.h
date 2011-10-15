#ifndef V8H_SYSTEM_H
#define V8H_SYSTEM_H

#include "v8h.h"
#include "object.h"

V8H_NS_START

class System : public Object<System>
{
    public:
	static V8H_FUNCTION(absoluteRequire);
	static V8H_FUNCTION(startup);
	static V8H_FUNCTION(getBinDir);
	static V8H_FUNCTION(getWorkingDir);
	static V8H_FUNCTION(getEnv);
	static V8H_FUNCTION(getLastError);

	static void dumpStackTrace();

	static v8::Persistent<v8::Object> sources;
	static v8::Handle<v8::Value> create();

	static void reportException(v8::TryCatch *trycatch);
};

V8H_NS_END

#endif
