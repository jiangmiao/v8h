do ->
  benchmark = (title, callback, times = 1e7) ->
    start = Date.now()
    for i in [1..times]
      callback(i)
    spent = Date.now() - start
    speed = times / spent
    console.info title, speed
    speed

  verbose = true
  log1 = (callback)->
    if (verbose?)
      callback()

  log2 = (text) ->
    if (verbose?)
      text
  world = "world"
  benchmark "callback", ->
    log1 -> "HELLO #{world}"

  benchmark "callback2", ->
    log2 "HELLO #{world}"

  verbose  = false
  benchmark "callback skip", ->
    log1 -> "HELLO #{world}"

    
