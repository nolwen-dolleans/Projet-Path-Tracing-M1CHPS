#!/bin/zsh


WIDTH=1000
HEIGHT=1000


for I in {12..15}; do

SAMPLES=$((2 ** I))
echo $SAMPLES
./build/ppm 32 $WIDTH $HEIGHT $SAMPLES

done


#cd ..
#START=`date +%s%N`
#END=`date +%s%N`
#cd performance
