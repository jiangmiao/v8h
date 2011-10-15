# Run memcached: memcached -s /tmp/memcached.sock"
headerDelim = new Buffer("\r\n")
message = new Buffer("Hello world");

Socket.connect "unix:/tmp/memcached.sock", (client) ->
  if !client?
    System.raise "connect failed"

  close = ->
    client.close()

  client.error ->
    puts "ERROR"
    close()

  times = 1e3
  st = Date.now()
  do storeOnce = ->
    buffer = Buffer.pop()
    buffer.writeUtf8 "set hello 0 3600 #{message.size()}\r\n"
    buffer.writeBuffer message
    buffer.writeUtf8 "\r\n"
    client.writeBuffer buffer, (n)->
      buffer.consume(n)
      client.readToken buffer, headerDelim, (n) ->
        --times
        if times
          Buffer.push buffer
          storeOnce()
        else
          puts 1e3 * 1000 / (Date.now() - st) + "/s"
          benchmarkGet()

  benchmarkGet = ->
    times = 1e3
    st = Date.now()
    do getOnce = ->
      buffer = Buffer.pop()
      buffer.writeUtf8 "get hello\r\n"
      client.writeBuffer buffer, ->
        client.readToken buffer, headerDelim, (n) ->
          header = buffer.readUtf8(n)
          [type, key, flags, length] = header.split " "
          if type != 'VALUE'
            return close()
          length = parseInt(length)
          client.readSize buffer, length, (n) ->
            text = buffer.readUtf8(n)
            --times
            if times
              Buffer.push buffer
              getOnce()
            else
              puts 1e3 * 1000 / (Date.now() - st) + "/s"
              close()