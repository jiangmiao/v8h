#ifndef V8H_FILE_H
#define V8H_FILE_H

#include <limits.h>
#include <stdlib.h>

#include "v8h.h"
namespace v8h 
{
	using namespace v8;
	class File
	{
	    public:
		typedef FILE* InternalPointer;
	    public:
		static V8H_DEF_SYM(read);
		static V8H_DEF_SYM(open);
		static V8H_DEF_SYM(close);
		enum {
			FIELD_INTERNAL,
			FIELDS_NUMBER
		};
		// Static Functions
		// Access Test
		static Handle<Boolean> access(const Arguments &args, int how);
		static V8H_FUNCTION(readable);
		static V8H_FUNCTION(exists);
		static V8H_FUNCTION(writable);
		static V8H_FUNCTION(executable);
		static V8H_FUNCTION(realpath);
		static V8H_FUNCTION(dirname);
		static V8H_FUNCTION(read);
		static V8H_FUNCTION(constructor);
		static Handle<Function> create();

		static V8H_DECLARE_GET_INTERNAL();
	};
}
#endif
