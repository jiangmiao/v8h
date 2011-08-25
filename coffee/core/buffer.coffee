class BufferPool
  constructor: (@limit, @buffer_reserve_limit = 1024 * 1024)->
    @buffers = []
    @num     = 0

  push: (buffer) ->
    if buffer.pushed?
      return
    if @buffers.length > @limit || buffer.reserve() > @buffer_reserve_limit
      buffer.close()
    else
      buffer.pushed = true
      @buffers.push buffer

  pop: ->
    if @buffers.length
      buffer = @buffers.pop()
      buffer.pushed = null
      buffer.clear()
      return buffer
    else
      return new Buffer(512)

global.$bufferPool = new BufferPool(100)

Buffer::extend
  isFound: (last, delim_buffer_size) ->
    # assert delim_buffer_size > 0 so ignore @size() == 0 && delim_buffer_size == 0
    last + delim_buffer_size <= @size()

  readBuffer: (size) ->
    buffer = $bufferPool.pop()
    @_readBuffer buffer, size
    buffer

  peekBuffer: (size) ->
    buffer = $bufferPool.pop()
    @_peekBuffer buffer, size

