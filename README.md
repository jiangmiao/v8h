About V8H
=========

    V8H is a nodejs like system base on C++0x and V8 javascript Engine. 

    Currently, V8H is only support Linux platform.
    Requirements:
      Linux         2.6.32+
      GCC           4.5+
      V8            3+
      nodejs        0.4+ (need nodejs to run coffee)
      coffee-script 1.0+

Quick build
===========

    $ git clone git://github.com/jiangmiao/v8h.git
    $ cd v8h
    $ scons                                    # build V8H
    $ ./generate_js_from_coffee.sh             # generate js from coffee file,
                                               # include core and examples
    $ bin/v8h js/examples/http.js              # run example http.js
    $ ab -c 50 -n 20000 http://127.0.0.1:9090/ # benchmark
    or
    $ siege -c 5 -t 3S http://127.0.0.1:9090

Performance
===========

    V8H use native epoll and communite with V8 directly, so it's very fast.
    currently v8h is 200% faster than nodejs for hello world test.

    v8h    : bin/v8h js/examples/http.js
    nginx  : static file hello.txt with 12 bytes "hello world\0"
    nodejs : the 'hello world' code grab from nodejs.org

Server 1

    CPU    : Intel(R) Atom(TM) CPU D510   @ 1.66GHz
    System : ArchLinux 32bits

    $ siege -c 1 -t 3S
    v8h    (1619.91 trans/s)
    nginx  (1417.58 trans/s)
    nodejs ( 935.42 trans/s)

    $ siege -c 3 -t 3S
    nginx  (2208.64 trans/s)
    v8h    (1854.01 trans/s)
    nodejs (1207.73 trans/s)

    $ ab -c 50 -n 20000
    nginx  (4588.30 reqs/s)
    v8h    (4561.54 reqs/s)
    nodejs (2090.68 reqs/s)

Server 2

    CPU    : AMD Phenom(tm) 9650 Quad-Core Processor
    System : ArchLinux 64bits

    $ siege -c 1 -t 3S
    v8h    (10850.71 trans/s)
    nginx  ( 9952.20 trans/s)
    nodejs ( 5694.71 trans/s)

    $ siege -c 8 -t 3S
    nginx  (20083.93 trans/s)
    v8h    (19897.75 trans/s)
    nodejs ( 7826.81 transls)

    $ ab -c 500 -n 50000
    v8h    (20450.32 reqs/s)
    nginx  (19229.02 reqs/s)
    nodejs ( 8222.89 reqs/s)


Further Work
============

    1、Asynchronous File I/O
    2、Timer
    3、Full HTTP protocol implement
    4、Native Module Supported


