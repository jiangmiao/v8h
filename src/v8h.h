#ifndef V8H_V8H_H
#define V8H_V8H_H

#include <v8.h>
#include "algorithm.h"

#define V8H_ENTER() HandleScope scope; \
	auto self = args.This();

#define V8H_LEAVE(arg) return scope.Close(arg);
#define V8H_LEAVE_INT(arg) return scope.Close(Integer::New(arg));
#define V8H_EXIT() return scope.Close(self);
#define V8H_SELF() auto self = args.This();
#define V8H_RETURN(arg) return arg;
#define V8H_RETURN_SELF() return self;
#define V8H_RETURN_INT(arg) return Integer::New(arg);

#define V8H_FUNCTION(name) Handle<Value> name(const Arguments &args)
#define V8H_LAMBDA [](const Arguments &args) -> Handle<Value>
#define V8H_SETTER(name) Handle<Value> name(Local<String> property, Local<Value> value, const AccessorInfo &info)
#define V8H_GETTER(name) Handle<Value> name(Local<String> property, const AccessorInfo &info)


#define V8H_INT_ARG(idx) args[idx]->Int32Value()

#define V8H_DEF_SYM(name) Persistent<String> name##_sym
#define V8H_SET_SYM(name) name##_sym = Persistent<String>::New(String::NewSymbol(#name))
#define V8H_GET_SYM(name) name##_sym
#define V8H_SYM(name) name##_sym

#define V8H_DEF_PER(type, name) Persistent<type> name
#define V8H_SET_PER(type, name, value) name = Persistent<type>(type::New(value))

#define V8H_CONSTANT_INT(name) c->Set(String::NewSymbol(#name), Integer::New(name))
#define V8H_CONSTANT_PTR(name) c->Set(String::NewSymbol(#name), External::Wrap(name))

// Auto Delete object
#define V8H_DESTRUCTOR() \
	void destructor(Persistent<Value> value, void *data)

// Need Persistent Object hints to delete by manual.
#define V8H_AUTO_DELETE(ptr) \
	ptr->instance = Persistent<Object>::New(self);\
	ptr->instance.MakeWeak(NULL, destructor);

#define V8H_SYS_CHECK() \
	if ( errno ) { return V8H_EXCEPTION("%s", strerror(errno)); }


// Create function defination
#define V8H_CREATE_START() \
	static Persistent<Function> function; \
	if (!function.IsEmpty()) \
		return function; \
	auto t = FunctionTemplate::New(constructor); \
	auto o = t->InstanceTemplate(); \
	auto c = t->GetFunction(); \
	function = Persistent<Function>::New(c); \
	auto p = GET_OBJ(c, "prototype");
	
#define V8H_CREATE_START_WITH_INTERNAL_FIELD() \
	static Persistent<Function> function; \
	if (!function.IsEmpty()) \
		return function; \
	auto t = FunctionTemplate::New(constructor); \
	auto o = t->InstanceTemplate(); \
	o->SetInternalFieldCount(FIELDS_NUMBER); \
	auto c = t->GetFunction(); \
	function = Persistent<Function>::New(c); \
	auto p = GET_OBJ(c, "prototype");

#define V8H_CREATE_END() \
	return c;

#define V8H_IMPLEMENT(name) SET(p, underline_to_camel(#name), name)
#define V8H_IMPLEMENT_ALIAS(name, alias) p->Set(String::NewSymbol(#name), FunctionTemplate::New(name)->GetFunction())
#define V8H_STATIC_IMPLEMENT(name) SET(c, underline_to_camel(#name), name)
#define V8H_C(name) SET(c, #name, name)
#define V8H_I(name) V8H_IMPLEMENT(name)
#define V8H_SI(name) V8H_STATIC_IMPLEMENT(name)

// use for V8::AdjustAmountOfExternalAllocatedMemory, the minimize size adjusted.
#define V8H_MEMORY_FIXED_SIZE 128

#define V8H_PERSISTENT_OBJECT(name) \
	Persistent<Object> name() \
	{ \
		static auto name =  Persistent<Object>::New(new_instance()); \
		return name; \
	}

#include "v8h_function.h"
#include "v8h_import.h"
#include "v8h_helper.h"
#endif
