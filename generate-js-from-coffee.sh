#!/bin/sh


if [ "$1" == "" ]; then
  watch=''
  cmd='cb'
else
  watch='&'
  cmd='cwb'
fi

for name in modules/core modules/net benchmark examples test
do
  eval coffee -$cmd -o $name $name/*.coffee $watch
done

if [ "$1" != "" ]; then
  while :;
  do
    sleep 60
  done
fi
