{benchmark} = require('./../modules/core/benchmark.js')

strs = []
for i in [1..1e3]
  strs.push 'hello'

str = strs.join(' ')

benchmark 'split', (times) ->
  for i in [1..times]
    buffer = Buffer.pop()
    buffer.writeUtf8(str)
    Buffer.push(buffer)
  @
