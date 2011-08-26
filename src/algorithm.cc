#include "algorithm.h"

namespace v8h
{
	int partial_find(const char *haystack_start, size_t haystack_size, const char *needle, size_t needle_size)
	{
		const char *haystack     = haystack_start;
		const char *haystack_end = haystack + haystack_size;
		const char *needle_end   = needle + needle_size;
		const char *first1       = NULL;
		const char *first2       = NULL;

		while (haystack != haystack_end) {
			first1 = haystack;
			first2 = needle;
			while (*first1 == *first2) {
				++first1;
				++first2;
				if (first1 == haystack_end || first2 == needle_end) {
					return haystack - haystack_start;
				}
			}
			++haystack;
		}
		return haystack_size;
	}

	/**
	 * Convert underline variable to lower camel
	 */
	const char* underline_to_camel(const char *src)
	{
		static char js_name[512];
		char *dest = js_name;
		char *last = dest + sizeof(js_name) - 1;
		bool is_underline = false;
		while (char c = *src) {
			if ( c == '_') {
				*dest++ = '_';
				++src;
				continue;
			} else {
				break;
			}
		}
		char c;
		while ( (c = *src++) && dest<last ) {
			if (c == '_') {
				is_underline = true;
			} else {
				if (is_underline && c >= 'a' && c <='z') {
					c = c + 'A' - 'a';
				}
				*dest++ = c;
				is_underline = false;
			}
		}
		*dest++ = 0;
		return js_name;
	}

}
