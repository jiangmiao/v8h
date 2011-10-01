import os
import SCons.Util

vars = Variables()
vars.Add('mode', 'compilation mode (debug, release)', 'release')
vars.Add('v8path', 'V8 path')
env = Environment(variables = vars)
env.Append(CCFLAGS = '-std=c++0x')
Help(vars.GenerateHelpText(env))

mode = ARGUMENTS.get('mode', 'debug')
if (mode == 'release'):
  # -DV8H_NDEBUG
  env.Append(CCFLAGS = '-O3 -DNDEBUG')
else:
  env.Append(CCFLAGS = '-O0 -g')

v8path = ARGUMENTS.get('v8path')
if (v8path != ''):
  env.Append(LIBPATH = v8path)
  env.Append(CPPPATH = v8path)

env.Append(LIBS = ['v8', 'pthread'])
env.Append(CCFLAGS = '-fno-exceptions -Wall')

env.Program('bin/v8h', [
  "src/v8h.cc",
  "src/buffer.cc",
  "src/algorithm.cc",
	"src/assert.cc",
	"src/file.cc",
  "src/system.cc",
  "src/service.cc",
  "src/socket.cc"
])

