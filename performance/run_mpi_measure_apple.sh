#!/bin/zsh
WIDTH=800
HEIGHT=600
SAMPLES=1000
_MPI=10
_OMP=16




for K in $(seq 1 $_OMP); do

export OMP_NUM_THREADS=$K

for I in $(seq 1 $_MPI); do
for J in $(seq 1 3); do

echo "Working with $I MPI thread and $K OMP threads"
mpirun -n $I ./build/ppm $WIDTH $HEIGHT $SAMPLES 25 no_image

done

done

done
