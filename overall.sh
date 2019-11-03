#!/bin/bash

n=1
last=100

while [ $n -lt $last ]

do
  cp textlarge.txt upload$n.txt
  cp textlarge.txt down$n.txt
  rm down$n.txtdownload.txt
  rm upload$n.txtcopied.txt
  n=$(($n+1))

done


n=1

while [ $n -lt $last ]

do
  ./TigerC <<< "tconnect 127.0.0.1 user pass
  tget down$n.txt
  tput upload$n.txt
  exit" &
  n=$(($n+1))
  echo $n

done

n=1


while [ $n -lt $last ]

do
  diff down$n.txtdownload.txt upload$n.txtcopied.txt
  n=$(($n+1))

done
