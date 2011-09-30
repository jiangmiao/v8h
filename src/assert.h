#ifndef V8H_ASSERT_H
#define V8H_ASSERT_H

#include "v8h.h"
#include "object.h"

V8H_NS_START

class Assert : public Object<Assert>
{
    public:
	static V8H_FUNCTION(eq);
	static V8H_FUNCTION(ok);
	static V8H_FUNCTION(constructor);
	static void dumpFunction(v8::Handle<v8::Function> function);
	static v8::Handle<v8::Object> create();
};

V8H_NS_END
#endif
