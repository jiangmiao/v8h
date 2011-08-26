#include <limits.h>
#include <libgen.h>
#include "v8h.h"
#include "error.h"
#include "buffer.h"
#include "system.h"

namespace v8h
{
	const char *System::dirname(const char *filename)
	{
		static char result[PATH_MAX];
		strcpy(result, filename);
		::dirname(result);
		return result;
	}

	V8H_FUNCTION(System::forceGC)
	{
		V8::LowMemoryNotification();
		return Undefined();
	}
	Handle<Value> System::require(const char *filename)
	{
		FILE *file = ::fopen(filename, "r");
		if (file == NULL) {
			return raise("open file %s failed: %s", filename, strerror(errno));
		}
		Buffer buffer;
		buffer.clear();
		// Wrap all a function
		buffer.write("(function(){;exports={};");
		char *dest = buffer.prepare(4096);
		while (size_t n = ::fread(dest, 1, buffer.remain(), file)) {
			buffer.commit(n);
		}
		if (::ferror(file)) {
			::fclose(file);
			return raise("read file %s failed: %s", filename, strerror(errno));
		}
		::fclose(file);
		buffer.write(";return exports;}).call(this);");

		auto global = Context::GetCurrent()->Global();

		auto oldDir = GET(global, "$currentDir");
		// Save js working dir
		SET(global, "$currentDir", String::New(dirname(filename)));
		auto script = Script::New(String::New(buffer.data(), buffer.size()), String::New(filename));
		auto result = script->Run();
		SET(global, "$currentDir", oldDir);
		return result;
	}

	V8H_FUNCTION(System::_require)
	{
		V8H_ENTER();
		String::Utf8Value file_utf8_value(args[0]);
		const char *filename = *file_utf8_value;
		V8H_LEAVE(require(filename));;
	}

	V8H_GETTER(System::get_system_error)
	{
		return String::New(strerror(errno));
	}

	V8H_GETTER(System::get_system_errno)
	{
		return Integer::New(errno);
	}

	V8H_FUNCTION(System::puts)
	{
		for(int i=0; i<args.Length(); ++i) {
			::puts(*String::Utf8Value(args[i]));
		}
		return Undefined();
	}

	V8H_FUNCTION(System::print)
	{
		for(int i=0; i<args.Length(); ++i) {
			String::Utf8Value text(args[i]);
			fwrite(*text, 1, text.length(), stdout);
		}
		fflush(stdout);
		return Undefined();
	}

	Handle<Object> System::init(Handle<Object> c)
	{
		V8H_STATIC_IMPLEMENT(_require);
		V8H_STATIC_IMPLEMENT(forceGC);
		V8H_STATIC_IMPLEMENT(puts);
		V8H_STATIC_IMPLEMENT(print);

		SET(c, "$emptyString", String::New(""));
		SET_ACC(c, "$systemError", get_system_error);
		SET_ACC(c, "$systemErrno", get_system_errno);

		return c;
	}

}

