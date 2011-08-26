#ifndef V8H_BUFFER_H
#define V8H_BUFFER_H

#include <v8.h>
#include "v8h.h"
namespace v8h 
{
	using namespace v8;
	class Buffer
	{
	    protected:
		char   *data_;
		size_t cursor_;
		size_t size_;
		size_t used_;
		size_t reserve_;
		void realloc(void *ptr, size_t size);
	    public:
		Persistent<Object> instance;
		Buffer(size_t reserve = 1024);
		~Buffer();
		char *data();

		void consume(size_t n);
		char *prepare(size_t n);
		void commit(size_t n);

		size_t size();
		size_t remain();
		size_t reserve();

		void zero();
		void clear();
		void sweep();

		void show();

		void write(const void *data, size_t n);
		void write(const char *data);
		void write(Handle<Value> data);
		void write(Buffer *buffer, size_t n = (size_t)-1);
		void write_int32(int32_t data);
		void write_uint32(uint32_t data);
		void write_int64(int64_t data);
		void write_double(double data);

		void read(void *dest, size_t size);
		double read_double();
		uint32_t read_uint32();
		int32_t read_int32();
		int64_t read_int64();
		/**
		 * Find dest buffer in the buffer.
		 * 
		 * \param target_buffer
		 * \param offset 
		 * \return the new offset of the buffer
		 * new offset + target_buffer->size() <= buffer->size() : the pattern is met.
		 * else find the string with new offset
		 */
		int partial_find(Buffer *needle, size_t offset);
		typedef Buffer* InternalPointer;
	    public:
		enum {
			FIELD_INTERNAL,
			FIELDS_NUMBER
		};
		static V8H_FUNCTION(commit);
		static V8H_FUNCTION(consume);
		static V8H_FUNCTION(size);
		static V8H_FUNCTION(reserve);
		static V8H_FUNCTION(remain);
		static V8H_FUNCTION(prepare);
		static V8H_FUNCTION(data);
		static V8H_FUNCTION(show);
		static V8H_FUNCTION(clear);
		static V8H_FUNCTION(zero);

		static V8H_FUNCTION(write_uint32);
		static V8H_FUNCTION(write_int32);
		static V8H_FUNCTION(write_int64);
		static V8H_FUNCTION(write_double);

		static V8H_FUNCTION(read_int32);
		static V8H_FUNCTION(read_uint32);
		static V8H_FUNCTION(read_int64);
		static V8H_FUNCTION(read_double);

		static V8H_FUNCTION(_peek_buffer);
		/**
		 * read and don't remove the data from the buffer
		 *
		 * \param number of bytes will be read default is the whole buffer
		 *
		 * \return the utf8 string
		 */
		static V8H_FUNCTION(peek_utf8);
		/**
		 * read data to another buffer
		 *
		 * \param dest buffer
		 * \param size
		 */
		static V8H_FUNCTION(_read_buffer);
		/**
		 * read the data and remove from the buffer
		 * 
		 * \param number of the bytes will be read, default is the whole buffer
		 * \return the utf8 string
		 */
		static V8H_FUNCTION(read_utf8);
		/**
		 * Write UTF8 string
		 *
		 * \param the utf8 string
		 * \return self
		 */
		static V8H_FUNCTION(write_utf8);
		/**
		 * Write UCS2 string
		 * \param the ucs2 string
		 * \return self
		 */
		static V8H_FUNCTION(write_ucs2);
		/**
		 * Write ASCII string
		 *
		 * \param ascii string
		 * \return self
		 */
		static V8H_FUNCTION(write_ascii);
		/**
		 * Write Buffer
		 *
		 * \param buffer
		 * \return self
		 */
		static V8H_FUNCTION(write_buffer);
		/**
		 * delete the buffer
		 */

		static V8H_FUNCTION(close);
		/**
		 * partial find buffer
		 * 
		 * \param the dest buffer will be matched
		 * \param the start offset (from 0) to current buffer
		 * 
		 * \return the position of partial find
		 *   found     :pos + dest.size() <= buffer.size()
		 *   not found :pos + dest.size() > buffer.size()
		 */
		static V8H_FUNCTION(partial_find_buffer);
		static V8H_FUNCTION(constructor);
		static Handle<Function> create();

		static V8H_DECLARE_GET_INTERNAL();
		static V8H_DECLARE_DESTRUCTOR();
		static V8H_DECLARE_NEW_INSTANCE();
		static V8H_DECLARE_GLOBAL();
	};
}
#endif
