{ok, eq} = Assert

test = (name, runCase) ->
  runCase()

test "Buffer: writeUtf8, consume", ->
  buffer = new Buffer()
  ok buffer.size() is 0

  buffer.writeUtf8("hello world")
  ok buffer.size() is 11

  buffer.consume(6)
  ok buffer.size() is 5


test "Buffer: prepare, commit, zero, clear", ->
  buffer = new Buffer(1)
  ok buffer.size()    is 0
  ok buffer.reserve() is 1

  buffer.prepare 64
  ok buffer.reserve() is 1*3*3*3*3

  buffer.commit 64
  ok buffer.size() is 64

  buffer.prepare 64
  ok buffer.reserve() is 1*3*3*3*3*3
  ok buffer.size()    is 64

  buffer.clear()
  eq buffer.size()   , 0
  eq buffer.reserve(), 1*3*3*3*3*3

test "Buffer: write read peek", ->
  buffer = new Buffer(1)

  buffer.writeInt32  8192
  buffer.writeUint32 0x87654321
  buffer.writeInt64  0x1234567890
  buffer.writeDouble 3.1415926
  buffer.writeUtf8 "hello世界"

  eq buffer.peekInt32()  , 8192         , 'peekInt32'
  eq buffer.readInt32()  , 8192         , 'readInt32'
  eq buffer.peekUint32() , 0x87654321   , 'peekUint32'
  eq buffer.readUint32() , 0x87654321   , 'readUint32'
  eq buffer.peekInt64()  , 0x1234567890 , 'peekInt64'
  eq buffer.readInt64()  , 0x1234567890 , 'readInt64'
  eq buffer.peekDouble() , 3.1415926    , 'peekDouble'
  eq buffer.readDouble() , 3.1415926    , 'readDouble'
  eq buffer.peekUtf8()   , "hello世界"  , 'peekUtf8'
  eq buffer.readUtf8()   , "hello世界"  , 'readUtf8'
  eq buffer.size()       , 0

  destBuffer = new Buffer(1)
  textBuffer = new Buffer("hello world")
  buffer.writeBuffer textBuffer
  eq buffer.peekBuffer(destBuffer, 5).peekUtf8(), "hello"           , 'peekBuffer'
  eq buffer.readBuffer(destBuffer, 5).peekUtf8(), "hellohello"      , 'readBuffer'
  eq buffer.peekUtf8()                          , ' world'          , 'peekBuffer2'
  eq buffer.readBuffer(destBuffer).readUtf8()   , "hellohello world", 'readBuffer2'

test "Buffer partialFind", ->
  buffer = new Buffer("hello world")
  eq buffer.partialFind(new Buffer("ld")), 9
  eq buffer.partialFind(new Buffer("he")), 0
  eq buffer.partialFind(new Buffer("ff")), 11

test "BufferPool", ->
  buffer = Buffer.pop()
  buffer.writeUtf8("hello")
  Buffer.push buffer
  ok buffer.pushed?

  buffer2 = Buffer.pop()
  ok !buffer.pushed?
  ok buffer2.size() is 0
  ok buffer is buffer2
