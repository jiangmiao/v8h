#include "buffer.h"
#include "assert.h"
#include "file.h"
#include "system.h"

using namespace v8h;
using namespace v8;
V8H_FUNCTION(puts)
{
	for (int i=0; i<args.Length(); ++i) {
		v8::String::Utf8Value data(args[0]);
		fwrite(*data, 1, data.length(), stdout);
	}
	fwrite("\n", 1, 1, stdout);
	fflush(stdout);
	return v8::Undefined();
}



void dumpError(TryCatch &trycatch)
{
	auto message = trycatch.Message();
	String::Utf8Value what(message->Get());
	String::Utf8Value sourceLine(message->GetSourceLine());
	String::Utf8Value scriptResourceName(message->GetScriptResourceName());
	auto lineNumber = message->GetLineNumber();
	auto columnNumber = message->GetStartColumn();
	printf("Error: %s\n    at %s:%d:%d\n", *what, *scriptResourceName, lineNumber, columnNumber);

}

int main(int argc, char **argv)
{
	V8H_DEBUG("V8 version: %s", V8::GetVersion());
	v8::V8::SetFlagsFromCommandLine(&argc, argv, true);
	HandleScope scope;
	auto context = Context::New();
	Context::Scope contextScope(context);
	auto global = context->Global();

	SET(global, "Buffer", Buffer::create());
	SET(global, "Assert", Assert::create());
	SET(global, "File"  , File::create());
	SET(global, "System", System::create());
	SET(global, "puts"  , puts);
	SET(global, "global", global);
	SET(global, "absoluteRequire", System::absoluteRequire);

	auto ARGV = v8::Array::New();
	for (int i=0; i<argc; ++i) {
		ARGV->Set(i, String::New(argv[i]));
	}
	SET(global, "ARGV", ARGV);

	TryCatch trycatch;
	 auto script = Script::New(String::New("(function(){var file = System.getBinDir()+'/../modules/core/startup.js'; var path = File.realpath(file); if (!path) throw new Error('cannot find startup file '+ file);System.absoluteRequire(path)}).call(this);"), String::New("core"));
	if (script.IsEmpty()) {
		dumpError(trycatch);
	} else {
		script->Run();
		if (trycatch.HasCaught()) {
			auto stackTrace = trycatch.StackTrace();
			if (!stackTrace.IsEmpty()) {
				String::Utf8Value info(stackTrace);
				fputs(*info, stderr);
				fputs("\n", stderr);
			} else {
				dumpError(trycatch);
			}

		}
	}
	return 0;
}
