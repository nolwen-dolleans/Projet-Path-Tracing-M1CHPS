#!/bin/zsh


WIDTH=1000
HEIGHT=1000


for I in {2..11}; do

SAMPLES=$((2 ** I))
echo $SAMPLES
./build/ppm 32 $WIDTH $HEIGHT $SAMPLES

done


./build/ppm 32 $WIDTH $HEIGHT $((2 ** 14))
#cd ..
#START=`date +%s%N`
#END=`date +%s%N`
#cd performance
