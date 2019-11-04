#!/bin/bash

n=1
last=0

while [ $n -lt $last ]

do
  rm upload$n.txt
  rm down$n.txt
  rm down$n.txtdownload.txt
  rm upload$n.txtcopied.txt
  n=$(($n+1))

done
