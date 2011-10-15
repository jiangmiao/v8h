#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#include <libgen.h>

#include "system.h"
#include "buffer.h"

V8H_NS_START

v8::Persistent<v8::Object> System::sources;

V8H_FUNCTION(System::startup)
{
	return v8::True();
}

V8H_FUNCTION(System::absoluteRequire)
{
	V8H_ENSURE(args.Length() == 1);
	auto global = v8::Context::GetCurrent()->Global();
	v8::String::Utf8Value path(args[0]);
	FILE *file = fopen(*path, "r");
	V8H_DEBUG("require: %s", *path);
	if (file == NULL) {
		return THROW_SYSTEM_EXCEPTION("open file failed");
	}

	Buffer buffer(1024);
	// inject exports roughly
	buffer.write("(function() {var exports = {};");
	while (size_t n = fread(buffer.prepare(4096), 1, buffer.remain(), file)) {
		buffer.commit(n);
	}

	if (ferror(file)) {
		fclose(file);
		return THROW_SYSTEM_EXCEPTION("read file failed");
	}
	buffer.write(";return exports;}).call(this)");
	// fwrite(buffer.data(), 1, buffer.size(), stderr);
	auto source   = v8::String::New(buffer.data(), buffer.size());
	// v8::TryCatch trycatch;
	SET(global, "__SOURCE__", source);
	auto lines = v8::Script::New(v8::String::New("global.__SOURCE__.split('\\n');"))->Run();
	global->Delete(v8::String::New("__SOURCE__"));
	sources->Set(args[0], lines);

	v8::Handle<v8::Script> script;
	//v8::TryCatch trycatch;
	script = v8::Script::New(source, args[0]);
	if (script.IsEmpty()) {
		//puts(*v8::String::Utf8Value(trycatch.Message()->GetSourceLine())); return trycatch.ReThrow();
		return THROW_EXCEPTION("compile failed");
	}

	auto oldDir  = GET(global, "requireDir");
	SET(global, "requireDir", v8::String::New(dirname(*path)));
	auto result = script->Run();
	SET(global, "requireDir", oldDir);
	return result;
}

V8H_FUNCTION(System::getBinDir)
{
	char proc_path[PATH_MAX];
	char bin_path[PATH_MAX];
	snprintf(proc_path, sizeof(proc_path), "/proc/%d/exe", getpid());
	if ( readlink(proc_path, bin_path, sizeof(bin_path)) == -1 ) {
		return THROW_SYSTEM_EXCEPTION("get bin dir failed");
	}
	return v8::String::New(dirname(bin_path));
}

V8H_FUNCTION(System::getWorkingDir)
{
	char workingDir[PATH_MAX];
	if (!getcwd(workingDir, sizeof(workingDir))) {
		return THROW_SYSTEM_EXCEPTION("get working dir");
	}
	return v8::String::New(workingDir);
}

V8H_FUNCTION(System::getEnv)
{
	V8H_ENSURE(args.Length() == 1);
	auto value = getenv(*v8::String::Utf8Value(args[0]));
	if (value) {
		return v8::String::New(value);
	} else {
		return v8::Null();
	}
}

V8H_FUNCTION(System::getLastError)
{
	return v8::String::New(strerror(errno));
}

v8::Handle<v8::Value> System::create()
{
	V8H_CREATE_OBJECT_START();

	sources = v8::Persistent<v8::Object>::New(v8::Object::New());
	V8H_STATIC_IMPLEMENT(absoluteRequire);
	V8H_STATIC_IMPLEMENT(startup);
	V8H_STATIC_IMPLEMENT(getBinDir);
	V8H_STATIC_IMPLEMENT(getWorkingDir);
	V8H_STATIC_IMPLEMENT(getEnv);
	V8H_STATIC_IMPLEMENT(getLastError);
	V8H_CREATE_END();
}

void System::dumpStackTrace()
{
	auto stackTrace = v8::StackTrace::CurrentStackTrace(1);
	if (stackTrace->GetFrameCount()) {
		auto stackFrame = stackTrace->GetFrame(0);
		auto scriptName = stackFrame->GetScriptName();
		auto lineNumber = stackFrame->GetLineNumber();
		puts(*v8::String::Utf8Value(scriptName));
		if (System::sources->Has(scriptName)) {
			auto source = System::sources->Get(scriptName);
			fputs("Assert Error: \n\n", stderr);
			auto lines = v8::Handle<v8::Array>::Cast(source);
			int startLine = lineNumber - 10;
			if (startLine < 0) 
				startLine = 0;
			for (int i=startLine; i<lineNumber; ++i) {
				if (i == lineNumber - 1) 
					fputs("\033[31m", stderr);
				fprintf(stderr, "%s\n", *v8::String::Utf8Value(lines->Get(i)));
				if (i == lineNumber - 1)
					fputs("\033[0m", stderr);
			}
			fputs("\n", stderr);
		}
	}
}


V8H_NS_END

