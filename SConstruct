import os
import SCons.Util

vars = Variables()
vars.Add('mode', 'compilation mode (debug, release)', 'release')
env = Environment(variables = vars)
Help(vars.GenerateHelpText(env))

mode = ARGUMENTS.get('mode', 'release')
env.Append(CCFLAGS = '-std=c++0x')
if (mode == 'release'):
  env.Append(CCFLAGS = '-O3 -DNDEBUG')
else:
  env.Append(CCFLAGS = '-O0 -g')

env.Append(LIBS = ['v8', 'pthread'])

env.Program('bin/v8h', [
  "src/v8h.cc",
  "src/buffer.cc",
  "src/algorithm.cc",
  "src/service.cc",
  "src/socket.cc",
  "src/system.cc",
  "src/error.cc",
  "src/console.cc",
  "src/config.cc",
  "src/file.cc"
])

