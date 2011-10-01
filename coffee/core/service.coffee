service = new Service()

$in = (fd, callback)->
  service.in fd, callback

$out = (fd, callback)->
  service.out fd, callback

$del = (fd, callback)->
  service.del fd, callback

$error = (fd, callback) ->
  service.error fd, callback

$triggerError = (fd) ->
  callback = service.fdsError[fd]
  unless callback?
    throw new Error "missing error callback"
  callback()
  

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

v8h.extend global,
  $in           : $in
  $out          : $out
  $del          : $del
  $error        : $error
  $run          : $run
  $triggerError : $triggerError

###
  $close        : $close
  $runJS        : $runJS
###

