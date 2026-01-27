#!/bin/zsh


WIDTH=800
HEIGHT=600
MAX=4000
NTIMES=5


for (( I=1600; I<=MAX; I+=200 ))
do

for j in {1..$NTIMES}; do

echo $j eme de $I
./build/ppm_mesures 32 $WIDTH $HEIGHT $I
done

done
