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
  eval coffee -$cmd -o js/$name coffee/$name/*.coffee $watch
done

for name in benchmark examples
do
  eval coffee -$cmd -o js/$name $name/*.coffee $watch
done

while :;
do
  sleep 60
done
