#ifndef V8H_V8H_H
#define V8H_V8H_H

#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

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
#define V8H_IMPORT_DESTRUCTOR_FUNCTION() \
        void destructor(Persistent<Value> value, void *data) { \
		DEL_PTR<InternalPointer>(value->ToObject(), FIELD_INTERNAL); \
		puts(__FUNCTION__); \
	}
#define V8H_IMPORT_NEW_INSTANCE() \
	Handle<Object> new_instance() \
	{ \
		return create()->NewInstance(); \
	}

// Need Persistent Object hints to delete by manual.
#define V8H_AUTO_DELETE(ptr) \
	ptr->instance = Persistent<Object>::New(self);\
	ptr->instance.MakeWeak(NULL, destructor);

#define V8H_SYS_CHECK() \
	if ( errno ) { return V8H_EXCEPTION("%s", strerror(errno)); }


#define V8H_FORWARD_FUNCTION_0(name) \
	V8H_FUNCTION(name) \
	{ \
		auto self = args.This(); \
		auto instance = get_internal(self); \
		instance->name(); \
		return self; \
	}

#define V8H_FORWARD_FUNCTION_1(name, arg0_type) \
	V8H_FUNCTION(name) \
	{ \
		auto self = args.This(); \
		auto arg0 = arg0_type(args[0]); \
		auto instance = get_internal(self); \
		instance->name(arg0); \
		return self; \
	}

#define V8H_FORWARD_FUNCTION_0_1(name, ret_type) \
	V8H_FUNCTION(name) \
	{ \
		auto self = args.This(); \
		auto instance = get_internal(self); \
		return ret_type(instance->name()); \
	}

#define V8H_FORWARD_FUNCTION_1_1(name, arg0_type, ret_type) \
	V8H_FUNCTION(name) \
	{ \
		auto self = args.This(); \
		auto arg0 = arg0_type(args[0]); \
		auto instance = get_internal(self); \
		return ret_type(instance->name(arg0)); \
	}


#define V8H_IMPORT_GET_INTERNAL() \
	InternalPointer get_internal(Handle<Object> self) \
	{ \
		return (InternalPointer)GET_PTR(self, FIELD_INTERNAL); \
	}

#define V8H_IMPORT_UNWRAP() \
	InternalPointer Unwrap(Handle<Value> value) \
	{ \
		return (FILE*)External::Unwrap(value); \
	}
	

#define V8H_IMPORT_GLOBAL() \
	Persistent<Object> global() \
	{ \
		static auto name =  Persistent<Object>::New(new_instance()); \
		return name; \
	}

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
#include <v8.h>
#include <stdarg.h>

namespace v8h 
{
	using namespace v8;
	// Anything
	template<typename T>
	inline Handle<T> SET(Handle<Object> object, const char *name, Handle<T> value)
	{
		object->Set(String::NewSymbol(name), value);
		return value;
	}

	template<typename T>
	inline Handle<T> SET(Handle<Object> object, Handle<Value> name, Handle<T> value)
	{
		object->Set(name, value);
		return value;
	}

	template<typename T>
	inline Handle<T> SET(Handle<Object> object, uint32_t name, Handle<T> value)
	{
		object->Set(name, value);
		return value;
	}

	// Function
	inline Handle<Function> SET(Handle<Object> object, const char *name, InvocationCallback callback)
	{
		auto function = FunctionTemplate::New(callback)->GetFunction();
		object->Set(String::NewSymbol(name), function);
		return function;
	}
	
	// Integer
	inline int32_t GET_INT(Handle<Object> object, Handle<Value> name)
	{
		return object->Get(name)->Int32Value();
	}

	inline uint32_t GET_UINT(Handle<Object> object, Handle<Value> name)
	{
		return object->Get(name)->Uint32Value();
	}



	inline Handle<Value> GET(Handle<Object> object, Handle<String> name)
	{
		return object->Get(name);
	}

	inline Handle<Value> GET(Handle<Object> object, const char *name)
	{
		return GET(object, String::NewSymbol(name));
	}


	inline void SET_INT(Handle<Object> object, Handle<Value> name, int value)
	{
		object->Set(name, Integer::New(value));
	}

	inline void SET(Handle<Object> object, const char *name, int value)
	{
		object->Set(String::NewSymbol(name), Integer::New(value));
	}

	inline void SET(Handle<Object> object, Handle<Value> name, int value)
	{
		object->Set(name, Integer::New(value));
	}

	// Object
	inline Handle<Object> GET_OBJ(Handle<Object> object, Handle<Value> name)
	{
		return object->Get(name)->ToObject();
	}

	inline Handle<Object> GET_OBJ(Handle<Object> object, const char *name)
	{
		return object->Get(String::NewSymbol(name))->ToObject();
	}



	// Pointer
	inline void* GET_PTR(Handle<Object> object, int field)
	{
		assert(field < object->InternalFieldCount() );
		return object->GetPointerFromInternalField(field);
	}

	inline void SET_PTR(Handle<Object> object, int field, void *data)
	{
		object->SetPointerInInternalField(field, data);
	}

	// Setter
	inline void SET_ACC(Handle<Object> object, Handle<String> name, AccessorGetter getter, AccessorSetter setter)
	{
		object->SetAccessor(name, getter, setter);
	}

	inline void SET_ACC(Handle<Object> object, const char *name, AccessorGetter getter, AccessorSetter setter = 0)
	{
		SET_ACC(object, String::NewSymbol(name), getter, setter);
	}

	// Delete object
	template<typename T>
	inline void DEL_PTR(Handle<Object> object, int field)
	{
		T pointer = (T)object->GetPointerFromInternalField(field);
		if (pointer) {
			delete pointer;
			object->SetPointerInInternalField(field, NULL);
		}
	}

	// Convert to C++ Value
	template<typename T>
	inline int TO_INT(Handle<T> object)
	{
		return object->Int32Value();
	}

	template<typename T>
	inline int32_t TO_INT32(Handle<T> object)
	{
		return object->Int32Value();
	}

	template<typename T>
	inline uint32_t TO_UINT32(Handle<T> object)
	{
		return object->Uint32Value();
	}
	template<typename T>
	inline int64_t TO_INT64(Handle<T> object)
	{
		return object->IntegerValue();
	}

	template<typename T>
	inline double TO_DOUBLE(Handle<T> object)
	{
		return object->NumberValue();
	}

	template<typename T>
	inline unsigned int TO_UINT(Handle<T> object)
	{
		return object->Uint32Value();
	}


	template<typename T>
	inline T TO_PTR(Handle<Value> value)
	{
		return (T)External::Unwrap(value);
	}

	class SmartString
	{
	    public:
		Handle<Value> value_;
		char          *data_;
		SmartString(Handle<Value> value)
		: value_(value)
		, data_(NULL)
		{

		}

		~SmartString()
		{
			if (data_) {
				free(data_);
			}
		}

		operator char*() 
		{
			if (value_->IsExternal()||value_->IsNumber()) {
				return TO_PTR<char*>(value_);
			} else {
				auto str    = value_->ToString();
				int  length = str->Utf8Length();
				data_  = (char*)malloc(length+1);
				str->WriteUtf8(data_, length);
				data_[length] = 0;
				return data_;
			}
		}
	};

	#define TO_PCHAR(name) SmartString(name)

	template<typename T>
	inline Handle<Object> TO_OBJ(Handle<T> object)
	{
		return object->ToObject();
	}

	template<typename T>
	inline Handle<Function> TO_FUNCUTION(Handle<T> object)
	{
		return Handle<Function>::Cast(object);
	}

	// Create V8 Value
	inline Handle<Integer> V8_INT(int n)
	{
		return Integer::New(n);
	}

	inline Handle<Integer> V8_UINT(unsigned int n)
	{
		return Integer::NewFromUnsigned(n);
	}

	inline Handle<Boolean> V8_BOOLEAN(bool n)
	{
		return Boolean::New(n);
	}

	inline Handle<Object> V8_OBJ()
	{
		return Object::New();
	}

	inline Handle<Value> V8_PTR(void *ptr)
	{
		return External::Wrap(ptr);
	}

	inline Handle<Value> V8_PTR_OR_NULL(void *ptr)
	{
		if (ptr == NULL)
			return Null();
		return V8_PTR(ptr);
	}

	inline Handle<Value> V8_INT32(uint32_t n)
	{
		return Int32::New(n);
	}

	inline Handle<Value> V8_UINT32(int32_t n)
	{
		return Uint32::New(n);
	}

	inline Handle<Value> V8_INT64(int64_t n)
	{
		return Integer::New(n);
	}

	inline Handle<Value> V8_DOUBLE(double n)
	{
		return Number::New(n);
	}

	// Throw exception
	inline Handle<Value> THROW_EXCEPTION(const char *format, ...)
	{
		va_list args;
		va_start(args, format);

		static char buffer[512];
		int n = vsnprintf(buffer, sizeof(buffer), format, args);
		va_end(args);
		return ThrowException(Exception::Error(String::New(buffer, n)));
	}

	inline Handle<Value> THROW_SYSTEM_EXCEPTION(const char *format)
	{
		static char buffer[512];
		int n = snprintf(buffer, sizeof(buffer), format, errno);
		return ThrowException(Exception::Error(String::New(buffer, n)));
	}

}

#endif
