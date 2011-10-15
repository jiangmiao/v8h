v8h.extend System,
  raise: (message)->
    throw new Error(message)

  raiseSystem: (message)->
    throw new Error(message + " : " + System.getLastError())
