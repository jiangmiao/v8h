Object::extend = (object)->
  for k, v of object
    if object.hasOwnProperty(k)
      @[k] = v

Object::eval = (callback) ->
  callback.call @

File.join = ->
  ([].slice.call arguments, 0).join("/")

# require
global.extend
  require: do ->
    requiredFiles = {}

    requireAbsolute = (filename)->
      realPath = File.realpath filename
      unless realPath?
        throw new Error "require: cannot find file #{filename}"
      filename = realPath

      if requiredFiles[filename]?
        if requiredFiles[filename] == 1
          throw new Error "require: loop require #{filename}"
        return requiredFiles[filename]

      # mark the file is in requiring
      requiredFiles[filename] = 1
      exports                 = _require(filename)

      # cache the exports
      requiredFiles[filename] = exports
      exports

    requireRelative = (filename)->
      requireAbsolute File.join $currentDir, filename

    requireIncludePath = (filename)->
      found = false
      for dir in $includePath
        path = File.join(dir, filename)
        if File.exists path
          filename = path
          found = true
          break
      unless found
        throw new Error "require: cannot find file #{filename}"
      requireAbsolute(filename)

    (filename) ->
      if filename[0] == '/'
        originalFileName = filename
      else
        originalFileName = $currentDir + "?" + filename

      if requiredFiles[originalFileName]?
        return requiredFiles[originalFileName]

      if filename[0] == '/'
        exports = requireAbsolute(filename)
      else if filename.substr(0,2) == './'
        exports = requireRelative(filename)
      else
        exports = requireIncludePath(filename)
        # search in library path

      requiredFiles[originalFileName] = exports



  p: ->
    puts.apply console, arguments

require './logger.js'
require './buffer.js'
require './service.js'
require './console.js'
require './socket.js'

scriptFile = $argv[1]
if scriptFile[0] == '/'
  require script
else
  require $workingDir + '/' + scriptFile

$run()
