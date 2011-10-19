#!/bin/sh


if [ "$1" == "" ]; then
  watch=''
  cmd='c'
else
  watch='&'
  cmd='cw'
fi

for name in modules/core modules/net benchmark examples test
do
  eval toffee -$cmd -o $name $name/*.toffee $watch
done

if [ "$1" != "" ]; then
  while :;
  do
    sleep 60
  done
fi
