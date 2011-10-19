# This is the ideal model HTTP server, currently doesn't work.

http = require('net/http_easy.js')

http.config
  address     : '127.0.0.1'
  listen      : 80
  backlog     : 500
  maxRequests : 10000

host = http.addHost 'example.com'
host.config
  root: '/home/miao/sites/example.com'

host.routes.config \
  ['root'       , '/'          , 'root' , 'index'] ,
  ['root_index' , '/[id].html' , 'root' , 'show'   , id: /\d+/]

host.process ->
  @_SERVER  # $_SERVER
  @_GET     # $_GET
  @_POST    # $_POST
  @_REQUEST # $_REQUEST
  @_FILES   # $_FILES
  @_COOKIE  # $_COOKIE
  @_SESSION # $_SESSION

  @renderFile "#{@controller}/#{@action}.js"

http.run()
