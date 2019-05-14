#!/bin/bash

max=0
num=1

function findinfile() {
    for i in `cat $1 | tr -c "a-zA-Z" "\n" | grep [^$] `
    do
        cnt=${#i}
        if [[ max -lt cnt ]]; then
            max=${cnt}
            maxstr=$i
        fi 
        echo No.${num} max=$max maxstr=$maxstr
        num=$[${num}+1]
    done
}

function findindir() {
    cwd=$1
    echo cwd = $cwd
    #echo `ls ${cwd}`
    for i in `ls $1` 
    do
        if [[ -f ${cwd}${i} ]]; then
            echo "file:"
            
            echo ${cwd}${i}:
            findinfile ${cwd}${i}   
        elif [[ -d ${cwd}${i} ]]; then
            echo "dir:"
            echo ${cwd}${i}
        findindir "${cwd}${i}/"
    else
        echo ${cwd}${i}
        echo "oops!"

        fi
    done
    cwd="$cwd../"
}


findindir $1
echo "一共搜寻了 $num 个文件，其中长度最长的字符串为 $maxstr ,长度为 $max 。"

#findindir "/home/zip/haizei/3.SystemProgramming/0.Homework/testdir/"
