i = Internal.Socket
v8h.extend i,
  createTcp: ->
    @socket(@PF_INET, @SOCK_STREAM, 0)

  createUnix: ->
    @socket(@PF_UNIX, @SOCK_STREAM, 0)

  accept: (family, fd) ->
    switch family
      when i.PF_INET 
        @acceptTcp fd
      when i.PF_UNIX 
        @acceptUnix fd

class Socket
  @TCP:  i.PF_INET
  @UNIX: i.PF_UNIX

  constructor: (@fd, @family, @address, @port) ->
    @events = {}

  listen: (backlog) ->
    i.listen @fd, backlog

  accept: (callback) ->
    $in @fd, =>
      [fd, address, port] = i.accept @family, @fd
      client = new Socket(fd, @family, address, port)
      callback.call this, client

  connect: (family, address, port, callback) ->
    $error @fd, =>
      callback.call @, null

    $out @fd, =>
      callback.call @, @

    switch family
      when i.PF_INET
        i.connectTcp @fd, address, port
      when i.PF_UNIX
        i.connectUnix @fd, address

  toString: ->
    "fd: #{@fd} family: #{@family} address: #{@address} port: #{@port}"

  error: (callback) ->
    $error @fd, ->
      callback()

  close: ->
    $del @fd
    i.shutdown @fd, i.SHUT_RDWR
    i.close @fd

  emit: (event) ->
    if events[event]?
      events[event].call @

  on: (event, callback) ->
    events[event] = callback

  # Reader
  # ======
  # read until condition become true
  readUntil: (buffer, condition, callback) ->
    $in @fd, =>
      n = i.readBuffer @fd, buffer
      if n == -1
        $triggerError(@fd)
      else if n == 0 #closed by client
        # @emit "closed"
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
    switch address[0]
      when 'unix'
        address[0] = i.PF_UNIX
      when 'tcp'
        address[0] = i.PF_INET
      else
        address.unshift Socket.TCP
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
      when i.PF_INET
        fd = i.createTcp()
        if i.bindTcp(fd, address, port) == -1
          System.raiseSystem "bind address #{address}:#{port} failed"
      when i.PF_UNIX
        fd = i.createUnix()
        if i.bindUnix(fd, address) == -1
          System.raiseSystem "bind unix socket #{address} failed"
      else
        System.raise "unsupported protocol"

    server = new Socket(fd, family, address, port)
    server.listen backlog
    server.accept callback

  # connect address, [options,] callback
  @connect: ->
    args     = Array::slice.call arguments, 0
    address  = args.shift()
    callback = args.pop()
    options  = args[0] ? {}

    [family, address, port] = @parse(address)
    switch family
      when i.PF_INET
        fd = i.createTcp()
        System.raise "create tcp" if fd == -1
      when i.PF_UNIX
        fd = i.createUnix()
        System.raise "create unix" if fd == -1
    server = new Socket(fd, family, "", 0)
    server.connect family, address, port, callback


global.Socket = Socket

