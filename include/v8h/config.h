#ifndef V8H_CONFIG_H
#define V8H_CONFIG_H

namespace v8h
{
	class Config
	{
	    public:
		static V8H_FUNCTION(constructor)
		{
			return args.This();
		}

		static Handle<Function> create()
		{
			V8H_CREATE_START();
			V8H_CREATE_END();
		}

		static V8H_IMPORT_NEW_INSTANCE();
		static V8H_IMPORT_GLOBAL();
	};
}

#endif
