http = require 'net/http.js'
server = http.createServer (req, res) ->
  res.write("HTTP/1.1 200 OK\r\nConnection: close\r\n\r\nHello World")
  res.end()
server.listen "127.0.0.1", 9090, 500
