#!/bin/bash
date=`date +%Y-%m-%d__%H:%M:%S`;

if [[ $# -ne 1 ]]; then
    exit 1
fi
DyAver=$1;

MemValue=(`free -m | head -n 2 | tail -n 1 | awk '{printf("%d %d", $3, $2)}'`)

MemLeft=`echo "${MemValue[1]}-${MemValue[0]}" | bc`

MemAvaPrec=`echo "scale=1;${MemValue[0]}*100/${MemValue[1]}" | bc`

DyAver=`echo "scale=1;${DyAver}*0.3+${MemAvaPrec}*0.7" | bc`


echo "${date} ${MemValue[1]}M ${MemLeft}M ${MemAvaPrec}% ${DyAver}%"
