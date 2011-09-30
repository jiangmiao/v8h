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

  resize: (@limit)->
    while @limit < @buffers.length
      @buffers.pop()


$bufferPool = new BufferPool(100)

v8h.extend Buffer,
  pop: ->
    $bufferPool.pop()

  push: (buffer)->
    buffer.isTemp = null
    $bufferPool.push(buffer)

  popTemp: ->
    buffer = $bufferPool.pop()
    buffer.isTemp = true

  pushTemp: (buffer)->
    if buffer.isTemp?
      buffer.push buffer

  slots: (n)->
    $bufferPool.resize(n)
