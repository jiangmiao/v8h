class v8h
  @extend = ->
    args = Array::slice.call arguments, 0
    dest = args.shift()
    for ob in args
      for k, v of ob
        if ob.hasOwnProperty k
          dest[k] = v
    dest

global.v8h = v8h
