#ifndef V8H_FILE_H
#define V8H_FILE_H

#include <limits.h>
#include <stdlib.h>

namespace v8h 
{
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

		static V8H_IMPORT_GET_INTERNAL();

		// Static Functions
		// Access Test
		static Handle<Boolean> access(const Arguments &args, int how)
		{
			String::Utf8Value filename(args[0]);
			return V8_BOOLEAN(::access(*filename, how) == 0);
		}

		static V8H_FUNCTION(readable)
		{
			return access(args, R_OK);
		}

		static V8H_FUNCTION(exists)
		{
			return access(args, F_OK);
		}

		static V8H_FUNCTION(writable)
		{
			return access(args, W_OK);
		}

		static V8H_FUNCTION(executable)
		{
			return access(args, X_OK);
		}

		static V8H_FUNCTION(realpath)
		{
			char resolved_path[PATH_MAX];
			String::Utf8Value path(args[0]);
			auto result = ::realpath(*path, resolved_path);
			if (result)
				return String::New(resolved_path);
			else
				return Null();
		}

		static V8H_FUNCTION(dirname)
		{
			String::Utf8Value path_object(args[0]);
			auto path = *path_object;
			return String::New(::dirname(path));
		}

		static V8H_FUNCTION(read)
		{
			String::Utf8Value path_object(args[0]);
			auto path = *path_object;
			FILE *file = fopen(path, "r");
			if (!file)
				return Null();
			
			static Buffer buffer(4096);
			buffer.clear();
			char *dest = buffer.prepare(4096);
			while (size_t n = fread(dest, 1, buffer.remain(), file))
				buffer.commit(n);

			if(ferror(file)) {
				fclose(file);
				return Null();
			}
			fclose(file);
			return String::New(buffer.data(), buffer.size());
		}

		static V8H_FUNCTION(constructor)
		{
			return args.This();
		}

		static Handle<Function> create()
		{
			V8H_CREATE_START();
			V8H_STATIC_IMPLEMENT(dirname);
			V8H_STATIC_IMPLEMENT(read);
			V8H_STATIC_IMPLEMENT(realpath);
			V8H_STATIC_IMPLEMENT(readable);
			V8H_STATIC_IMPLEMENT(writable);
			V8H_STATIC_IMPLEMENT(executable);
			V8H_STATIC_IMPLEMENT(exists);
			V8H_CREATE_END();
		}

	};
}

#endif
