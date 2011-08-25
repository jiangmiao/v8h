delim  = new Buffer("\r\n\r\n")

# Create Socket
server = Socket.createTcp()

# bind server to 127.0.0.1:9090, reuse the address
Socket.bindTcp server, "127.0.0.1", 9090, true

# Make server listen backlog = 50
Socket.listen server, 50

# Pollin server
$in server, ->

  # system accept
  # returns {fd: ,address: ,port:} if failed return -1
  client = Socket._acceptTcp server
  if client == -1
    # show the systemError 
    p $systemError

    # Safe close server, it will also remove server from epoll
    Socket.close server

  fd     = client.fd

  # get buffer from global $bufferPool
  buffer = $bufferPool.pop()
  text   = $bufferPool.pop()

  # set text's text
  text.writeUtf8("HTTP/1.1 200 OK\r\nConnection: close\r\n\r\nHello World")

  # define close action
  close = ->
    # put the buffer back to the pool, it will discard all data in buffer
    $bufferPool.push buffer
    $bufferPool.push text
    Socket.close fd

  # if there is any error occurred on fd
  $error fd, ->
    p $systemError
    close()

  # pollin fd
  $in fd, ->

    # read data to buffer
    Socket._readBuffer fd, buffer

    # send the text to fd, the sent data will be removed from text.
    Socket._writeBuffer fd, text

    # close the socket and push the buffers to the pool.
    close()
