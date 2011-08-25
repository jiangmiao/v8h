$in = (fd, callback)->
  service.in fd, callback

$out = (fd, callback)->
  service.out fd, callback

$del = (fd, callback)->
  service.del fd, callback

$error = (fd, callback) ->
  service.error fd, callback

$triggerError = (fd) ->
  callback = service.fds_error[fd]
  unless callback?
    throw new Error "missing error callback"
  callback()
  

$close = (fd)->
  service.del fd
  Socket.close fd

$run = ->
  service.run.apply service, arguments

$runJS = ->
  times = 0
  maxTimes = 21234
  if $gcTimes?
    maxTimes = $gcTimes
  while true
    n = service.wait()
    for i in [0...n]
      service.triggerByEventId(i)
    times += n
    if times > maxTimes
      times = 0
      forceGC()

  @

global.extend
  $in           : $in
  $out          : $out
  $del          : $del
  $error        : $error
  $triggerError : $triggerError
  $close        : $close
  $run          : $run
  $runJS        : $runJS

