#ifndef V8H_MODULE_H
#define V8H_MODULE_H

V8H_NS_START
template<typename T>
class Module
{
    public:
	static V8H_FUNCTION(constructor)
	{
		return args.This();
	}
};

V8H_NS_END
#endif
