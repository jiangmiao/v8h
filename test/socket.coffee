{ok, eq} = Assert

ok v8h.arrayEqual Socket.parse('127.0.0.1:9090'),            [Socket.TCP, '127.0.0.1', 9090]
ok v8h.arrayEqual Socket.parse('unix:/tmp/memcached.sock'),  [Socket.UNIX, '/tmp/memcached.sock', 0]
