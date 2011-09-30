#ifndef V8H_FILE_H

#include "v8h.h"

V8H_NS_START

class File
{
    public:
	static V8H_FUNCTION(constructor);
	static V8H_FUNCTION(realpath);
	static v8::Handle<v8::Object> create();
};

V8H_NS_END


#endif
