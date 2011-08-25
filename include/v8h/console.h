#ifndef V8H_CONSOLE_H
#define V8H_CONSOLE_H

namespace v8h
{
	class Console
	{
	    public:
		static V8H_FUNCTION(constructor)
		{
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
