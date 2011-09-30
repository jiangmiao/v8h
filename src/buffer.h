#ifndef V8H_BUFFER_H
#define V8H_BUFFER_H

#include <v8.h>
#include "v8h.h"
#include "object.h"
#include "internal.h"

V8H_NS_START

class Buffer :  public Object<Buffer>, public Internal<Buffer>
{
    protected:
	char     *memory_;
	uint32_t cursor_;
	uint32_t size_;
	uint32_t reserve_;
    public:
	Buffer(uint32_t reserve = 1024);
	~Buffer();

	// Data Control
	// ============
	char * data();
	void   zero();
	void   clear();

	char * prepare(uint32_t n);
	void   consume(uint32_t n);
	void   commit(uint32_t n);

	// Size Getter
	// ===========
	uint32_t size    ( ) const;
	uint32_t remain  ( ) const;
	uint32_t used    ( ) const;
	uint32_t reserve ( ) const;

	// Writer
	// ======
	void write(const char *data);
	void write(const void *data, uint32_t size);
	void write(v8::Handle<v8::Value> data);
	void write(Buffer *buffer, uint32_t size = UINT32_MAX);
	void writeInt32(int32_t data);
	void writeUint32(uint32_t data);
	void writeInt64(int64_t data);
	void writeDouble(double data);

	// Reader
	// ======
	void     read(void *dest, uint32_t size);
	void     readBuffer(Buffer *buffer, uint32_t size);
	int32_t  readInt32();
	uint32_t readUint32();
	int64_t  readInt64();
	double   readDouble();

	// Peeker
	// ======
	void     peek(void *dest, uint32_t size);
	void     peekBuffer(Buffer *buffer, uint32_t size);
	int32_t  peekInt32();
	uint32_t peekUint32();
	int64_t  peekInt64();
	double   peekDouble();

	// Utils
	uint32_t partialFind(Buffer *needle, uint32_t offset);
	void     show();
    public:
	// Data Control
	// ===========
	static V8H_FUNCTION(data);
	static V8H_FUNCTION(zero);

	static V8H_FUNCTION(prepare);
	static V8H_FUNCTION(commit);
	static V8H_FUNCTION(consume);
	static V8H_FUNCTION(clear);


	// Size Getter
	// ===========
	static V8H_FUNCTION(size);
	static V8H_FUNCTION(remain);
	static V8H_FUNCTION(used);
	static V8H_FUNCTION(reserve);

	// Writer
	// ======
	static V8H_FUNCTION(writeBuffer);
	static V8H_FUNCTION(writeUtf8);
	static V8H_FUNCTION(writeInt32);
	static V8H_FUNCTION(writeUint32);
	static V8H_FUNCTION(writeInt64);
	static V8H_FUNCTION(writeDouble);

	// Reader
	// ======
	static V8H_FUNCTION(readBuffer);
	static V8H_FUNCTION(readUtf8);
	static V8H_FUNCTION(readInt32);
	static V8H_FUNCTION(readUint32);
	static V8H_FUNCTION(readInt64);
	static V8H_FUNCTION(readDouble);

	// Peeker
	// ======
	static V8H_FUNCTION(peekBuffer);
	static V8H_FUNCTION(peekUtf8);
	static V8H_FUNCTION(peekInt32);
	static V8H_FUNCTION(peekUint32);
	static V8H_FUNCTION(peekInt64);
	static V8H_FUNCTION(peekDouble);

	// Utils
	// =====
	static V8H_FUNCTION(partialFind);
	static V8H_FUNCTION(show);

	// Construtor
	// ==========
	static V8H_FUNCTION(constructor);
	static v8::Handle<v8::Function> create();
};
V8H_NS_END

/*
 struct {
	 uint32_t cursor_;
	 uint32_t size_;
 } backup;
 void save();
 void restore();

 */
#endif
