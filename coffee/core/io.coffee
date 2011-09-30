# Type | Prefix | mode
# ---------------------
# UNIX | unix:  | r w
# TCP  | tcp:   | r w
# FILE | file:  | r w a

# Examples:
# UNIX unix:/tmp/memcached.sock
# TCP  tcp:www.baidu.com
# TCP  tcp:8.8.8.8
# FILE file:/tmp/memcached.sock
# FILE /tmp/memcached
# FILE memcached
# FILE file:something

# IO Specification
#
# Function needs override readUntilOnce readWriteOnce
class IO
  close: ->

  # read buffer, number, callback
  # read buffer, string, callback
  # read buffer, function, callback
  read        : (misc, callback) ->
    type = typeof(misc)
    switch type
      when 'number'
        misc = parseInt(misc)
        @readSize misc, callback
      when 'string'
        @readToken misc, callback
      when 'function'
        @readFunction misc, callback
      else
        throw new TypeError("read only support Number, String or Function.")

  readSize  : (size, callback) ->
    @readUntil @buffer, ->
      @buffer.size() >= size
    , ->
      callback.call @, size

  readToken : (delim, callback) ->
    if delim instanceof Buffer
      delimBuffer = delim
    else
      deliBuffer = Buffer.popTemp()
      deliBuffer.writeUtf8(delim)
    delimBuffeSize = delimBuffer.size()
    if delimBuffeSize == 0
      callback @

    last = 0
    @readUntil ->
      last = @buffer.partialFindBuffer(delimBuffer)
      if buffer.isFound(last, delimBuffer) then true else false
    , ->
      callback.call @ size, delimBuffeSize

  readUntilOnce: (condition, callback) ->
  readUntil : (condition, callback) ->
    if condition.call @
      callback.call @
    else
      $in @fd, ->
        @readUntil condition, callback

  readSizeUtf8    : (size, callback) ->
    @readSize size, (size) ->
      callback.call @, @buffer.readUtf8(size)

  readTokenUtf8: (delim, callback) ->
    @readTokenBuffer size, (size, delimSize) ->
      data = @buffer.readUtf8 size
      @buffer.consume delimSize
      callback.call @, data


  # string, callback           -> writeUtf8
  # buffer, callback           -> writeBuffer
  # buffer, number  , callback -> writeSize
  # buffer, function, callback -> writeUntil
  write       : ->
    switch arguments.length
      when 2
        # string, callback
        # buffer, callback

        [misc, callback] = arguments
        type = typeof(misc)
        switch type
          when 'string'
            @writeUtf8 misc, callback
          when 'object'
            name = misc.constructor.name
            if name == 'Buffer'
              @writeBuffer buffer, buffer.size(), callback
            else
              throw new TypeError("write only support Buffer or String.")
          else
            throw new TypeError("write only support Buffer or String.")
      when 3
        # buffer, number  , callback
        # buffer, string  , callback
        # buffer, function, callback

        [buffer, misc, callback] = arguments
        type = typeof(misc)
        switch type
          when 'number'
            @writeSize buffer, misc, callback
          when 'function'
            @writeFunction buffer, misc, callback
          else
            throw new TypeError("write only support Number or Function.")
      else
        throw new TypeError("write needs 2 or 3 arguments.")

  writeSize   : (buffer, length, callback) ->
  writeUntil  : (buffer, condition, callback) ->

  writeUtf8   : (text, callback) ->
  writeBuffer : (buffer, callback) ->

  writeDouble : (data, callback) ->
    buffer = Buffer.popTemp()
    buffer.writeDouble(data)
    @writeBuffer buffer, callback

  writeInt32  : (data, callback) ->
  writeUint32 : (data, callback) ->

  @bind: (type, callback) ->
    @["open#{type.capitalize()}"] = callback
    @["open_#{type}"] = callback

  @open: (path, mode) ->
    if type = @path.match /(unix|tcp|file)\:(.*)/
      @type = type[1]
      @path = type[2]
    else
      @type = 'file'
    func = "open_#{type}"
    unless @[func]?
      throw new Error "invalid type #{type}"
    @[func].call @, @path, @mode

  @checkMethods: ->
    methods = ['readOnce', 'writeOnce']
    #for k, v in @prototype
