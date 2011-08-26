#include "console.h"

namespace v8h
{
	V8H_FUNCTION(Console::constructor)
	{
		return args.This();
	}

	Handle<Function> Console::create()
	{
		V8H_CREATE_START();
		V8H_CREATE_END();
	}

	V8H_IMPORT_NEW_INSTANCE_WITH_CLASS(Console);
	V8H_IMPORT_GLOBAL_WITH_CLASS(Console);
}
