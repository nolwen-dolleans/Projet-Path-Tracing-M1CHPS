#!/bin/zsh


WIDTH=800
HEIGHT=600
SAMPLES=1000
RUNTIME=0

for I in {1..3}; do

START=`date +%s%N`
./build/ppm 32 $WIDTH $HEIGHT $SAMPLES
END=`date +%s%N`


RUNTIME=$((RUNTIME+$((END-START))))

done

RUNTIME=$((RUNTIME/3e9))

echo $RUNTIME


#cd ..
#START=`date +%s%N`
#END=`date +%s%N`
#cd performance
