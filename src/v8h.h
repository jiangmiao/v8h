#ifndef V8H_V8H_H
#define V8H_V8H_H

#include <v8.h>

#define V8H_NS_START namespace v8h {
#define V8H_NS_END }

#define V8H_ENTER() HandleScope scope; \
	auto self = args.This();


#define V8H_FUNCTION(name) v8::Handle<v8::Value> name(const v8::Arguments &args)
#define V8H_SETTER(name) Handle<Value> name(Local<String> property, Local<Value> value, const AccessorInfo &info)
#define V8H_GETTER(name) Handle<Value> name(Local<String> property, const AccessorInfo &info)
#define V8H_LAMBDA [](const Arguments &args) -> Handle<Value>


#define V8H_DEF_SYM(name) v8::Persistent<v8::String> name##_sym
#define V8H_SET_SYM(name) name##_sym = v8::Persistent<v8::String>::New(v8::String::NewSymbol(#name))
#define V8H_GET_SYM(name) name##_sym
#define V8H_SYM(name) name##_sym

// Auto Delete object
#define V8H_DESTRUCTOR() \
	void destructor(Persistent<Value> value, void *data)

// Need Persistent Object hints to delete by manual.
#define V8H_AUTO_DELETE(ptr) \
	ptr->instance = v8::Persistent<v8::Object>::New(self);\
	ptr->instance.MakeWeak(NULL, destructor);

#define V8H_SYS_CHECK() \
	if ( errno ) { return V8H_EXCEPTION("%s", strerror(errno)); }


// Create function defination
#define V8H_FUNCTION_CREATE() \
	v8::Handle<v8::Function> create()

// Start Create Function
#define V8H_CREATE_START() \
	static v8::Persistent<v8::Function> function; \
	if (!function.IsEmpty()) \
		return function; \
	auto t = v8::FunctionTemplate::New(constructor); \
	auto o = t->InstanceTemplate(); \
	auto c = t->GetFunction(); \
	function = v8::Persistent<v8::Function>::New(c); \
	auto p = GET_OBJ(c, "prototype");

#define V8H_CREATE_OBJECT_START() \
	static v8::Persistent<v8::Object> object; \
	if (!object.IsEmpty()) \
		return object; \
	auto c = v8::Object::New(); \
	object = v8::Persistent<v8::Object>::New(c); \

#define V8H_CREATE_START_WITH_INTERNAL_FIELD() \
	static v8::Persistent<v8::Function> function; \
	if (!function.IsEmpty()) \
		return function; \
	auto t = v8::FunctionTemplate::New(constructor); \
	auto o = t->InstanceTemplate(); \
	o->SetInternalFieldCount(FIELDS_NUMBER); \
	auto c = t->GetFunction(); \
	function = v8::Persistent<v8::Function>::New(c); \
	auto p = GET_OBJ(c, "prototype");

#define V8H_CREATE_END() \
	return c;

#define V8H_IMPLEMENT(name) SET(p, #name, name)
#define V8H_IMPLEMENT_ALIAS(name, alias) p->Set(String::NewSymbol(#name), FunctionTemplate::New(name)->GetFunction())
#define V8H_STATIC_IMPLEMENT(name) SET(c, #name, name)
#define V8H_CONSTANT_INT(name) c->Set(v8::String::NewSymbol(#name), v8::Integer::New(name))
#define V8H_CONSTANT_PTR(name) c->Set(v8::String::NewSymbol(#name), v8::External::Wrap(name))

// use for V8::AdjustAmountOfExternalAllocatedMemory, the minimize size adjusted.
#define V8H_MEMORY_FIXED_SIZE(object) (sizeof(object) + 128)

#define V8H_PERSISTENT_OBJECT(name) \
	Persistent<Object> name() \
	{ \
		static auto name =  Persistent<Object>::New(new_instance()); \
		return name; \
	}

// Arguments Hepler
// ================
#define V8H_ARG(idx, func, default) \
	(argc > idx ? func(args[idx]) : default)


#define V8H_MAP(name) \
	SET(self, V8H_SYM(name), name)

#define V8H_MAP_VALUE(name, value) \
	SET(self, V8H_SYM(name), value)

#define V8H_STR(name, string) \
	v8::String::Utf8Value name##_value(string);

#define V8H_STRLEN(name) \
	name##_value.length()

// Assert and Throw
// ================
#define V8H_ASSERT(expression) \
	if (!(expression)) \
		return THROW_EXCEPTION(#expression);

#define V8H_ENSURE(expression) \
	if (!(expression)) \
		return THROW_EXCEPTION(#expression);
#define V8H_SYSTEM_ENSURE(expression) \
	if (!(expression)) \
		return THROW_SYSTEM_EXCEPTION(#expression);
#define V8H_THROW(expression) \
	if (expression) \
		return THROW_EXCEPTION(#expression);
#define V8H_SYSTEM_THROW(expression) \
	if (expression) \
		return THROW_SYSTEM_EXCEPTION(#expression);

// Logger
#ifdef V8H_NDEBUG
# define V8H_DEBUG(...) ;
#else
# define V8H_DEBUG(...) fputs("[DEBUG] ", stderr);fprintf(stderr, __VA_ARGS__);fputs("\n", stderr);
#endif

#include "v8h_function.h"
#include "v8h_helper.h"

// Uint32 MAX
// ==========
#endif
