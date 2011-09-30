#include "assert.h"
#include "system.h"

V8H_NS_START

void Assert::dumpFunction(v8::Handle<v8::Function> function)
{
	v8::String::Utf8Value source(function->ToString());
	fwrite(*source, 1, source.length(), stderr);
	fwrite("\n", 1, 1, stderr);
	fflush(stderr);
}

V8H_FUNCTION(Assert::ok)
{
	V8H_ENSURE(args.Length() == 1 || args.Length() == 2);
	auto guard   = args[0]->BooleanValue();
	if (guard) {
		return v8::True();
	} else{
		auto message = args.Length() == 2 ? args[1]->ToString() : v8::String::NewSymbol("");
		System::dumpStackTrace();
		return ThrowException(v8::Exception::Error(message));
	}

}

V8H_FUNCTION(Assert::eq)
{
	V8H_ENSURE(args.Length() == 2 || args.Length() == 3);
	auto actual   = args[0];
	auto expected = args[1];
	if (actual->StrictEquals(expected)) {
		return v8::True();
	} else{
		auto message = args.Length() == 3 ? args[2]->ToString() : v8::String::NewSymbol("");
		auto stackTrace = v8::StackTrace::CurrentStackTrace(2);
		auto stackFrame = stackTrace->GetFrame(1);
		auto scriptName = stackFrame->GetScriptName();
		puts(*v8::String::Utf8Value(scriptName));
		return ThrowException(v8::Exception::Error(message));
	}
}

V8H_FUNCTION(Assert::constructor)
{
	return args.This();
}

v8::Handle<v8::Object> Assert::create()
{
	V8H_CREATE_OBJECT_START();
	V8H_STATIC_IMPLEMENT(eq);
	V8H_STATIC_IMPLEMENT(ok);
	V8H_CREATE_END();
}

V8H_NS_END
