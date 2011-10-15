class Helper
  @parseHeader = (text) ->
    header = {}
    fields = text.split "\r\n"
    request = fields.shift()
    [method, requestUri] = request.split " ", 3
    for field in fields
      [key, value] = field.split ": ", 2
      continue unless value?
      header[key] = value

    env =
      method: method
      requestUri: requestUri
      header: header
    env


class Client
  @headerDelim: new Buffer("\r\n\r\n")
  @boundaryDelim: new Buffer("\r\n")
  constructor: (@client, @callback) ->
    @fd  = @client.fd
    self = @
    $error @fd, ->
      self.error()
    @buffer = $bufferPool.pop()

  start: ->
    @recvHeader()

  error: ->
    @close()

  close: ->
    $bufferPool.push @buffer
    if @boundary?
      $bufferPool.push @boundary
    Socket.close @fd

  recvHeader: ->
    self = @
    @buffer.clear()
    Socket.readToken @fd, @buffer, Client.headerDelim, (n) =>
      header = @buffer.readUtf8(n)
      @req =  Helper.parseHeader header
      header = @req.header
      @header = header
      if header.Connection? && header.Connection.toLowerCase() == 'keep-alive'
        @isClosed = false
      else
        @isClosed = true

      if header['Content-Length']?
        @contentLength = parseInt(header['Content-Length'])
        @recvBody()
      else
        @process()
    , @
  recvBody: ->
    if @header['Content-Type'].match /^multipart\/form-data/
      m = @header['Content-Type'].match /boundary=(.*)/
      unless m
        @close()
        return
      @boundary = $bufferPool.pop()
      @boundary.writeUtf8('--' + m[1])
      @recvMultiPart()
    else
      Socket.readSize @fd, @buffer, @contentLength, =>
        @process()


  recvMultiPart: ->
    Socket.readToken @fd, @buffer, @boundary, (n) =>
      @buffer.consume(n)
      Socket.readSize @fd, @buffer, 2, (n) =>
        str = @buffer.readUtf8(2)
        if str == "\r\n"
          Socket.readToken @fd, @buffer, Client.headerDelim, (n) =>
            header = @buffer.readUtf8(n)
            puts header
            @recvMultiPart()
        else
          Socket.readToken @fd, @buffer, Client.boundaryDelim, (n) =>
            @process()


  process: (callback)->
    @buffer.clear()
    @callback @req, @

  write: (text) ->
    @buffer.writeUtf8 text

  flush: ->
    Socket.writeBuffer @fd, @buffer, =>

  end: ->
    Socket.writeBuffer @fd, @buffer, =>
      if @isClosed
        @close()
      else
        @start()
    , @

class Server
  constructor: (@callback) ->
  listen: (@address, @port, @backlog) ->
    callback = @callback
    @server  = Socket.createTcp()
    Socket.autoCloseOnError @server
    rt = Socket.bindTcp @server, @address, @port, true
    if rt == -1
      p $systemError
      Socket.close @server
      return
    Socket.listen @server, @backlog
    Socket.acceptTcp @server, (client) ->
      client = new Client(client, callback)
      client.start()

exports =
  Server: Server
  Client: Client
  Helper: Helper
  createServer: (callback) ->
    new Server callback
