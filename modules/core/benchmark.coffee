exports.benchmark = ->
    args = Array::slice.call arguments, 0
    callback = args.pop()
    if args.length
      print args[0]
    else
      print 'unknown benchmark'
    
    times = 1
    time = 0
    evaluate = true
    while !done
      st = Date.now()
      callback(times)
      ed = Date.now()
      time = ed - st
      break unless evaluate
      if time < 100
        times *= 3
      else if time > 2000
        done = true
      else
        times = times * 2000 / time
        evaluate = false
    score = times * 1000 / time
    unit = ''
    if score > 10000
      score /= 1000
      unit = 'k'
    puts "#{Math.floor(score)}#{unit} tps"
