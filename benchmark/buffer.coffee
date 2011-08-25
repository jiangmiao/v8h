benchmark = (title, callback, times = 1e5) ->
  start = Date.now()
  for i in [1..times]
    callback(i)
  spent = Date.now() - start
  speed = times / spent
  console.info title, speed
  speed

text = 'hello world'
for i in [1..10]
  text += 'hello world'
hello = new Buffer(text)
buffer = new Buffer()
benchmark 'buffer writeBuffer', ->
  buffer.writeBuffer hello

buffer.clear()
benchmark 'buffer writeAscii', ->
  buffer.writeAscii hello

buffer.clear()
benchmark 'buffer writeUcs2', ->
  buffer.writeUcs2 hello

buffer.clear()
benchmark 'buffer writeUtf8', ->
  buffer.writeUtf8 text

str = ''
benchmark 'string', ->
  str += text
