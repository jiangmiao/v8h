do ->
  File.join = ->
    Array::join.call arguments, '/'

  # Require
  requiredFiles = {}
  global.requiredFiles = requiredFiles
  global.require = (file) ->
    hint = "#{requireDir}?#{file}"
    if requiredFiles[hint]?
      return requiredFiles[hint]

    if file[0] == '.' and file[1] == '/'
      path = File.join requireDir, file
      path = File.realpath path
      # relative
    else if file[0] == '/'
      # absolute
      path = File.realpath file
    else
      # search in module path
      path = false

    if !path
      throw new Error("require: cannot find file #{file}")
    if requiredFiles[path]
      requiredFiles[hint] = requiredFiles[path]
      return requiredFiles[hint]
    exports = System.absoluteRequire path
    requiredFiles[hint] = requiredFiles[path] = exports
    exports

  require './v8h.js'
  require './buffer.js'

  if ARGV[1]?
    script = ARGV[1].toString()
    if script[0] == '/'
      require ARGV[1]
    else
      require File.join System.getWorkingDir(), ARGV[1]
  else
    puts "usage: v8h script"

###
scriptFile = $argv[1]
if scriptFile[0] == '/'
  require script
else
  require __DIR__ + '/' + scriptFile

$run()
###
