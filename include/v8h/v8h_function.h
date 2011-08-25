#ifndef V8H_FUNCTION_H
#define V8H_FUNCTION_H
#define V8H_C_FUNCTION_0_0(name) \
	Handle<Value> name(const Arguments &args) \
	{ \
		::name(); \
		return args.This(); \
	}

#define V8H_C_FUNCTION_0_1(name, ret_type) \
	Handle<Value> name(const Arguments &args) \
	{ \
    return ret_type(::name()); \
	}

#define V8H_C_FUNCTION_1_0(name, arg0_type) \
	Handle<Value> name(const Arguments &args) \
	{ \
		::name(arg0_type(args[0])); \
		return args.This(); \
	}

#define V8H_C_FUNCTION_1_1(name, arg0_type, ret_type) \
	Handle<Value> name(const Arguments &args) \
	{ \
    return ret_type(::name(arg0_type(args[0]))); \
	}

#define V8H_C_FUNCTION_2_0(name, arg0_type, arg1_type) \
	Handle<Value> name(const Arguments &args) \
	{ \
		::name(arg0_type(args[0]), arg1_type(args[1])); \
		return args.This(); \
	}

#define V8H_C_FUNCTION_2_1(name, arg0_type, arg1_type, ret_type) \
	Handle<Value> name(const Arguments &args) \
	{ \
    return ret_type(::name(arg0_type(args[0]), arg1_type(args[1]))); \
	}

#define V8H_C_FUNCTION_3_0(name, arg0_type, arg1_type, arg2_type) \
	Handle<Value> name(const Arguments &args) \
	{ \
		::name(arg0_type(args[0]), arg1_type(args[1]), arg2_type(args[2])); \
		return args.This(); \
	}

#define V8H_C_FUNCTION_3_1(name, arg0_type, arg1_type, arg2_type, ret_type) \
	Handle<Value> name(const Arguments &args) \
	{ \
    return ret_type(::name(arg0_type(args[0]), arg1_type(args[1]), arg2_type(args[2]))); \
	}

#define V8H_C_FUNCTION_4_0(name, arg0_type, arg1_type, arg2_type, arg3_type) \
	Handle<Value> name(const Arguments &args) \
	{ \
		::name(arg0_type(args[0]), arg1_type(args[1]), arg2_type(args[2]), arg3_type(args[3])); \
		return args.This(); \
	}

#define V8H_C_FUNCTION_4_1(name, arg0_type, arg1_type, arg2_type, arg3_type, ret_type) \
	Handle<Value> name(const Arguments &args) \
	{ \
    return ret_type(::name(arg0_type(args[0]), arg1_type(args[1]), arg2_type(args[2]), arg3_type(args[3]))); \
	}

#define V8H_C_FUNCTION_5_0(name, arg0_type, arg1_type, arg2_type, arg3_type, arg4_type) \
	Handle<Value> name(const Arguments &args) \
	{ \
		::name(arg0_type(args[0]), arg1_type(args[1]), arg2_type(args[2]), arg3_type(args[3]), arg4_type(args[4])); \
		return args.This(); \
	}

#define V8H_C_FUNCTION_5_1(name, arg0_type, arg1_type, arg2_type, arg3_type, arg4_type, ret_type) \
	Handle<Value> name(const Arguments &args) \
	{ \
    return ret_type(::name(arg0_type(args[0]), arg1_type(args[1]), arg2_type(args[2]), arg3_type(args[3]), arg4_type(args[4]))); \
	}

#endif
