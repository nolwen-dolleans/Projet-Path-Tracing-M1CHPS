#!/bin/zsh


WIDTH=500
HEIGHT=500
SAMPLES=200


for IMGSIZE in "24" "24ptr" "32";do
cd ..
START=`date +%s%N`
./build/ppm $IMGSIZE $WIDTH $HEIGHT $SAMPLES
END=`date +%s%N`
cd performance
EXE_TIME=$(($END - $START))

echo "Time (s)\n $EXE_TIME" >> "measures/time_$IMGSIZE _WIDTH=$WIDTH HEIGHT=$HEIGHT SAMPLES=$SAMPLES"

done
rm -rf tmp.csv
