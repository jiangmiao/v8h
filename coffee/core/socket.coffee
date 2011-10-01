i = Internal.Socket
v8h.extend i,
  createTcp: ->
    @socket(@PF_INET, @SOCK_STREAM, 0)

  createUnix: ->
    @socket(@PF_UNIX, @SOCK_STREAM, 0)

  accept: (family, fd) ->
    switch family
      when 'tcp' then @acceptTcp fd
      when 'unix' then @acceptUnix fd

class Socket
  constructor: (@fd, @family, @address, @port) ->

  listen: (backlog) ->
    i.listen @fd, backlog

  accept: (callback)->
    $in @fd, =>
      [fd, address, port] = i.accept @family, @fd
      client = new Socket(fd, @family, address, port)
      callback.call this, client

  toString: ->
    "fd: #{@fd} family: #{@family} address: #{@address} port: #{@port}"

  error: (callback) ->
    $error @fd, ->
      callback()

  close: ->
    $del @fd
    i.close @fd


  # Reader
  # ======
  # read until condition become true
  readUntil: (buffer, condition, callback) ->
    $in @fd, =>
      n = i.readBuffer @fd, buffer
      if n == -1
        $triggerError(@fd)
      else if n == 0 #closed by client
        $triggerError(@fd)
      else
        callback() if condition()
    callback() if condition()

  readSize: (buffer, size, callback) ->
    if buffer.size() >= size
      callback size
    else
      @readUntil buffer, ->
        buffer.size() >= size
      , ->
        callback size

  readToken: (buffer, delim, callback) ->
    unless delim instanceof Buffer
      throw new Error("delim must be an instance of Buffer")
    delim_size = delim.size()
    if delim_size == 0
      callback 0
      return
    last = 0
    @readUntil buffer, ->
      last = buffer.partialFind(delim, last)
      last + delim_size <= buffer.size()
    , ->
      callback last + delim_size

  # Writer
  # ======
  writeUntilOnce: (buffer, condition, callback) ->
    n = i.writeBuffer @fd, buffer
    if n == -1
      Buffer.pushTemp buffer
      $triggerError @fd
    else if condition()
      Buffer.pushTemp buffer
      callback()

  writeUntil: (buffer, condition, callback) ->
    $out @fd, =>
      @writeUntilOnce buffer, condition, callback
    @writeUntilOnce buffer, condition, callback

  writeBuffer: (buffer, callback) ->
    @writeUntil buffer, ->
      buffer.size() == 0
    , ->
      callback()

  writeUtf8: (text, callback) ->
    buffer = Buffer.popTemp()
    buffer.writeUtf8(text)
    @writeBuffer buffer, callback

  # Static Methods
  # ==============
  @parse: (address_str) ->
    address = address_str.split ':'
    if address[0] != 'unix' and address[0] != 'tcp'
      address.unshift 'tcp'
    if address.length < 3
      address[2] = 0
    address


  # createServer address, [options,] callback
  # options: backlog
  @createServer: ->
    args     = Array::slice.call arguments, 0
    address  = args.shift()
    callback = args.pop()
    options  = args[0] ? {}
    backlog  = options.backlog ? 500

    [family, address, port] = @parse(address)
    switch family
      when 'tcp'
        fd = i.createTcp()
        i.bindTcp(fd, address, port)
      when 'unix'
        fd = i.createUnix()
        i.bindUnix(fd, address)
    server = new Socket(fd, family, address, port)
    server.listen backlog
    server.accept callback

global.Socket = Socket

