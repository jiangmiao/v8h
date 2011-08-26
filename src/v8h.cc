#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <locale.h>
#include <libgen.h>


#include "../include/v8h/all.h"

using namespace std;
using namespace v8;
using namespace v8h;

char *get_bin_dir()
{
	static char bin_dir[PATH_MAX];
	char proc_path[PATH_MAX];
	char bin_path[PATH_MAX];
	snprintf(proc_path, sizeof(proc_path), "/proc/%d/exe", getpid());
	readlink(proc_path, bin_path, sizeof(bin_path));
	strcpy(bin_dir, dirname(bin_path));
	return bin_dir;
}

char *get_module_file(char *dir, char *dir_end, const char *filename)
{
	static char path[PATH_MAX];
	strcpy(dir_end, filename);
	realpath(dir, path);
	return path;
}

char *realpath(const char *filename)
{
	static char path[PATH_MAX];
	realpath(filename, path);
	return path;
}


int main(int argc, char *argv[])
{
	V8::SetFlagsFromCommandLine(&argc, argv, true);
	if (argc == 1) {
		puts("v8h [v8-options] script name");
		return 1;
	}
	setlocale(LC_ALL, "");
	HandleScope scope;
	auto context = Context::New();
	Context::Scope context_scope(context);

	auto global = context->Global();
	v8h::System::init(global);

	auto buffer     = v8h::Buffer::create();
	auto tempBuffer = v8h::Buffer::global();
	auto c = Object::New();
	v8h::C::init(c);

	auto includePath = Array::New();


	char *path = (char*)malloc(PATH_MAX);
	char *dir = get_bin_dir();
	char *dir_end = dir + strlen(dir);
	strcpy(dir_end, "/../js");
	if (realpath(dir, path)) {
		SET(includePath, includePath->Length(), String::New(path));
	}

	getcwd(path, PATH_MAX);
	SET(global , "$workingDir"  , String::New(path));
	free(path);

	auto v8_argv = Array::New();
	for (int i=0; i<argc; ++i) {
		SET(v8_argv, i, String::New(argv[i]));
	}

	auto config = v8h::Console::global();
	SET(global , "global"     , global);
	SET(global , "$includePath", includePath);
	SET(global , "c"          , c);
	SET(global , "$argv"       , v8_argv);
	SET(global , "$config"    , config);
	SET(global , "service"    , v8h::Service::global());
	SET(global , "console"    , v8h::Console::global());
	SET(global , "Socket"     , v8h::Socket::create());
	SET(global , "File"       , v8h::File::create());
	SET(global , "Buffer"     , buffer);
	SET(buffer , "temp"       , tempBuffer);
	SET(global , "tempBuffer" , tempBuffer);


	TryCatch trycatch;
	auto result = System::require(get_module_file(dir, dir_end, "/../js/core/startup.js"));
	if (trycatch.HasCaught()) {
		auto stack_trace = trycatch.StackTrace();
		String::Utf8Value u(stack_trace);
		puts(*u);
		return 0;
	}
	return 0;
}

