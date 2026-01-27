#!/bin/zsh


WIDTH=1000
HEIGHT=1000
MAX=4000

echo "N Samples, Mean Runtime" > "measures/all_measures_$ALGO$T.csv"

for(J = 0..4); do
for (I=0 ; I<$MAX ; 400); do

SAMPLES=$((2 ** I))
echo $SAMPLES
./build/ppm 32 $WIDTH $HEIGHT $SAMPLES

done

done

#cd ..
#START=`date +%s%N`
#END=`date +%s%N`
#cd performance
