http = require 'net/http.js'
info = ->
  $logger.info.apply $logger, arguments

delim = new Buffer("\r\n\r\n")
server = Socket.createTcp()
Socket.bindTcp server, "127.0.0.1", 9090, true
Socket.listen server, 500
Socket.acceptTcp server, (client)->
  Stream.create client.fd, ->
    header = ''
    @readToken delim, (n) =>
      header = @buffer.readUtf8(n)
      info "#{@fd}: recv header #{n} bytes."
      puts '----'
      print header
      puts '----'
      req    = http.Helper.parseHeader header
      header = req.header
      if header["Content-Length"]?
        @readSize parseInt(header["Content-Length"]), (n) =>
          info "#{@fd}: recv body #{n} bytes."
          process()
      else
        process()

    process = =>
      @buffer.clear()
      info "#{@fd}: response the request."
      @writeUtf8 "HTTP/1.1 200 OK\r\nConnection: close\r\n\r\nHello World" + header["Content-Length"] , =>
        info "#{@fd}: close the socket."
        @close()

