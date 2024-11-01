#!/usr/bin/env sh

mkdir -p results

for i in $(seq 1 1000); do
    printf "Running with %d threads..." $i
    time -f "%e" -o results/tmp ./build/release/pdi-numbers -e 1000000 -t $i >/dev/null
    cat results/tmp >> results/time.dat
    printf "done\n"
done

rm results/tmp
