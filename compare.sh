#!/bin/bash

n=1
last=100

while [ $n -lt $last ]

do
  diff down$n.txtdownload.txt upload$n.txtcopied.txt
  n=$(($n+1))

done
