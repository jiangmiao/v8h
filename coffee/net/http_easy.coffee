# Host
class Host
  config: ->
    @extend.apply this, arguments

  process: (callback) ->
    @process = callback

  constructor: ->
    @config
      root: '.'
      route: {}

    @process = ->
      @renderFile "#{@controller}/#{@action}.js"
# Http
class Http
  config: ->
    @extend.apply this, arguments

  addHost: (hostName) ->
    @hosts[hostName] = new Host()

  constructor: ->
    @config
      address     : '127.0.0.1'
      listen      : 80
      backlog     : 500
      maxRequests : 10000

    @hosts = []
