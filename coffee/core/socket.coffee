# Need a black hole Buffer
# BlackholeBuffer
# FileBuffer
Socket.extend
  # Methods defined in C++
  # _close
  # listen
  # binTcp
  # binUnix
  # creatTcp
  # creatUnix
  # _accepTcp
  # _accepUnix
  # _connecTcp
  # _connecUnix
  # _readBuffer
  # _writeBuffer
  #
  # Methods defined in Javascript
  # readUntil 
  # readSize
  # readToken
  # writeUntil
  # writeBuffer
  # writeUtf8
  # connectTcp
  # connectUnix
  # acceptTcp
  # acceptUnix
  # autoCloseOnError

  readUntil: (fd, buffer, condition, callback) ->
    $in fd, ->
      n = Socket._readBuffer fd, buffer
      if n == -1
        $triggerError(fd)
      else
        if condition()
          callback()
    if condition()
      callback()

  readSize: (fd, buffer, size, callback) ->
    if buffer.size() >= size
      callback size
    else
      Socket.readUntil fd, buffer, ->
        buffer.size() >= size
      , ->
        callback size

  readToken: (fd, buffer, deliBuffer, callback) ->
    deliBuffeSize = deliBuffer.size()
    if deliBuffeSize == 0
      callback 0
      return
    last          = 0
    Socket.readUntil fd, buffer, ->
      last = buffer.partialFindBuffer(deliBuffer, last)
      if buffer.isFound(last, deliBuffeSize)
        return true
      false
    , ->
      callback last + deliBuffeSize

  readInt32: (fd, buffer, callback) ->
    Socket.readSize fd, buffer, 4, ->
      callback buffer.readInt32()

  readUint32: (fd, buffer, callback) ->
    Socket.readSize fd, buffer, 4, ->
      callback buffer.readUint32()

  readInt64: (fd, buffer, callback) ->
    Socket.readSize fd, buffer, 4, ->
      callback buffer.readInt64()

  readDouble: (fd, buffer, callback) ->
    Socket.readDouble fd, buffer, 4, ->
      callback buffer.readDouble()

  writeUntilOnce: (fd, buffer, condition, callback) ->
    n = Socket._writeBuffer fd, buffer
    if n == -1 || condition()
      # if the buffer is temporary, it must be created by writeUtf8
      # so put it backt to the buffer pool
      if buffer.temporary?
        buffer.temporary = null
        $bufferPool.push buffer

      if n == -1
        $triggerError fd
      else
        callback()

  writeUntil: (fd, buffer, condition, callback) ->
    $out fd, ->
      Socket.writeUntilOnce fd, buffer, condition, callback

  writeBuffer: (fd, buffer, callback) ->
    $out fd, ->
    n = Socket._writeBuffer fd, buffer
    size = buffer.size()
    if n == -1 && c.errno != c.EAGAIN || size == 0
      if buffer.temporary?
        buffer.temporary = null
        $bufferPool.push buffer

      if size == 0
        callback()
      else
        $triggerError fd
    else
      $out fd, ->
        Socket.writeBuffer fd, buffer, callback


  writeUtf8: (fd, text, callback) ->
    buffer = $bufferPool.pop()
    buffer.writeUtf8(text)
    buffer.temporary = true
    Socket.writeBuffer fd, buffer, callback

  connectUnix: (fd, address, callback) ->
    $out fd, ->
      callback()
    Socket._connectUnix fd, address

  connectTcp: (fd, address, port, callback) ->
    $out fd, ->
      callback()
    Socket._connectTcp fd, address, port

  acceptTcp: (fd, callback) ->
    $in fd, ->
      client = Socket._acceptTcp fd
      if (client == -1)
        $triggerError fd
      else
        callback client

  acceptUnix: (fd, callback) ->
    $in fd, ->
      client = acceptUnix fd
      callback client

  autoCloseOnError: (fd) ->
    $error fd, ->
      Socket.close fd

  close: (fd) ->
    $del fd
    Socket._close fd

Socket.read = Socket.readSize

class Stream
  constructor: (@fd) ->
    @buffer = $bufferPool.pop()
    $error @fd, =>
      @close()


  readSize: (size, callback) ->
    Socket.readSize @fd, @buffer, size, callback

  readToken: (delim, callback)->
    Socket.readToken @fd, @buffer, delim, callback

  readUntil: (condition, callback) ->
    Socket.readUntil @fd, @buffer, condition, callback

  readInt32: (callback) ->
    Socket.readInt32 @fd, @buffer, callback

  clear: ->
    @buffer.clear()

  startWriting: (callback)->
    Socket.writeBuffer @fd, @buffer, callback

  writeInt32: (n, callback) ->
    @buffer.writeInt32 n, callback
    @startWriting callback

  writeUtf8: (text, callback) ->
    @buffer.writeUtf8 text
    @startWriting callback

  writeBuffer: (data, callback) ->
    @buffer.write data
    @startWriting callback

  writeUntil: (condition, callback) ->
    Socket.writeUntil @fd, @buffer, condition, callback

  close: (path) ->
    $bufferPool.push @buffer
    Socket.close @fd

  connectUnix: (path, callback) ->
    Socket.connectUnix @fd, path, callback

  connectTcp: (address, port, callback) ->
    Socket.connectTcp @fd, address, port, callback

  @create: (fd, callback) ->
    stream = new Stream fd
    callback.call stream
    stream

global.extend
  Socket: Socket
  Stream: Stream
exports = Socket

