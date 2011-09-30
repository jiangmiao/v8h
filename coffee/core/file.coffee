# depends IO

class File extends IO
  @realpath: (path) ->
    buffer = Buffer.pop()
    path = c.realpath(path, buffer.prepare(c.PATH_MAX))
    if path != null
      path = c.string(path)
    Buffer.push buffer
    path

  @syncReadUtf8: (path) ->
    buffer = @syncReadBuffer(path)
    if buffer?
      data = buffer.readUtf8()
      Buffer.push buffer
    else
      data = null
    data

  @syncReadBuffer: (path, buffer = null) ->
    rt = null
    file = c.fopen(path, "r")
    if file?
      unless buffer?
        buffer = Buffer.pop()
      while n = c.fread(buffer.prepare(4096), 1, 4096, file)
        buffer.commit(n)
      if c.ferror(file) == 0
        rt = buffer
    rt

  @syncWriteUtf8: (path, data) ->
    rt = -1
    file = c.fopen(path, "w")
    if file?
      buffer = Buffer.pop()
      buffer.writeUtf8(data)
      while buffer.size()
        n = c.fwrite(buffer.data(), 1, buffer.size(), file)
        if n == 0
          break
        buffer.consume(n)
      if c.ferror(file) == 0
        rt = 0
      else
        rt = -1
      Buffer.push buffer
      c.fclose(file)
    rt

  @syncWriteBuffer: (path, buffer) ->
    rt = -1
    file = c.fopen(path, "w")
    if file?
      buffer.save()
      while buffer.size()
        n = c.fwrite(buffer.data(), 1, buffer.size(), file)
        if n == 0
          break
        buffer.consume(n)
      buffer.restore()
      if c.ferror(file) == 0
        rt = 0
      else
        rt = -1
      c.fclose(file)
    rt

  @join: ->
    ([].slice.call arguments, 0).join("/")

  @exists: (path)->
    c.access(path, c.F_OK) == 0

  @readable: (path) ->
    c.access(path, c.R_OK) == 0

  @writable: (path) ->
    c.access(path, c.W_OK) == 0

  @executable: (path) ->
    c.access(path, c.X_OK) == 0


IO.bind 'file', (path, mode) ->
  new File path, mode

global.File = File
