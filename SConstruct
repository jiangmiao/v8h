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
  env.Append(CCFLAGS = '-O3 -DNDEBUG -DV8H_NDEBUG')
else:
  env.Append(CCFLAGS = '-O0 -g')

v8path = ARGUMENTS.get('v8path')
if (v8path != ''):
  env.Append(LIBPATH = v8path)
  env.Append(CPPPATH = v8path)

env.Append(LIBS = ['v8', 'pthread'])
env.Append(CCFLAGS = '-fno-exceptions -Wall')

env.VariantDir('build', 'src')
env.Program('v8h', [
  "build/v8h.cc",
  "build/buffer.cc",
  "build/algorithm.cc",
  "build/assert.cc",
  "build/file.cc",
  "build/system.cc",
  "build/service.cc",
  "build/socket.cc"
])

