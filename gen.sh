#!/bin/bash

n=1
last=5

while [ $n -lt $last ]

do
  cp a.txt .txt
  n=$(($n+1))

done
