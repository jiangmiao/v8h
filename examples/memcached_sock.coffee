# Run memcached: memcached -s /tmp/memcached.sock"
headerDelim = new Buffer("\r\n")

memcached = Socket.createUnix()
$error memcached, ->
  p $systemError
  puts "try 'memcached -s /tmp/memcached.sock' to start memcached unix socket"
  Socket.close memcached

puts "client: connecting /tmp/memcached.sock"
Socket.connectUnix memcached, "/tmp/memcached.sock", =>
  puts "client: connected"

  Stream.create memcached, ->
      # set key flags exptime bytes value CRLF
      puts "client: set key hello's value to 'hello world'"
      @writeUtf8 "set hello 0 30 11\r\nHello World\r\n", =>

        @readToken headerDelim, (n) =>
          print "server: "
          print @buffer.readUtf8(n)

          # get key
          puts "client: get key hello"
          benchmark_get = =>
            times = 0
            startTime = Date.now()
            do_benchmark = =>
              @writeUtf8 "get hello\r\n", =>
                recvMemcachedData = =>
                  @readToken headerDelim, (n) =>
                    header = @buffer.readUtf8(n)
                    if header.match /^VALUE /
                      [type, key, flags, length] = header.split " "
                      length = parseInt(length)
                      @readSize length, (n)=>
                        @readToken headerDelim, (n) =>
                          @buffer.consume(n)
                          recvMemcachedData()
                    else
                      ++times
                      elapsed = Date.now() - startTime
                      if elapsed > 2000
                        puts "get hello speed: " + parseInt(times*1000 / elapsed) + "/s"
                        puts "now benchmark the set speed in 2 seconds"
                        benchmark_set()
                      else
                        do_benchmark()
                recvMemcachedData()
            do_benchmark()

          benchmark_set = =>
            times = 0
            startTime = Date.now()
            do_benchmark = =>
              @writeUtf8 "set hello 0 30 11\r\nHello World\r\n", =>
                @readToken headerDelim, (n) =>
                  @buffer.clear()
                  ++times
                  elapsed = Date.now() - startTime
                  if elapsed > 2000
                    puts "set hello speed: " + parseInt(times*1000 / elapsed) + "/s"
                    puts "close the socket"
                    @close()
                  else
                    do_benchmark()
            do_benchmark()

          @writeUtf8 "get hello\r\n", =>
            recvMemcachedData = =>
              @readToken headerDelim, (n) =>
                header = @buffer.readUtf8(n)
                print "server: recv "+header
                if header.match /^VALUE /
                  [type, key, flags, length] = header.split " "
                  length = parseInt(length)
                  puts "client: #{key} has #{length} bytes value, recv the value"
                  @readSize length, (n)=>
                    puts "server: the value is #{@buffer.readUtf8(n)}"
                    @readToken headerDelim, (n) =>
                      @buffer.consume(n)
                      recvMemcachedData()
                else
                  puts "client: END receved"
                  puts "now benchmark the get speed in 2 seconds"
                  benchmark_get()
            recvMemcachedData()
