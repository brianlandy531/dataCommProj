#!/bin/bash

n=1
last=2

while [ $n -lt $last ]

do
  cp textlarge.txt upload$n.txt
  cp textlarge.txt down$n.txt
  rm down$n.txtdownload.txt
  rm upload$n.txtcopied.txt
  n=$(($n+1))

done