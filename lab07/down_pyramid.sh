#!/bin/bash

read n

if [ $n -lt 0 ]; then
    exit 1
elif [ $n -gt 12 ]; then
    echo "bigger than 12"
    exit 1
else

    for ((i=$n; i>0; i--)) 
        do
        for ((j=0; j<$n-i; j++))
        do
            echo -n " "
        done
        for ((j=0; j<2*i-1; j++))
        do
            echo -n "*"
        done
        echo ""
        done
fi 