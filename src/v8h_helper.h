#ifndef V8H_HELPER_H
#define V8H_HELPER_H

#include <stdlib.h>
#include <error.h>
#include <errno.h>
#include <stdarg.h>
#include <assert.h>
#include <v8.h>

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
