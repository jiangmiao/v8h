class Logger
  @LOG_DEBUG = 0x16
  @LOG_INFO  = 0x8
  @LOG_WARN  = 0x4
  @LOG_ERROR = 0x2
  @LOG_FAULT = 0x1

  constructor: ->
    @logLevel = Logger.LOG_DEBUG

  log: (name, text, level) ->
    if @logLevel >= level
      puts "[#{name}] #{text}"

  info: (text) ->
    @log "INFO", text, Logger.LOG_INFO

  warn: (text) ->
    @log "WARN", text, Logger.LOG_WARN

  error: (text) ->
    @log "ERROR", text, Logger.LOG_ERROR

  debug: (text) ->
    @log "DEBUG", text, Logger.LOG_DEBUG

global.extend
  $logger : new Logger()
  Logger  : Logger
