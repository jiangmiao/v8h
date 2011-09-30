lines = []
lines << "#ifndef V8H_FUNCTION_H
#define V8H_FUNCTION_H"
for argc in (0..5)
	argc_range = (0..argc-1)
  for has_return in (0..1)
    args = []
    args << "name"
		argc_range.collect{|i| args << "arg#{i}_type"}
		args2 = argc_range.collect{|i| "\t\tauto arg#{i} = arg#{i}_type(args[#{i}]);"}.join(" \\\n")
		args3 = argc_range.collect{|i| "arg#{i}_type(args[#{i}])"}.join(", ")
    if has_return == 1
      args << "ret_type"
      args = args.join(", ")
      str = <<EOT
#define V8H_C_FUNCTION_#{argc}_#{has_return}(#{args}) \\
	v8::Handle<v8::Value> name(const v8::Arguments &args) \\
	{ \\
		return ret_type(::name(#{args3})); \\
	}

#define V8H_FORWARD_FUNCTION_#{argc}_#{has_return}(#{args}) \\
	V8H_FUNCTION(name) \\
	{ \\
		auto self = args.This(); \\
		auto instance = getInternal(self); \\
		return ret_type(instance->name(#{args3})); \\
	}

#define V8H_FORWARD_#{argc}_#{has_return}(klass, #{args}) \\
	V8H_FUNCTION(klass::name) \\
	{ \\
		auto self = args.This(); \\
		auto instance = getInternal(self); \\
		return ret_type(instance->name(#{args3})); \\
	}
EOT
    else
      args = args.join(", ")
      str = <<EOT
#define V8H_C_FUNCTION_#{argc}_#{has_return}(#{args}) \\
	V8H_FUNCTION(name) \\
	{ \\
		::name(#{args3}); \\
		return args.This(); \\
	}

#define V8H_C_FUNCTION_#{argc}(#{args}) \\
	V8H_FUNCTION(name) \\
	{ \\
		::name(#{args3}); \\
		return args.This(); \\
	}

#define V8H_FORWARD_FUNCTION_#{argc}(#{args}) \\
	V8H_FUNCTION(name) \\
	{ \\
		auto self = args.This(); \\
		auto instance = getInternal(self); \\
		instance->name(#{args3}); \\
		return self; \\
	}

#define V8H_FORWARD_#{argc}_#{has_return}(klass, #{args}) \\
	V8H_FUNCTION(klass::name) \\
	{ \\
		auto self = args.This(); \\
		auto instance = getInternal(self); \\
		instance->name(#{args3}); \\
		return self; \\
	}


#define V8H_FORWARD_#{argc}(klass, #{args}) \\
	V8H_FUNCTION(klass::name) \\
	{ \\
		auto self = args.This(); \\
		auto instance = getInternal(self); \\
		instance->name(#{args3}); \\
		return self; \\
	}

EOT
    end
    lines << str
  end
end
lines << "#endif"
lines << ""
File.open("v8h_function.h", "w").write lines.join("\n")
