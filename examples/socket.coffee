delim = new Buffer("\r\n\r\n")

Socket.createServer '127.0.0.1:9090', (client) ->
  buffer = Buffer.pop()
  message = Buffer.pop()
  message.writeUtf8 "HTTP/1.1 200 OK\r\nConnection: close\r\n\r\nHello World"

  close = ->
    client.close()
    Buffer.push buffer
    Buffer.push message

  client.error ->
    close()

  client.readToken buffer, delim, ->
    client.writeBuffer message, ->
      close()
