#!/bin/bash

n=1
last=20

while [ $n -lt $last ]

do
  ./TigerC <<< "tconnect 127.0.0.1 brian pass
  exit" &
  n=$(($n+1))

done
