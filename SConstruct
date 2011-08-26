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

v8h_obj = env.Object('obj/v8h.o', "src/v8h.cc")
env.Program('bin/v8h', v8h_obj)

