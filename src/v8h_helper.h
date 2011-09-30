#ifndef V8H_HELPER_H
#define V8H_HELPER_H

#include <stdlib.h>
#include <error.h>
#include <errno.h>
#include <stdarg.h>
#include <assert.h>
#include <v8.h>

V8H_NS_START

// Set Anything
// ============
template<typename T>
inline v8::Handle<T> SET(v8::Handle<v8::Object> object, v8::Handle<v8::Value> name, v8::Handle<T> value)
{
	object->Set(name, value);
	return value;
}

template<typename T>
inline v8::Handle<T> SET(v8::Handle<v8::Object> object, const char *name, v8::Handle<T> value)
{
	object->Set(v8::String::NewSymbol(name), value);
	return value;
}

template<typename T>
inline v8::Handle<T> SET(v8::Handle<v8::Object> object, uint32_t index, v8::Handle<T> value)
{
	object->Set(index, value);
	return value;
}

// v8::Function
// ========
inline v8::Handle<v8::Function> SET(v8::Handle<v8::Object> object, const char *name, v8::InvocationCallback callback)
{
	auto function = v8::FunctionTemplate::New(callback)->GetFunction();
	object->Set(v8::String::NewSymbol(name), function);
	return function;
}


// Integer
// =======
inline int32_t GET_INT(v8::Handle<v8::Object> object, v8::Handle<v8::Value> name)
{
	return object->Get(name)->Int32Value();
}

inline uint32_t GET_UINT(v8::Handle<v8::Object> object, v8::Handle<v8::Value> name)
{
	return object->Get(name)->Uint32Value();
}

inline v8::Handle<v8::Value> GET(v8::Handle<v8::Object> object, v8::Handle<v8::String> name)
{
	return object->Get(name);
}

inline v8::Handle<v8::Value> GET(v8::Handle<v8::Object> object, const char *name)
{
	return GET(object, v8::String::NewSymbol(name));
}


inline void SET_INT(v8::Handle<v8::Object> object, v8::Handle<v8::Value> name, int value)
{
	object->Set(name, v8::Integer::New(value));
}

inline void SET(v8::Handle<v8::Object> object, const char *name, int value)
{
	object->Set(v8::String::NewSymbol(name), v8::Integer::New(value));
}

inline void SET(v8::Handle<v8::Object> object, v8::Handle<v8::Value> name, int value)
{
	object->Set(name, v8::Integer::New(value));
}


// v8::Object
// ==========
inline v8::Handle<v8::Object> GET_OBJ(v8::Handle<v8::Object> object, v8::Handle<v8::Value> name)
{
	return object->Get(name)->ToObject();
}

inline v8::Handle<v8::Object> GET_OBJ(v8::Handle<v8::Object> object, const char *name)
{
	return object->Get(v8::String::NewSymbol(name))->ToObject();
}



// Pointer
// =======
inline void* GET_PTR(v8::Handle<v8::Object> object, int field)
{
	assert(field < object->InternalFieldCount() );
	return object->GetPointerFromInternalField(field);
}

inline void SET_PTR(v8::Handle<v8::Object> object, int field, void *ptr)
{
	object->SetPointerInInternalField(field, ptr);
}


// Setter
// ======
inline void SET_ACC(v8::Handle<v8::Object> object, v8::Handle<v8::String> name, v8::AccessorGetter getter, v8::AccessorSetter setter)
{
	object->SetAccessor(name, getter, setter);
}

inline void SET_ACC(v8::Handle<v8::Object> object, const char *name, v8::AccessorGetter getter, v8::AccessorSetter setter = 0)
{
	SET_ACC(object, v8::String::NewSymbol(name), getter, setter);
}


// Delete object
// =============
template<typename T>
inline void DEL_PTR(v8::Handle<v8::Object> object, int field)
{
	T pointer = (T)object->GetPointerFromInternalField(field);
	if (pointer) {
		delete pointer;
		object->SetPointerInInternalField(field, NULL);
	}
}


// Convert to C++ v8::Value
template<typename T>
inline int TO_INT(v8::Handle<T> object)
{
	return object->Int32Value();
}

template<typename T>
inline uint32_t TO_UINT(v8::Handle<T> object)
{
	return object->Uint32Value();
}

template<typename T>
inline int32_t TO_INT32(v8::Handle<T> object)
{
	return object->Int32Value();
}

template<typename T>
inline uint32_t TO_UINT32(v8::Handle<T> object)
{
	return object->Uint32Value();
}

template<typename T>
inline int64_t TO_INT64(v8::Handle<T> object)
{
	return object->IntegerValue();
}

template<typename T>
inline double TO_DOUBLE(v8::Handle<T> object)
{
	return object->NumberValue();
}

template<typename T>
inline T TO_PTR(v8::Handle<v8::Value> value)
{
	return (T)v8::External::Unwrap(value);
}

class SmartString
{
    public:
	v8::Handle<v8::Value> value_;
	char          *data_;
	SmartString(v8::Handle<v8::Value> value)
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

#define TO_PCHAR(name) Smartv8::String(name)

template<typename T>
inline v8::Handle<v8::Object> TO_OBJ(v8::Handle<T> object)
{
	return object->ToObject();
}

template<typename T>
inline v8::Handle<v8::Function> TO_FUNCUTION(v8::Handle<T> object)
{
	return v8::Handle<v8::Function>::Cast(object);
}

// Create V8 v8::Value
// ===================
inline v8::Handle<v8::Integer> V8_INT(int n)
{
	return v8::Integer::New(n);
}

inline v8::Handle<v8::Integer> V8_UINT(unsigned int n)
{
	return v8::Integer::NewFromUnsigned(n);
}

inline v8::Handle<v8::Boolean> V8_BOOLEAN(bool n)
{
	return v8::Boolean::New(n);
}

inline v8::Handle<v8::Object> V8_OBJ()
{
	return v8::Object::New();
}

inline v8::Handle<v8::Value> V8_PTR(void *ptr)
{
	return v8::External::Wrap(ptr);
}

inline v8::Handle<v8::Value> V8_PTR_OR_NULL(void *ptr)
{
	if (ptr == NULL)
		return v8::Null();
	return V8_PTR(ptr);
}

inline v8::Handle<v8::Value> V8_INT32(int32_t n)
{
	return v8::Int32::New(n);
}

inline v8::Handle<v8::Value> V8_UINT32(uint32_t n)
{
	return v8::Uint32::NewFromUnsigned(n);
}

inline v8::Handle<v8::Value> V8_INT64(int64_t n)
{
	// TODO
	// use double store int64_t value for JS, maybe loose precision
	return v8::Number::New((double)n);
}

inline v8::Handle<v8::Value> V8_DOUBLE(double n)
{
	return v8::Number::New(n);
}


// Throw exception
// ===============
inline v8::Handle<v8::Value> THROW_EXCEPTION(const char *message)
{
	return ThrowException(v8::Exception::Error(v8::String::New(message)));
}

inline v8::Handle<v8::Value> THROW_SYSTEM_EXCEPTION(const char *name)
{
	static char buffer[512];
	int n = snprintf(buffer, sizeof(buffer), "%s : %s", name, strerror(errno));
	return ThrowException(v8::Exception::Error(v8::String::New(buffer, n)));
}


// Limit
// =====
const uint32_t UINT32_MAX = 0xFFFFFFFFL;
const int32_t INT32_MAX   = 0x7FFFFFFFL;
const int64_t INT64_MAX   = 0x7FFFFFFFFFFFFFFFLL;
V8H_NS_END

#endif
