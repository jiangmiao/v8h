#ifndef V8H_OBJECT_H
#define V8H_OBJECT_H

V8H_NS_START
template<typename T>
class Object
{
    public:
	static v8::Handle<v8::Object> newInstance()
	{
		return T::create()->NewInstance();
	}

	static v8::Handle<v8::Object> global()
	{
		static v8::Persistent<v8::Object> global = newInstance();
		return global;
	}

	static V8H_FUNCTION(constructor)
	{
		return args.This();
	}
};

V8H_NS_END
#endif
