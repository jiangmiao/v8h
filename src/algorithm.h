#ifndef V8H_ALGORITHM_H
#define V8H_ALGORITHM_H

#include <string.h>

namespace v8h
{
	template <class T> 
	inline const T& min ( const T& a, const T& b ) 
	{
		return (a<b)?a:b;
	}

	int partial_find(const char *haystack_start, size_t haystack_size, const char *needle, size_t needle_size);

	/**
	 * Convert underline variable to lower camel
	 */
	const char* underline_to_camel(const char *src);

}

#endif
