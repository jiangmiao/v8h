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
V8H use native epoll and communite with V8 directly, so it's very fast even fast than nginx.
currently about 250% faster than nodejs for hello world test.

Further Work
============

    1、Asychronous File I/O
    2、Timer
    3、Full HTTP protocol implement
    4、Native Module Supported


