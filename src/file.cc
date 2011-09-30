#include <limits.h>
#include "file.h"

V8H_NS_START

V8H_FUNCTION(File::realpath)
{
	V8H_ENSURE(args.Length() == 1);

	static char resolvedPath[PATH_MAX];
	char *result = ::realpath(*v8::String::Utf8Value(args[0]), resolvedPath);
	if (result == NULL) {
		return v8::False();
	} else {
		return v8::String::New(result);
	}
}

V8H_FUNCTION(File::constructor)
{
	return args.This();
}

v8::Handle<v8::Object> File::create()
{
	V8H_CREATE_OBJECT_START();
	V8H_STATIC_IMPLEMENT(realpath);
	V8H_CREATE_END();
}

V8H_NS_END
