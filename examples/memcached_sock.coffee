# Run memcached: memcached -s /tmp/memcached.sock"
headerDelim = new Buffer("\r\n")

message = new Buffer '<!DOCTYPE html><html><head><meta charset="utf-8"/><title>快漫</title><link href="/css/a.css?1305006217" rel="stylesheet" type="text/css"/></head><body><div class="nav"><div class="passport"><a href="/passport/login">登录</a><a href="/passport/register">注册</a></div><div class="menu"><a href="/">快漫</a><a href="/tops">动漫排行榜</a><a href="/animes">动画列表</a></div></div><div class="l2 root"><div class="sidebar"><h3 class="top">最新更新a</h3><h4>4月12日</h4><ul><li><a href="/animes/0">日常</a></li><li><a href="/animes/1">DOG DAYS</a></li><li><a href="/animes/2">魔法少女小圆</a></li><li><a href="/animes/3">无名</a></li><li><a href="/animes/4">圣痕炼金士</a></li><li><a href="/animes/5">银魂</a></li><li><a href="/animes/6">圣痕</a></li></ul><h4>4月12日</h4><ul><li><a href="/animes/0">日常</a></li><li><a href="/animes/1">DOG DAYS</a></li><li><a href="/animes/2">魔法少女小圆</a></li><li><a href="/animes/3">无名</a></li><li><a href="/animes/4">圣痕炼金士</a></li><li><a href="/animes/5">银魂</a></li><li><a href="/animes/6">圣痕</a></li></ul><h4>4月12日</h4><ul><li><a href="/animes/0">日常</a></li><li><a href="/animes/1">DOG DAYS</a></li><li><a href="/animes/2">魔法少女小圆</a></li><li><a href="/animes/3">无名</a></li><li><a href="/animes/4">圣痕炼金士</a></li><li><a href="/animes/5">银魂</a></li><li><a href="/animes/6">圣痕</a></li></ul><h4>4月12日</h4><ul><li><a href="/animes/0">日常</a></li><li><a href="/animes/1">DOG DAYS</a></li><li><a href="/animes/2">魔法少女小圆</a></li><li><a href="/animes/3">无名</a></li><li><a href="/animes/4">圣痕炼金士</a></li><li><a href="/animes/5">银魂</a></li><li><a href="/animes/6">圣痕</a></li></ul><h4>4月12日</h4><ul><li><a href="/animes/0">日常</a></li><li><a href="/animes/1">DOG DAYS</a></li><li><a href="/animes/2">魔法少女小圆</a></li><li><a href="/animes/3">无名</a></li><li><a href="/animes/4">圣痕炼金士</a></li><li><a href="/animes/5">银魂</a></li><li><a href="/animes/6">圣痕</a></li></ul><h4>4月12日</h4><ul><li><a href="/animes/0">日常</a></li><li><a href="/animes/1">DOG DAYS</a></li><li><a href="/animes/2">魔法少女小圆</a></li><li><a href="/animes/3">无名</a></li><li><a href="/animes/4">圣痕炼金士</a></li><li><a href="/animes/5">银魂</a></li><li><a href="/animes/6">圣痕</a></li></ul><h4>4月12日</h4><ul><li><a href="/animes/0">日常</a></li><li><a href="/animes/1">DOG DAYS</a></li><li><a href="/animes/2">魔法少女小圆</a></li><li><a href="/animes/3">无名</a></li><li><a href="/animes/4">圣痕炼金士</a></li><li><a href="/animes/5">银魂</a></li><li><a href="/animes/6">圣痕</a></li></ul></div><div class="context"><h3 class="new">四月新番</h3><ul class="posters_list"></ul></div></div><div style="clear: both">1317489517</div><div class="footer">&copy;coryright 2011 kuaiman.cc</div></body></html>'

text = message.readUtf8()
st = Date.now()
text = text.replace /ul/g, ->
  'aaa'
puts Date.now() - st

b = new Buffer()
while b.size() < 100 * 1024
  b.writeBuffer(message)
message = b

Socket.connect "unix:/tmp/memcached.sock", (client) ->
  if !client?
    System.raise "connect failed"

  close = ->
    client.close()

  client.error ->
    puts "ERROR"
    close()

  times = 1e3
  st = Date.now()
  do storeOnce = ->
    buffer = Buffer.pop()
    buffer.writeUtf8 "set hello 0 3600 #{message.size()}\r\n"
    buffer.writeBuffer message
    buffer.writeUtf8 "\r\n"
    client.writeBuffer buffer, ->
      client.readToken buffer, headerDelim, (n) ->
        --times
        if times
          Buffer.push buffer
          storeOnce()
        else
          puts 1e3 * 1000 / (Date.now() - st) + "/s"
          benchmarkGet()

  benchmarkGet = ->
    times = 1e3
    st = Date.now()
    do getOnce = ->
      buffer = Buffer.pop()
      buffer.writeUtf8 "get hello\r\n"
      client.writeBuffer buffer, ->
        client.readToken buffer, headerDelim, (n) ->
          header = buffer.readUtf8(n)
          [type, key, flags, length] = header.split " "
          if type != 'VALUE'
            return close()
          length = parseInt(length)
          client.readSize buffer, length, (n) ->
            text = buffer.readUtf8(n)
            text = text.replace /ul/g, ->
              'aaa'
            --times
            if times
              Buffer.push buffer
              getOnce()
            else
              puts 1e3 * 1000 / (Date.now() - st) + "/s"
              close()

return
