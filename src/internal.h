#ifndef V8H_INTERNAL_H
#define V8H_INTERNAL_H

#include <typeinfo>
#include "v8h.h"
V8H_NS_START
template<typename T>
class Internal
{
    public:
	enum {
		FIELD_INTERNAL,
		FIELDS_NUMBER
	};

	v8::Persistent<v8::Object> instance;
	virtual ~Internal()
	{
		if (!instance.IsEmpty()) {
			instance.Dispose();
		}
	}

	static T * getInternal(v8::Handle<v8::Object> object)
	{
		return (T*)GET_PTR(object, FIELD_INTERNAL);
	}

	// Get internal from any Handle<T>
	template<typename S>
	static T * getInternal(v8::Handle<S> value)
	{
		return getInternal(value->ToObject());
	}

	static void destructor(v8::Persistent<v8::Value> value, void *data)
	{
		auto object = value->ToObject();
		auto self = getInternal(object);
		if (self)
			delete self;
		V8H_DEBUG("%s wasn't free property.", typeid(T).name());
		SET_PTR(object, FIELD_INTERNAL, NULL);
	}

	static V8H_FUNCTION(close)
	{
		auto self = args.This();
		DEL_PTR<T*>(self, FIELD_INTERNAL);
		return v8::Undefined();
	}

	void wrap(v8::Handle<v8::Object> object)
	{
		SET_PTR(object, FIELD_INTERNAL, this);
		instance = v8::Persistent<v8::Object>::New(object);
		instance.MakeWeak(NULL, destructor);
	}
};
V8H_NS_END
#endif
