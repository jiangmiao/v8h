#include <unistd.h>
#include <libgen.h>
#include <limits.h>
#include <stdlib.h>
#include "file.h"
#include "buffer.h"

namespace v8h 
{
	using namespace v8;

	// Static Functions
	// Access Test
	Handle<Boolean> File::access(const Arguments &args, int how)
	{
		String::Utf8Value filename(args[0]);
		return V8_BOOLEAN(::access(*filename, how) == 0);
	}

	V8H_FUNCTION(File::readable)
	{
		return access(args, R_OK);
	}

	V8H_FUNCTION(File::exists)
	{
		return access(args, F_OK);
	}

	V8H_FUNCTION(File::writable)
	{
		return access(args, W_OK);
	}

	V8H_FUNCTION(File::executable)
	{
		return access(args, X_OK);
	}

	V8H_FUNCTION(File::realpath)
	{
		char resolved_path[PATH_MAX];
		String::Utf8Value path(args[0]);
		auto result = ::realpath(*path, resolved_path);
		if (result)
			return String::New(resolved_path);
		else
			return Null();
	}

	V8H_FUNCTION(File::dirname)
	{
		String::Utf8Value path_object(args[0]);
		auto path = *path_object;
		return String::New(::dirname(path));
	}

	V8H_FUNCTION(File::read)
	{
		String::Utf8Value path_object(args[0]);
		auto path = *path_object;
		FILE *file = fopen(path, "r");
		if (!file)
			return Null();

		Buffer buffer(4096);
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

	V8H_FUNCTION(File::constructor)
	{
		return args.This();
	}

	Handle<Function> File::create()
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

	V8H_IMPORT_GET_INTERNAL_WITH_CLASS(File);
	V8H_DEF_SYM(File::read);
	V8H_DEF_SYM(File::open);
	V8H_DEF_SYM(File::close);
}
