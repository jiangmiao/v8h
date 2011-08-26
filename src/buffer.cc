#include "buffer.h"

namespace v8h 
{
	void Buffer::realloc(void *ptr, size_t size)
	{
		V8::AdjustAmountOfExternalAllocatedMemory(size-reserve_);
		data_ = (char*)::realloc(ptr, size);
		reserve_ = size;
	}
	Buffer::Buffer(size_t reserve)
	: cursor_(0)
	, size_(0)
	, used_(0)
	, reserve_(reserve)
	{
		data_ = (char*)malloc(reserve);
		V8::AdjustAmountOfExternalAllocatedMemory(sizeof(Buffer) + V8H_MEMORY_FIXED_SIZE + reserve_);
	}

	Buffer::~Buffer()
	{
		V8::AdjustAmountOfExternalAllocatedMemory(- sizeof(Buffer) - V8H_MEMORY_FIXED_SIZE - reserve_ );
		free(data_);
		instance.Dispose();
	}

	void Buffer::zero()
	{
		prepare(1);
		*(data_+used_) = 0;
	}

	void Buffer::write(const void *data, size_t n)
	{
		prepare(n);
		memcpy(data_ + used_, data, n);
		commit(n);
	}

	void Buffer::write(const char *data)
	{
		write(data, strlen(data));
	}

	void Buffer::write(Handle<Value> data)
	{
		auto text        = data->ToString();
		auto utf8_length = text->Utf8Length();
		auto n           = text->WriteUtf8(prepare(utf8_length), utf8_length);
		commit(n);
	}

	void Buffer::write(Buffer *buffer, size_t n)
	{
		if (n > buffer->size()) {
			n = buffer->size();
		}
		write(buffer->data(), n);
	}

	char *Buffer::data()
	{
		return data_ + cursor_;
	}

	size_t Buffer::size()
	{
		return size_;
	}

	void Buffer::consume(size_t n)
	{
		if (size_ < n)
			n = size_;
		cursor_ += n;
		size_   -= n;
		if (size_ == 0) {
			clear();
		}
	}
	char *Buffer::prepare(size_t n)
	{
		size_t reserve = reserve_;
		while (used_ + n > reserve) {
			reserve *= 3;
		}
		if (reserve > reserve_)
			realloc(data_, reserve);
		return data_ + used_;
	}

	void Buffer::commit(size_t n)
	{
		size_ += n;
		used_ += n;
	}

	void Buffer::clear()
	{
		cursor_ = 0;
		size_   = 0;
		used_   = 0;
	}

	void Buffer::sweep()
	{
		memmove(data_, data_+cursor_, size_);
		cursor_ = 0;
		used_   = size_;
	}

	size_t Buffer::remain()
	{
		return reserve_ - used_;
	}

	void Buffer::show()
	{
		fwrite(data(), 1, size(), stdout);
		fflush(stdout);
	}

	size_t Buffer::reserve()
	{
		return reserve_;
	}

	void Buffer::write_int32(int32_t data)
	{
		write(&data, sizeof(data));
	}
	void Buffer::write_uint32(uint32_t data)
	{
		write(&data, sizeof(data));
	}
	void Buffer::write_int64(int64_t data)
	{
		write(&data, sizeof(data));
	}
	void Buffer::write_double(double data)
	{
		write(&data, sizeof(data));
	}


	void Buffer::read(void *dest, size_t size)
	{
		size = min(size, size_);
		memcpy(dest, data(), size);
		commit(size);
	}

	double Buffer::read_double()
	{
		double data;
		read(&data, sizeof(data));
		return data;
	}

	uint32_t Buffer::read_uint32()
	{
		uint32_t data;
		read(&data, sizeof(data));
		return data;
	}

	int32_t Buffer::read_int32()
	{
		int32_t data;
		read(&data, sizeof(data));
		return data;
	}

	int64_t Buffer::read_int64()
	{
		int64_t data;
		read(&data, sizeof(data));
		return data;
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
	int Buffer::partial_find(Buffer *needle, size_t offset)
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


	V8H_FUNCTION(Buffer::_peek_buffer)
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
	V8H_FUNCTION(Buffer::peek_utf8)
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
	V8H_FUNCTION(Buffer::_read_buffer)
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
	V8H_FUNCTION(Buffer::read_utf8)
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
	V8H_FUNCTION(Buffer::write_utf8)
	{
		auto self        = args.This();
		auto buffer      = get_internal(self);
		buffer->write(args[0]);
		return self;
	}

	V8H_FUNCTION(Buffer::write_ucs2)
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
	V8H_FUNCTION(Buffer::write_ascii)
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
	V8H_FUNCTION(Buffer::write_buffer)
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
	V8H_FUNCTION(Buffer::close)
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
	V8H_FUNCTION(Buffer::partial_find_buffer)
	{
		auto self          = args.This();
		auto needle        = TO_OBJ(args[0]);
		auto offset        = TO_UINT(args[1]);
		auto self_buffer   = get_internal(self);
		auto needle_buffer = get_internal(needle);
		auto pos           = self_buffer->partial_find(needle_buffer, offset);
		V8H_RETURN_INT(pos);
	}


	V8H_FUNCTION(Buffer::constructor)
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

	Handle<Function> Buffer::create()
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

	V8H_FORWARD_FUNCTION_WITH_CLASS_1_0(Buffer , commit       , TO_UINT);
	V8H_FORWARD_FUNCTION_WITH_CLASS_1_0(Buffer , consume      , TO_UINT);
	V8H_FORWARD_FUNCTION_WITH_CLASS_0_1(Buffer , size         , V8_UINT);
	V8H_FORWARD_FUNCTION_WITH_CLASS_0_1(Buffer , reserve      , V8_UINT);
	V8H_FORWARD_FUNCTION_WITH_CLASS_0_1(Buffer , remain       , V8_UINT);
	V8H_FORWARD_FUNCTION_WITH_CLASS_1_1(Buffer , prepare      , TO_UINT     , V8_PTR);
	V8H_FORWARD_FUNCTION_WITH_CLASS_0_1(Buffer , data         , V8_PTR);
	V8H_FORWARD_FUNCTION_WITH_CLASS_0_0(Buffer , show);
	V8H_FORWARD_FUNCTION_WITH_CLASS_0_0(Buffer , clear);
	V8H_FORWARD_FUNCTION_WITH_CLASS_0_0(Buffer , zero);
	V8H_FORWARD_FUNCTION_WITH_CLASS_1_0(Buffer , write_uint32 , TO_UINT32);
	V8H_FORWARD_FUNCTION_WITH_CLASS_1_0(Buffer , write_int32  , TO_INT32);
	V8H_FORWARD_FUNCTION_WITH_CLASS_1_0(Buffer , write_int64  , TO_INT64);
	V8H_FORWARD_FUNCTION_WITH_CLASS_1_0(Buffer , write_double , TO_DOUBLE);
	V8H_FORWARD_FUNCTION_WITH_CLASS_0_1(Buffer , read_int32   , V8_INT32);
	V8H_FORWARD_FUNCTION_WITH_CLASS_0_1(Buffer , read_uint32  , V8_UINT32);
	V8H_FORWARD_FUNCTION_WITH_CLASS_0_1(Buffer , read_int64   , V8_INT64);
	V8H_FORWARD_FUNCTION_WITH_CLASS_0_1(Buffer , read_double  , V8_DOUBLE);

	V8H_IMPORT_GET_INTERNAL_WITH_CLASS(Buffer)
	V8H_IMPORT_DESTRUCTOR_WITH_CLASS(Buffer);
	V8H_IMPORT_NEW_INSTANCE_WITH_CLASS(Buffer);
	V8H_IMPORT_GLOBAL_WITH_CLASS(Buffer);
}
