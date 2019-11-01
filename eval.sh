#!/bin/bash

n=1
last=20

while [ $n -lt $last ]

do
  ./TigerC <<< "tconnect 127.0.0.1 brian pass"  &
  #tget down$n.txt
  #tput upload$n.txt
  #exit“ &
  sleep 3
  n=$(($n+1))

done
