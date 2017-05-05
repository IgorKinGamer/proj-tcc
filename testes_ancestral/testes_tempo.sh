#!/bin/bash

IFS=$'\n'
REPS=30

# Graus com os quais rodará
for g in $(cat graus_testes.txt)
do
    IFS=$'\n'
    for i in `seq ${REPS}`;
    do
        IFS=' '
        echo --- $i ---
        echo \> ./tmp $* - $g
        ./tmp $* - $g
    done
done
