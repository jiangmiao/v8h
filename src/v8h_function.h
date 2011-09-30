#ifndef V8H_FUNCTION_H
#define V8H_FUNCTION_H
#define V8H_C_FUNCTION_0_0(name) \
	V8H_FUNCTION(name) \
	{ \
		::name(); \
		return args.This(); \
	}

#define V8H_C_FUNCTION_0(name) \
	V8H_FUNCTION(name) \
	{ \
		::name(); \
		return args.This(); \
	}

#define V8H_FORWARD_FUNCTION_0(name) \
	V8H_FUNCTION(name) \
	{ \
		auto self = args.This(); \
		auto instance = getInternal(self); \
		instance->name(); \
		return self; \
	}

#define V8H_FORWARD_0_0(klass, name) \
	V8H_FUNCTION(klass::name) \
	{ \
		auto self = args.This(); \
		auto instance = getInternal(self); \
		instance->name(); \
		return self; \
	}


#define V8H_FORWARD_0(klass, name) \
	V8H_FUNCTION(klass::name) \
	{ \
		auto self = args.This(); \
		auto instance = getInternal(self); \
		instance->name(); \
		return self; \
	}


#define V8H_C_FUNCTION_0_1(name, ret_type) \
	v8::Handle<v8::Value> name(const v8::Arguments &args) \
	{ \
		return ret_type(::name()); \
	}

#define V8H_FORWARD_FUNCTION_0_1(name, ret_type) \
	V8H_FUNCTION(name) \
	{ \
		auto self = args.This(); \
		auto instance = getInternal(self); \
		return ret_type(instance->name()); \
	}

#define V8H_FORWARD_0_1(klass, name, ret_type) \
	V8H_FUNCTION(klass::name) \
	{ \
		auto self = args.This(); \
		auto instance = getInternal(self); \
		return ret_type(instance->name()); \
	}

#define V8H_C_FUNCTION_1_0(name, arg0_type) \
	V8H_FUNCTION(name) \
	{ \
		::name(arg0_type(args[0])); \
		return args.This(); \
	}

#define V8H_C_FUNCTION_1(name, arg0_type) \
	V8H_FUNCTION(name) \
	{ \
		::name(arg0_type(args[0])); \
		return args.This(); \
	}

#define V8H_FORWARD_FUNCTION_1(name, arg0_type) \
	V8H_FUNCTION(name) \
	{ \
		auto self = args.This(); \
		auto instance = getInternal(self); \
		instance->name(arg0_type(args[0])); \
		return self; \
	}

#define V8H_FORWARD_1_0(klass, name, arg0_type) \
	V8H_FUNCTION(klass::name) \
	{ \
		auto self = args.This(); \
		auto instance = getInternal(self); \
		instance->name(arg0_type(args[0])); \
		return self; \
	}


#define V8H_FORWARD_1(klass, name, arg0_type) \
	V8H_FUNCTION(klass::name) \
	{ \
		auto self = args.This(); \
		auto instance = getInternal(self); \
		instance->name(arg0_type(args[0])); \
		return self; \
	}


#define V8H_C_FUNCTION_1_1(name, arg0_type, ret_type) \
	v8::Handle<v8::Value> name(const v8::Arguments &args) \
	{ \
		return ret_type(::name(arg0_type(args[0]))); \
	}

#define V8H_FORWARD_FUNCTION_1_1(name, arg0_type, ret_type) \
	V8H_FUNCTION(name) \
	{ \
		auto self = args.This(); \
		auto instance = getInternal(self); \
		return ret_type(instance->name(arg0_type(args[0]))); \
	}

#define V8H_FORWARD_1_1(klass, name, arg0_type, ret_type) \
	V8H_FUNCTION(klass::name) \
	{ \
		auto self = args.This(); \
		auto instance = getInternal(self); \
		return ret_type(instance->name(arg0_type(args[0]))); \
	}

#define V8H_C_FUNCTION_2_0(name, arg0_type, arg1_type) \
	V8H_FUNCTION(name) \
	{ \
		::name(arg0_type(args[0]), arg1_type(args[1])); \
		return args.This(); \
	}

#define V8H_C_FUNCTION_2(name, arg0_type, arg1_type) \
	V8H_FUNCTION(name) \
	{ \
		::name(arg0_type(args[0]), arg1_type(args[1])); \
		return args.This(); \
	}

#define V8H_FORWARD_FUNCTION_2(name, arg0_type, arg1_type) \
	V8H_FUNCTION(name) \
	{ \
		auto self = args.This(); \
		auto instance = getInternal(self); \
		instance->name(arg0_type(args[0]), arg1_type(args[1])); \
		return self; \
	}

#define V8H_FORWARD_2_0(klass, name, arg0_type, arg1_type) \
	V8H_FUNCTION(klass::name) \
	{ \
		auto self = args.This(); \
		auto instance = getInternal(self); \
		instance->name(arg0_type(args[0]), arg1_type(args[1])); \
		return self; \
	}


#define V8H_FORWARD_2(klass, name, arg0_type, arg1_type) \
	V8H_FUNCTION(klass::name) \
	{ \
		auto self = args.This(); \
		auto instance = getInternal(self); \
		instance->name(arg0_type(args[0]), arg1_type(args[1])); \
		return self; \
	}


#define V8H_C_FUNCTION_2_1(name, arg0_type, arg1_type, ret_type) \
	v8::Handle<v8::Value> name(const v8::Arguments &args) \
	{ \
		return ret_type(::name(arg0_type(args[0]), arg1_type(args[1]))); \
	}

#define V8H_FORWARD_FUNCTION_2_1(name, arg0_type, arg1_type, ret_type) \
	V8H_FUNCTION(name) \
	{ \
		auto self = args.This(); \
		auto instance = getInternal(self); \
		return ret_type(instance->name(arg0_type(args[0]), arg1_type(args[1]))); \
	}

#define V8H_FORWARD_2_1(klass, name, arg0_type, arg1_type, ret_type) \
	V8H_FUNCTION(klass::name) \
	{ \
		auto self = args.This(); \
		auto instance = getInternal(self); \
		return ret_type(instance->name(arg0_type(args[0]), arg1_type(args[1]))); \
	}

#define V8H_C_FUNCTION_3_0(name, arg0_type, arg1_type, arg2_type) \
	V8H_FUNCTION(name) \
	{ \
		::name(arg0_type(args[0]), arg1_type(args[1]), arg2_type(args[2])); \
		return args.This(); \
	}

#define V8H_C_FUNCTION_3(name, arg0_type, arg1_type, arg2_type) \
	V8H_FUNCTION(name) \
	{ \
		::name(arg0_type(args[0]), arg1_type(args[1]), arg2_type(args[2])); \
		return args.This(); \
	}

#define V8H_FORWARD_FUNCTION_3(name, arg0_type, arg1_type, arg2_type) \
	V8H_FUNCTION(name) \
	{ \
		auto self = args.This(); \
		auto instance = getInternal(self); \
		instance->name(arg0_type(args[0]), arg1_type(args[1]), arg2_type(args[2])); \
		return self; \
	}

#define V8H_FORWARD_3_0(klass, name, arg0_type, arg1_type, arg2_type) \
	V8H_FUNCTION(klass::name) \
	{ \
		auto self = args.This(); \
		auto instance = getInternal(self); \
		instance->name(arg0_type(args[0]), arg1_type(args[1]), arg2_type(args[2])); \
		return self; \
	}


#define V8H_FORWARD_3(klass, name, arg0_type, arg1_type, arg2_type) \
	V8H_FUNCTION(klass::name) \
	{ \
		auto self = args.This(); \
		auto instance = getInternal(self); \
		instance->name(arg0_type(args[0]), arg1_type(args[1]), arg2_type(args[2])); \
		return self; \
	}


#define V8H_C_FUNCTION_3_1(name, arg0_type, arg1_type, arg2_type, ret_type) \
	v8::Handle<v8::Value> name(const v8::Arguments &args) \
	{ \
		return ret_type(::name(arg0_type(args[0]), arg1_type(args[1]), arg2_type(args[2]))); \
	}

#define V8H_FORWARD_FUNCTION_3_1(name, arg0_type, arg1_type, arg2_type, ret_type) \
	V8H_FUNCTION(name) \
	{ \
		auto self = args.This(); \
		auto instance = getInternal(self); \
		return ret_type(instance->name(arg0_type(args[0]), arg1_type(args[1]), arg2_type(args[2]))); \
	}

#define V8H_FORWARD_3_1(klass, name, arg0_type, arg1_type, arg2_type, ret_type) \
	V8H_FUNCTION(klass::name) \
	{ \
		auto self = args.This(); \
		auto instance = getInternal(self); \
		return ret_type(instance->name(arg0_type(args[0]), arg1_type(args[1]), arg2_type(args[2]))); \
	}

#define V8H_C_FUNCTION_4_0(name, arg0_type, arg1_type, arg2_type, arg3_type) \
	V8H_FUNCTION(name) \
	{ \
		::name(arg0_type(args[0]), arg1_type(args[1]), arg2_type(args[2]), arg3_type(args[3])); \
		return args.This(); \
	}

#define V8H_C_FUNCTION_4(name, arg0_type, arg1_type, arg2_type, arg3_type) \
	V8H_FUNCTION(name) \
	{ \
		::name(arg0_type(args[0]), arg1_type(args[1]), arg2_type(args[2]), arg3_type(args[3])); \
		return args.This(); \
	}

#define V8H_FORWARD_FUNCTION_4(name, arg0_type, arg1_type, arg2_type, arg3_type) \
	V8H_FUNCTION(name) \
	{ \
		auto self = args.This(); \
		auto instance = getInternal(self); \
		instance->name(arg0_type(args[0]), arg1_type(args[1]), arg2_type(args[2]), arg3_type(args[3])); \
		return self; \
	}

#define V8H_FORWARD_4_0(klass, name, arg0_type, arg1_type, arg2_type, arg3_type) \
	V8H_FUNCTION(klass::name) \
	{ \
		auto self = args.This(); \
		auto instance = getInternal(self); \
		instance->name(arg0_type(args[0]), arg1_type(args[1]), arg2_type(args[2]), arg3_type(args[3])); \
		return self; \
	}


#define V8H_FORWARD_4(klass, name, arg0_type, arg1_type, arg2_type, arg3_type) \
	V8H_FUNCTION(klass::name) \
	{ \
		auto self = args.This(); \
		auto instance = getInternal(self); \
		instance->name(arg0_type(args[0]), arg1_type(args[1]), arg2_type(args[2]), arg3_type(args[3])); \
		return self; \
	}


#define V8H_C_FUNCTION_4_1(name, arg0_type, arg1_type, arg2_type, arg3_type, ret_type) \
	v8::Handle<v8::Value> name(const v8::Arguments &args) \
	{ \
		return ret_type(::name(arg0_type(args[0]), arg1_type(args[1]), arg2_type(args[2]), arg3_type(args[3]))); \
	}

#define V8H_FORWARD_FUNCTION_4_1(name, arg0_type, arg1_type, arg2_type, arg3_type, ret_type) \
	V8H_FUNCTION(name) \
	{ \
		auto self = args.This(); \
		auto instance = getInternal(self); \
		return ret_type(instance->name(arg0_type(args[0]), arg1_type(args[1]), arg2_type(args[2]), arg3_type(args[3]))); \
	}

#define V8H_FORWARD_4_1(klass, name, arg0_type, arg1_type, arg2_type, arg3_type, ret_type) \
	V8H_FUNCTION(klass::name) \
	{ \
		auto self = args.This(); \
		auto instance = getInternal(self); \
		return ret_type(instance->name(arg0_type(args[0]), arg1_type(args[1]), arg2_type(args[2]), arg3_type(args[3]))); \
	}

#define V8H_C_FUNCTION_5_0(name, arg0_type, arg1_type, arg2_type, arg3_type, arg4_type) \
	V8H_FUNCTION(name) \
	{ \
		::name(arg0_type(args[0]), arg1_type(args[1]), arg2_type(args[2]), arg3_type(args[3]), arg4_type(args[4])); \
		return args.This(); \
	}

#define V8H_C_FUNCTION_5(name, arg0_type, arg1_type, arg2_type, arg3_type, arg4_type) \
	V8H_FUNCTION(name) \
	{ \
		::name(arg0_type(args[0]), arg1_type(args[1]), arg2_type(args[2]), arg3_type(args[3]), arg4_type(args[4])); \
		return args.This(); \
	}

#define V8H_FORWARD_FUNCTION_5(name, arg0_type, arg1_type, arg2_type, arg3_type, arg4_type) \
	V8H_FUNCTION(name) \
	{ \
		auto self = args.This(); \
		auto instance = getInternal(self); \
		instance->name(arg0_type(args[0]), arg1_type(args[1]), arg2_type(args[2]), arg3_type(args[3]), arg4_type(args[4])); \
		return self; \
	}

#define V8H_FORWARD_5_0(klass, name, arg0_type, arg1_type, arg2_type, arg3_type, arg4_type) \
	V8H_FUNCTION(klass::name) \
	{ \
		auto self = args.This(); \
		auto instance = getInternal(self); \
		instance->name(arg0_type(args[0]), arg1_type(args[1]), arg2_type(args[2]), arg3_type(args[3]), arg4_type(args[4])); \
		return self; \
	}


#define V8H_FORWARD_5(klass, name, arg0_type, arg1_type, arg2_type, arg3_type, arg4_type) \
	V8H_FUNCTION(klass::name) \
	{ \
		auto self = args.This(); \
		auto instance = getInternal(self); \
		instance->name(arg0_type(args[0]), arg1_type(args[1]), arg2_type(args[2]), arg3_type(args[3]), arg4_type(args[4])); \
		return self; \
	}


#define V8H_C_FUNCTION_5_1(name, arg0_type, arg1_type, arg2_type, arg3_type, arg4_type, ret_type) \
	v8::Handle<v8::Value> name(const v8::Arguments &args) \
	{ \
		return ret_type(::name(arg0_type(args[0]), arg1_type(args[1]), arg2_type(args[2]), arg3_type(args[3]), arg4_type(args[4]))); \
	}

#define V8H_FORWARD_FUNCTION_5_1(name, arg0_type, arg1_type, arg2_type, arg3_type, arg4_type, ret_type) \
	V8H_FUNCTION(name) \
	{ \
		auto self = args.This(); \
		auto instance = getInternal(self); \
		return ret_type(instance->name(arg0_type(args[0]), arg1_type(args[1]), arg2_type(args[2]), arg3_type(args[3]), arg4_type(args[4]))); \
	}

#define V8H_FORWARD_5_1(klass, name, arg0_type, arg1_type, arg2_type, arg3_type, arg4_type, ret_type) \
	V8H_FUNCTION(klass::name) \
	{ \
		auto self = args.This(); \
		auto instance = getInternal(self); \
		return ret_type(instance->name(arg0_type(args[0]), arg1_type(args[1]), arg2_type(args[2]), arg3_type(args[3]), arg4_type(args[4]))); \
	}

#endif
