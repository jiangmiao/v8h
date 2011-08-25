#ifndef V8H_BUFFER_H
#define V8H_BUFFER_H

namespace v8h 
{
	class Buffer
	{
	    protected:
		char   *data_;
		size_t cursor_;
		size_t reserve_;
		size_t used_;
		size_t size_;
		bool   binded_;

		void realloc(void *ptr, size_t size)
		{
			V8::AdjustAmountOfExternalAllocatedMemory(size-reserve_);
			data_ = (char*)::realloc(ptr, size);
			reserve_ = size;
		}
	    public:
		Persistent<Object> instance;
		Buffer(size_t reserve = 1024)
		: cursor_(0)
		, size_(0)
		, used_(0)
		, binded_(false)
		, reserve_(reserve)
		{
			data_ = (char*)malloc(reserve);
			V8::AdjustAmountOfExternalAllocatedMemory(sizeof(Buffer) + V8H_MEMORY_FIXED_SIZE + reserve_);
		}

		~Buffer()
		{
			V8::AdjustAmountOfExternalAllocatedMemory(- sizeof(Buffer) - V8H_MEMORY_FIXED_SIZE - reserve_ );
			free(data_);
			instance.Dispose();
		}

		void zero()
		{
			prepare(1);
			*(data_+used_) = 0;
		}

		void write(const void *data, size_t n)
		{
			prepare(n);
			memcpy(data_ + used_, data, n);
			commit(n);
		}

		void write(const char *data)
		{
			write(data, strlen(data));
		}

		void write(Handle<Value> data)
		{
			auto text        = data->ToString();
			auto utf8_length = text->Utf8Length();
			auto n           = text->WriteUtf8(prepare(utf8_length), utf8_length);
			commit(n);
		}

		void write(Buffer *buffer, size_t n = (size_t)-1)
		{
			if (n > buffer->size()) {
				n = buffer->size();
			}
			write(buffer->data(), n);
		}

		char *data()
		{
			return data_ + cursor_;
		}

		size_t size() const
		{
			return size_;
		}

		void consume(size_t n)
		{
			if (size_ < n)
				n = size_;
			cursor_ += n;
			size_   -= n;
			if (size_ == 0) {
				clear();
			}
		}
		char *prepare(size_t n)
		{
			size_t reserve = reserve_;
			while (used_ + n > reserve) {
				reserve *= 3;
			}
			if (reserve > reserve_)
				realloc(data_, reserve);
			return data_ + used_;
		}

		void commit(size_t n)
		{
			size_ += n;
			used_ += n;
		}

		void clear()
		{
			cursor_ = 0;
			size_   = 0;
			used_   = 0;
		}

		void sweep()
		{
			memmove(data_, data_+cursor_, size_);
			cursor_ = 0;
			used_   = size_;
		}

		size_t remain()
		{
			return reserve_ - used_;
		}

		void show()
		{
			fwrite(data(), 1, size(), stdout);
			fflush(stdout);
		}

		size_t reserve()
		{
			return reserve_;
		}

		void write_int32(int32_t data)
		{
			write(&data, sizeof(data));
		}
		void write_uint32(uint32_t data)
		{
			write(&data, sizeof(data));
		}
		void write_int64(int64_t data)
		{
			write(&data, sizeof(data));
		}
		void write_double(double data)
		{
			write(&data, sizeof(data));
		}


		void read(void *dest, size_t size)
		{
			size = min(size, size_);
			memcpy(dest, data(), size);
			commit(size);
		}

		template<typename T>
		T read()
		{
			T data;
			read(&data, sizeof(data));
			return data;
		}
		double read_double()
		{
			return read<double>();
		}

		uint32_t read_uint32()
		{
			return read<uint32_t>();
		}

		int32_t read_int32()
		{
			return read<int32_t>();
		}

		int64_t read_int64()
		{
			return read<int64_t>();
		}

		/**
		 * Find dest buffer in the buffer.
		 * 
		 * \param target_buffer
		 * \param offset 
		 * \return the new offset of the buffer
		 * new offset + target_buffer->size() <= buffer->size() : the pattern is met.
		 * else find the string with new offset
		 */
		int partial_find(Buffer *needle, size_t offset)
		{
			if (needle->size() == 0) {
				return offset;
			}

			if (offset >= size_) {
				return size_;
			}

			int pos =  v8h::partial_find(data() + offset, size_ - offset, needle->data(), needle->size());
			return pos + offset;
		}

		typedef Buffer* InternalPointer;
	    public:
		enum {
			FIELD_INTERNAL,
			FIELDS_NUMBER
		};

		static V8H_IMPORT_GET_INTERNAL()
		static V8H_FORWARD_FUNCTION_1(commit    , TO_UINT);
		static V8H_FORWARD_FUNCTION_1(consume   , TO_UINT);
		static V8H_FORWARD_FUNCTION_0_1(size    , V8_UINT);
		static V8H_FORWARD_FUNCTION_0_1(reserve , V8_UINT);
		static V8H_FORWARD_FUNCTION_0_1(remain  , V8_UINT);
		static V8H_FORWARD_FUNCTION_1_1(prepare , TO_UINT   , V8_PTR);
		static V8H_FORWARD_FUNCTION_0_1(data    , V8_PTR);
		static V8H_FORWARD_FUNCTION_0(show);
		static V8H_FORWARD_FUNCTION_0(clear);
		static V8H_FORWARD_FUNCTION_0(zero);

		static V8H_FORWARD_FUNCTION_1(write_uint32 , TO_UINT32);
		static V8H_FORWARD_FUNCTION_1(write_int32  , TO_INT32);
		static V8H_FORWARD_FUNCTION_1(write_int64  , TO_INT64);
		static V8H_FORWARD_FUNCTION_1(write_double , TO_DOUBLE);

		static V8H_FORWARD_FUNCTION_0_1(read_int32  , V8_INT32);
		static V8H_FORWARD_FUNCTION_0_1(read_uint32 , V8_UINT32);
		static V8H_FORWARD_FUNCTION_0_1(read_int64  , V8_INT64);
		static V8H_FORWARD_FUNCTION_0_1(read_double , V8_DOUBLE);

		static V8H_FUNCTION(_peek_buffer)
		{
			auto self   = args.This();
			auto buffer = get_internal(self);
			auto dest   = get_internal(args[0]->ToObject());

			size_t n = buffer->size();
			if (args.Length() > 1)
				n = TO_UINT(args[1]);
			dest->write(buffer->data(), n);
			return self;
			
		}

		/**
		 * read and don't remove the data from the buffer
		 *
		 * \param number of bytes will be read default is the whole buffer
		 *
		 * \return the utf8 string
		 */
		static V8H_FUNCTION(peek_utf8)
		{
			auto self = args.This();
			size_t n;
			auto buffer = get_internal(self);
			if (args.Length() > 0)
				n = TO_UINT(args[0]);
			else
				n = buffer->size();
			if (n > buffer->size())
				n = buffer->size();
			Handle<String> rt = String::New(buffer->data(), n);
			return rt;
		}


		/**
		 * read data to another buffer
		 *
		 * \param dest buffer
		 * \param size
		 */
		static V8H_FUNCTION(_read_buffer)
		{
			auto self   = args.This();
			auto buffer = get_internal(self);
			auto dest   = get_internal(args[0]->ToObject());

			size_t n = buffer->size();
			if (args.Length() > 1)
				n = TO_UINT(args[1]);
			dest->write(buffer->data(), n);
			buffer->commit(n);
			return self;
		}

		/**
		 * read the data and remove from the buffer
		 * 
		 * \param number of the bytes will be read, default is the whole buffer
		 *
		 * \return the utf8 string
		 */
		static V8H_FUNCTION(read_utf8)
		{
			auto self = args.This();
			size_t n;
			auto buffer = get_internal(self);
			if (args.Length() > 0)
				n = TO_UINT(args[0]);
			else
				n = buffer->size();
			if (n > buffer->size())
				n = buffer->size();
			Handle<String> rt = String::New(buffer->data(), n);
			buffer->consume(n);
			return rt;
		}

		/**
		 * Write UTF8 string
		 *
		 * \param the utf8 string
		 * \return self
		 */
		static V8H_FUNCTION(write_utf8)
		{
			auto self        = args.This();
			auto buffer      = get_internal(self);
			buffer->write(args[0]);
			return self;
		}

		static V8H_FUNCTION(write_ucs2)
		{
			auto self   = args.This();
			auto text   = args[0]->ToString();
			auto buffer = get_internal(self);
			auto length = text->Length()*2;
			auto n      = text->Write((uint16_t*)buffer->prepare(length), 0, -1);
			buffer->commit(n);
			return self;
		}

		/**
		 * Write ASCII string
		 *
		 * \param ascii string
		 * \return self
		 */
		static V8H_FUNCTION(write_ascii)
		{
			auto self   = args.This();
			auto text   = args[0]->ToString();
			auto buffer = get_internal(self);
			auto length = text->Length();
			auto n      = text->WriteAscii(buffer->prepare(length), 0, length);
			buffer->commit(n);
			return self;
		}

		/**
		 * Write Buffer
		 *
		 * \param buffer
		 * \return self
		 */
		static V8H_FUNCTION(write_buffer)
		{
			auto self = args.This();
			auto dest = get_internal(self);
			auto src  = get_internal(args[0]->ToObject());
			auto n    = (size_t) -1;
			auto size = src->size();
			if (n>size) {
				n = size;
			}
			dest->write(src, n);
			return self;
		}

		/**
		 * delete the buffer
		 */
		static V8H_FUNCTION(close)
		{
			auto self = args.This();
			DEL_PTR<InternalPointer>(self, FIELD_INTERNAL);
			return self;
		}

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
		static V8H_FUNCTION(partial_find_buffer)
		{
			auto self          = args.This();
			auto needle        = TO_OBJ(args[0]);
			auto offset        = TO_UINT(args[1]);
			auto self_buffer   = get_internal(self);
			auto needle_buffer = get_internal(needle);
			auto pos           = self_buffer->partial_find(needle_buffer, offset);
			V8H_RETURN_INT(pos);
		}


		static V8H_IMPORT_DESTRUCTOR_FUNCTION();
		static V8H_FUNCTION(constructor)
		{
			auto self = args.This();
			size_t reserve = 1024;
			if (args.Length() > 0 && args[0]->IsUint32()) {
				reserve = TO_UINT(args[0]);
			}
			auto buffer = new Buffer(reserve);
			SET_PTR(self, FIELD_INTERNAL, buffer);
			if (args.Length() > 0 && args[0]->IsString()) {
				write_utf8(args);
			}
			V8H_AUTO_DELETE(buffer);
			return self;
		}

		static Handle<Function> create()
		{
			V8H_CREATE_START_WITH_INTERNAL_FIELD();

			V8H_IMPLEMENT(prepare);
			V8H_IMPLEMENT(data);
			V8H_IMPLEMENT(zero);

			V8H_IMPLEMENT(write_buffer);
			V8H_IMPLEMENT(write_utf8);
			V8H_IMPLEMENT(write_ucs2);
			V8H_IMPLEMENT(write_ascii);

			V8H_IMPLEMENT(write_uint32);
			V8H_IMPLEMENT(write_int32);
			V8H_IMPLEMENT(write_double);

			V8H_IMPLEMENT(peek_utf8);
			V8H_IMPLEMENT(_peek_buffer);

			V8H_IMPLEMENT(read_utf8);
			V8H_IMPLEMENT(_read_buffer);

			V8H_IMPLEMENT(read_int32);
			V8H_IMPLEMENT(read_uint32);
			V8H_IMPLEMENT(read_int64);
			V8H_IMPLEMENT(read_double);

			V8H_IMPLEMENT(consume);
			V8H_IMPLEMENT(commit);
			V8H_IMPLEMENT(close);
			V8H_IMPLEMENT(size);
			V8H_IMPLEMENT(reserve);
			V8H_IMPLEMENT(remain);
			V8H_IMPLEMENT(clear);
			V8H_IMPLEMENT(show);
			V8H_IMPLEMENT(partial_find_buffer);

			V8H_CREATE_END();
		}

		static V8H_IMPORT_NEW_INSTANCE();
		static V8H_IMPORT_GLOBAL();
	};
}

#endif
