#!/bin/sh
for scoreType in 1 2;
do
for L in 30 100 150;
do
for N in 10 20;
do
./main $L $N $scoreType
./main $L $N $scoreType
./main $L $N $scoreType
done
done
done