#include "buffer.h"
#include "algorithm.h"

V8H_NS_START
Buffer::Buffer(uint32_t reserve)
: memory_(0)
, cursor_(0)
, size_(0)
{
	if (reserve == 0)
		reserve = 1;
	reserve_ = reserve;
	memory_ = (char*)malloc(reserve_);
	v8::V8::AdjustAmountOfExternalAllocatedMemory(V8H_MEMORY_FIXED_SIZE(Buffer) + reserve_);
}

Buffer::~Buffer()
{
	v8::V8::AdjustAmountOfExternalAllocatedMemory(- V8H_MEMORY_FIXED_SIZE(Buffer) - reserve_ );
	free(memory_);
}

// Data Controller
// ===============
char * Buffer::data()
{
	return memory_ + cursor_;
}

void Buffer::zero()
{
	prepare(1);
	*(memory_+used()) = 0;
}

void Buffer::clear()
{
	cursor_ = 0;
	size_   = 0;
}

char * Buffer::prepare(uint32_t n)
{
	if (n == 0) 
		n = 1;
	uint32_t reserve = reserve_;
	while (used() + n > reserve) {
		reserve *= 3;
	}

	if (reserve > reserve_) {
		v8::V8::AdjustAmountOfExternalAllocatedMemory(reserve-reserve_);
		memory_ = (char*)::realloc(memory_, reserve);
		reserve_ = reserve;
	}
	return memory_ + used();
}

void Buffer::commit(uint32_t n)
{
	size_ += n;
}

void Buffer::consume(uint32_t n)
{
	if (size_ < n)
		n = size_;
	cursor_ += n;
	size_   -= n;
	if (size_ == 0) {
		clear();
	}
}


// Size Getter
// ==========
uint32_t Buffer::size() const
{
	return size_;
}

uint32_t Buffer::remain() const
{
	return reserve_ - used();
}

uint32_t Buffer::used() const
{
	return cursor_ + size_;
}

uint32_t Buffer::reserve() const
{
	return reserve_;
}


// Writer
// ======
void Buffer::write(const void *data, uint32_t n) // }}}
{
	prepare(n);
	memcpy(memory_ + used(), data, n);
	commit(n);
}

void Buffer::write(const char *data)
{
	write(data, strlen(data));
}

void Buffer::write(Buffer *buffer, uint32_t n)
{
	write(buffer->data(), min(n, buffer->size()));
}

void Buffer::write(v8::Handle<v8::Value> data)
{
	v8::String::Utf8Value utf8(data);
	write(*utf8, utf8.length());
}

void Buffer::writeInt32(int32_t data)
{
	write(&data, sizeof(data));
}

void Buffer::writeUint32(uint32_t data)
{
	write(&data, sizeof(data));
}

void Buffer::writeInt64(int64_t data)
{
	write(&data, sizeof(data));
}

void Buffer::writeDouble(double data)
{
	write(&data, sizeof(data));
}


// Peeker
// ======
void Buffer::peek(void *dest, uint32_t size)
{
	memcpy(dest, data(), min(size, size_));
}

void Buffer::peekBuffer(Buffer *buffer, uint32_t size)
{
	size = min(size, size_);
	peek(buffer->prepare(size), size);
	buffer->commit(size);
}

int32_t Buffer::peekInt32()
{
	int32_t data;
	peek(&data, sizeof(data));
	return data;
}

uint32_t Buffer::peekUint32()
{
	uint32_t data;
	peek(&data, sizeof(data));
	return data;
}

int64_t Buffer::peekInt64()
{
	int64_t data;
	peek(&data, sizeof(data));
	return data;
}

double Buffer::peekDouble()
{
	double data;
	peek(&data, sizeof(data));
	return data;
}


// Reader
// ======
void Buffer::read(void *dest, uint32_t size)
{
	size = min(size, size_);
	memcpy(dest, data(), size);
	consume(size);
}

void Buffer::readBuffer(Buffer *buffer, uint32_t size)
{
	size = min(size, size_);
	read(buffer->prepare(size), size);
	buffer->commit(size);
}

int32_t Buffer::readInt32()
{
	int32_t data;
	read(&data, sizeof(data));
	return data;
}

uint32_t Buffer::readUint32()
{
	uint32_t data;
	read(&data, sizeof(data));
	return data;
}

int64_t Buffer::readInt64()
{
	int64_t data;
	read(&data, sizeof(data));
	return data;
}

double Buffer::readDouble()
{
	double data;
	read(&data, sizeof(data));
	return data;
}


// Utils
// =====
uint32_t Buffer::partialFind(Buffer *needle, uint32_t offset)
{
	if (needle->size() == 0) {
		return offset;
	}

	if (offset >= size_) {
		return size_;
	}

	uint32_t pos =  v8h::partial_find(data() + offset, size_ - offset, needle->data(), needle->size());
	return pos + offset;
}

void Buffer::show()
{
	fwrite(data(), 1, size(), stdout);
	fflush(stdout);
}

// Javascript Implements
// =====================

// Data Controller
// ===============
V8H_FORWARD_0_1(Buffer , data    , V8_PTR);
V8H_FORWARD_0_0(Buffer , zero);
V8H_FORWARD_0_0(Buffer , clear);


V8H_FORWARD_1_1(Buffer , prepare , TO_UINT   , V8_PTR);
V8H_FORWARD_1_0(Buffer , commit  , TO_UINT);
V8H_FORWARD_1_0(Buffer , consume , TO_UINT);

// Size Getter
// ===========
V8H_FORWARD_0_1(Buffer , size    , V8_UINT);
V8H_FORWARD_0_1(Buffer , remain  , V8_UINT);
V8H_FORWARD_0_1(Buffer , used    , V8_UINT);
V8H_FORWARD_0_1(Buffer , reserve , V8_UINT);

// Writer
// ======
V8H_FUNCTION(Buffer::writeBuffer)
{
	auto self = args.This();
	auto dest = getInternal(self);
	auto src  = getInternal(args[0]->ToObject());
	auto size = min((uint32_t)-1, src->size());
	dest->write(src, size);
	return self;
}

V8H_FUNCTION(Buffer::writeUtf8)
{
	auto self        = args.This();
	auto buffer      = getInternal(self);
	buffer->write(args[0]);
	return self;
}

V8H_FORWARD_1_0(Buffer , writeInt32  , TO_INT32);
V8H_FORWARD_1_0(Buffer , writeUint32 , TO_UINT32);
V8H_FORWARD_1_0(Buffer , writeInt64  , TO_INT64);
V8H_FORWARD_1_0(Buffer , writeDouble , TO_DOUBLE);

// Peeker
// ======
V8H_FUNCTION(Buffer::peekBuffer)
{
	auto self   = args.This();
	auto buffer = getInternal(self);

	v8::Handle<v8::Object> dest;
	uint32_t size = buffer->size();
	if (args[0]->IsObject()) {
		dest = args[0]->ToObject();
		// peekBuffer to Buffer
		if( args.Length() > 1) {
			size = TO_UINT(args[1]);
		}
	} else {
		// peekBuffer to a Buffer from BufferPool
		/*
		dest = Buffer::pop();
		if( args.Length() > 0) {
			size = TO_UINT(args[0]);
		}
		*/
	}
	size = min(size, buffer->size());
	buffer->peekBuffer(getInternal(dest), size);
	return dest;
}

V8H_FUNCTION(Buffer::peekUtf8)
{
	auto self = args.This();
	uint32_t n;
	auto buffer = getInternal(self);
	if (args.Length() > 0)
		n = TO_UINT(args[0]);
	else
		n = buffer->size();
	n = min(n, buffer->size());
	auto rt = v8::String::New(buffer->data(), n);
	return rt;
}

V8H_FORWARD_0_1(Buffer , peekInt32   , V8_INT32);
V8H_FORWARD_0_1(Buffer , peekUint32  , V8_UINT32);
V8H_FORWARD_0_1(Buffer , peekInt64   , V8_INT64);
V8H_FORWARD_0_1(Buffer , peekDouble  , V8_DOUBLE);

// Reader
// ======
V8H_FUNCTION(Buffer::readBuffer)
{
	auto self   = args.This();
	auto argc   = args.Length();
	auto buffer = getInternal(self);
	v8::Handle<v8::Object> dest;
	uint32_t size = (uint32_t)-1;
	if (args[0]->IsObject()) {
		dest  = args[0]->ToObject();
		size  = V8H_ARG(1, TO_UINT, size);
	} else {
		dest = newInstance();
		size = V8H_ARG(0, TO_UINT, size);
	}
	size = min(size, buffer->size());
	buffer->readBuffer(getInternal(dest), size);
	return dest;
}

V8H_FUNCTION(Buffer::readUtf8)
{
	auto self = args.This();
	uint32_t n;
	auto buffer = getInternal(self);
	if (args.Length() > 0)
		n = TO_UINT(args[0]);
	else
		n = buffer->size();
	n = min(n, buffer->size());
	auto rt = v8::String::New(buffer->data(), n);
	buffer->consume(n);
	return rt;
}

V8H_FORWARD_0_1(Buffer , readInt32   , V8_INT32);
V8H_FORWARD_0_1(Buffer , readUint32  , V8_UINT32);
V8H_FORWARD_0_1(Buffer , readInt64   , V8_INT64);
V8H_FORWARD_0_1(Buffer , readDouble  , V8_DOUBLE);

// Utils
// =====
V8H_FUNCTION(Buffer::partialFind)
{
	auto self          = args.This();
	auto needle        = TO_OBJ(args[0]);
	auto offset        = TO_UINT(args[1]);
	auto self_buffer   = getInternal(self);
	auto needle_buffer = getInternal(needle);
	auto pos           = self_buffer->partialFind(needle_buffer, offset);
	return V8_INT(pos);
}

V8H_FORWARD_0_0(Buffer , show);

// Constructor
// ===========
V8H_FUNCTION(Buffer::constructor) // }}}
{
	auto self = args.This();
	uint32_t reserve = 1024;
	if (args.Length() > 0 && args[0]->IsUint32()) {
		reserve = TO_UINT(args[0]);
	}
	auto buffer = new Buffer(reserve);
	buffer->wrap(self);
	if (args.Length() > 0 && args[0]->IsString()) {
		writeUtf8(args);
	}
	return self;
}

v8::Handle<v8::Function> Buffer::create()
{
	V8H_CREATE_START_WITH_INTERNAL_FIELD();

	// Data Controller
	// ===============
	V8H_IMPLEMENT(data);
	V8H_IMPLEMENT(zero);
	V8H_IMPLEMENT(clear);

	V8H_IMPLEMENT(prepare);
	V8H_IMPLEMENT(commit);
	V8H_IMPLEMENT(consume);

	// Size Getter
	// ===========
	V8H_IMPLEMENT(size);
	V8H_IMPLEMENT(used);
	V8H_IMPLEMENT(remain);
	V8H_IMPLEMENT(reserve);

	// Writer
	// ======
	V8H_IMPLEMENT(writeBuffer);
	V8H_IMPLEMENT(writeUtf8);
	V8H_IMPLEMENT(writeInt32);
	V8H_IMPLEMENT(writeUint32);
	V8H_IMPLEMENT(writeInt64);
	V8H_IMPLEMENT(writeDouble);

	// Reader
	// ======
	V8H_IMPLEMENT(readBuffer);
	V8H_IMPLEMENT(readUtf8);
	V8H_IMPLEMENT(readInt32);
	V8H_IMPLEMENT(readUint32);
	V8H_IMPLEMENT(readInt64);
	V8H_IMPLEMENT(readDouble);

	// Peeker
	V8H_IMPLEMENT(peekBuffer);
	V8H_IMPLEMENT(peekUtf8);
	V8H_IMPLEMENT(peekInt32);
	V8H_IMPLEMENT(peekUint32);
	V8H_IMPLEMENT(peekInt64);
	V8H_IMPLEMENT(peekDouble);

	V8H_IMPLEMENT(partialFind);
	V8H_IMPLEMENT(show);

	V8H_IMPLEMENT(close);
	V8H_CREATE_END();
}


V8H_NS_END
