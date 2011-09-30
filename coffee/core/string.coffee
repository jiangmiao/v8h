String::extend
  capitalize: ->
    @replace /^[a-z]/, (m) ->
      m.upperCase()
