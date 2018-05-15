#!/bin/bash
beginTime=$(date +%s%N)
# 여기에 작업 내용을 구현한다 
    sleep 1 
    endTime=$(date +%s%N) 
    elapsed=`echo "($endTime - $beginTime) / 1000000" | bc` 
    elapsedSec=`echo "scale=6;$elapsed / 1000" | bc | awk '{printf "%.6f", $1}'` 
    echo TOTAL: $elapsedSec sec >> time.txt

