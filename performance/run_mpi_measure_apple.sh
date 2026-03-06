#!/bin/zsh
WIDTH=800
HEIGHT=600
SAMPLES=1000
MAX=1

OUTPUT="performance/measures/runtime_by_samplings.csv"

for J in $(seq 1 10); do
for I in $(seq 1 $MAX); do

echo "run $I of $MAX with $J MPI processes and 26 bounces"

mpirun -n $J ./build/ppm $WIDTH $HEIGHT $SAMPLES

done

echo "" >> "$OUTPUT"

done
