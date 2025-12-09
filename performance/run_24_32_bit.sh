#!/bin/zsh


WIDTH=500
HEIGHT=500
SAMPLES=200

if [ ! -e "measures" ]
then
mkdir measures
fi

for IMGSIZE in "24" "24ptr" "32";do
PATH_MEASURES="measures/time_$IMGSIZE: width=$WIDTH height=$HEIGHT samples=$SAMPLES"

for N in 1..3; do

cd ..
START=`date +%s%N`
./build/ppm $IMGSIZE $WIDTH $HEIGHT $SAMPLES
END=`date +%s%N`
cd performance

EXE_TIME=$((($END - $START)*0.000000001))

if [ -e $PATH_MEASURES ]
then
rm -rf $PATH_MEASURES
fi

echo "Time (s)\n $EXE_TIME" >> $PATH_MEASURES

done

done
