#ifndef V8H_C_H
#define V8H_C_H

#include <limits.h>
#include "v8h.h"
#include "error.h"
#include "buffer.h"
namespace v8h
{
	class C
	{
	    public:
		static V8H_C_FUNCTION_1_1(malloc   , TO_INT          , V8_PTR_OR_NULL);
		static V8H_C_FUNCTION_1_0(free     , TO_PTR<void*>);
		static V8H_C_FUNCTION_1_0(perror   , TO_PCHAR);

		static V8H_C_FUNCTION_2_1(access   , TO_PCHAR        , TO_INT   , V8_INT);
		static V8H_C_FUNCTION_4_1(fread    , TO_PCHAR        , TO_UINT  , TO_UINT  , TO_PTR<FILE*> , V8_INT);
		static V8H_C_FUNCTION_4_1(fwrite   , TO_PCHAR        , TO_UINT  , TO_UINT  , TO_PTR<FILE*> , V8_INT);
		static V8H_C_FUNCTION_1_1(feof     , TO_PTR<FILE*>   , V8_INT);
		static V8H_C_FUNCTION_1_1(ferror   , TO_PTR<FILE*>   , V8_INT);
		static V8H_C_FUNCTION_1_1(fclose   , TO_PTR<FILE*>   , V8_INT);
		static V8H_C_FUNCTION_2_1(fopen    , TO_PCHAR        , TO_PCHAR , V8_PTR_OR_NULL);
		static V8H_C_FUNCTION_1_1(puts     , TO_PCHAR        , V8_INT);
		static V8H_C_FUNCTION_2_1(realpath , TO_PCHAR        , TO_PCHAR , V8_PTR_OR_NULL);
		static V8H_C_FUNCTION_1_1(strerror , TO_INT          , V8_PTR_OR_NULL);

		static V8H_FUNCTION(string)
		{
			char *ptr = TO_PTR<char*>(args[0]);
			if (args.Length() > 1) {
				return String::New(ptr, TO_INT(args[1]));
			} else {
				return String::New(ptr);
			}
		}

		static V8H_GETTER(get_errno)
		{
			return Integer::New(errno);
		}

		static V8H_FUNCTION(isNull)
		{
			return Boolean::New(TO_PTR<void*>(args[0]) == NULL);
		}

		static Handle<Object> init(Handle<Object> c)
		{
			SET_ACC(c, "errno", get_errno);
			V8H_STATIC_IMPLEMENT(strerror);

			V8H_STATIC_IMPLEMENT(malloc);
			V8H_STATIC_IMPLEMENT(free);
			V8H_STATIC_IMPLEMENT(perror);

			V8H_STATIC_IMPLEMENT(isNull);

			V8H_STATIC_IMPLEMENT(access);
			V8H_STATIC_IMPLEMENT(realpath);
			V8H_STATIC_IMPLEMENT(fread);
			V8H_STATIC_IMPLEMENT(fwrite);
			V8H_STATIC_IMPLEMENT(fread);
			V8H_STATIC_IMPLEMENT(fopen);
			V8H_STATIC_IMPLEMENT(fclose);
			V8H_STATIC_IMPLEMENT(feof);
			V8H_STATIC_IMPLEMENT(ferror);

			V8H_STATIC_IMPLEMENT(puts);

			V8H_STATIC_IMPLEMENT(string);

			V8H_CONSTANT_INT(W_OK);
			V8H_CONSTANT_INT(R_OK);
			V8H_CONSTANT_INT(X_OK);
			V8H_CONSTANT_INT(F_OK);
			V8H_CONSTANT_INT(PATH_MAX);

			V8H_CONSTANT_PTR(stdout);
			V8H_CONSTANT_PTR(stdin);
			V8H_CONSTANT_PTR(stderr);
			return c;
		}
	};

}

#endif
