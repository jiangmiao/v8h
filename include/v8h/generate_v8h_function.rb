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
      str = <<EOT
#define V8H_C_FUNCTION_#{argc}_#{has_return}(#{args.join(", ")}) \\
	Handle<Value> name(const Arguments &args) \\
	{ \\
    return ret_type(::name(#{args3})); \\
	}
EOT
    else
      str = <<EOT
#define V8H_C_FUNCTION_#{argc}_#{has_return}(#{args.join(", ")}) \\
	Handle<Value> name(const Arguments &args) \\
	{ \\
		::name(#{args3}); \\
		return args.This(); \\
	}
EOT
    end
    lines << str
  end
end
lines << "#endif"
lines << ""
File.open("v8h_function.h", "w").write lines.join("\n")
