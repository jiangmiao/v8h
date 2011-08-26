#include "config.h"

namespace v8h
{
	V8H_FUNCTION(Config::constructor)
	{
		return args.This();
	}

	Handle<Function> Config::create()
	{
		V8H_CREATE_START();
		V8H_CREATE_END();
	}

	V8H_IMPORT_NEW_INSTANCE_WITH_CLASS(Config);
	V8H_IMPORT_GLOBAL_WITH_CLASS(Config);
}
