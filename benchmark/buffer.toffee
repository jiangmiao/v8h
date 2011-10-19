{benchmark} = require('./../modules/core/benchmark.js')

benchmark 'Buffer pop then push', (times)->
  for i in [1..times]
    buffer = Buffer.pop()
    Buffer.push buffer
  @

benchmark 'Buffer writeUtf8 then clear', (times)->
  buffer = new Buffer()
  for i in [1..times]
    buffer.writeUtf8("hello hello hello hello")
    buffer.clear()
  Buffer.push buffer
  @

benchmark 'Buffer peekUtf8', (times)->
  buffer = new Buffer()
  buffer.writeUtf8("hello hello hello hello")
  for i in [1..times]
    buffer.peekUtf8()
  Buffer.push buffer
  @
