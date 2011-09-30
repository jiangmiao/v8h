Object::extend = (object)->
  for k, v of object
    if object.hasOwnProperty(k)
      @[k] = v

Object::eval = (callback) ->
  callback.call @
