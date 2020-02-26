#!/bin/sh
for K in 1000 1500 2000 2500 3000 3500 4000 4500 5000;
do
for O in 100 150 200 250 300 350 400 450 500;
do
  for species in CHIMP DOG MOUSE;
  do
    echo aligning HUMAN and $species for K=$K O=$O...
    ./blastz HUMAN $species K=$K O=$O \
    | ./lav2maf /dev/stdin HUMAN $species \
    | ./single_cov2 /dev/stdin R=HUMAN \
    | ./maf_project /dev/stdin HUMAN \
    > HUMAN.$species.L$L.O$O.maf
    ../prog3 HUMAN.$species.align.true.fa HUMAN.$species.L$L.O$O.maf 5
    ../prog3 HUMAN.$species.align.true.fa HUMAN.$species.L$L.O$O.maf 0
    echo
  done
done
done
