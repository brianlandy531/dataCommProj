#!/bin/bash

n=1
last=5

while [ $n -lt $last ]

do
  ./TigerC <<< "tconnect 127.0.0.1 user pass
  tget down$n.txt
  tput upload$n.txt
  exit" &
  n=$(($n+1))
  echo $n

done
