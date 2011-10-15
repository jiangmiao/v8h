#!/bin/sh


if [ "$1" == "" ]; then
  watch=''
  cmd='cb'
else
  watch='&'
  cmd='cwb'
fi

for name in core net
do
  eval coffee -$cmd -o bin/modules/$name modules/$name/*.coffee $watch
done

for name in benchmark examples test
do
  eval coffee -$cmd -o bin/$name $name/*.coffee $watch
done

if [ "$1" != "" ]; then
  while :;
  do
    sleep 60
  done
fi
