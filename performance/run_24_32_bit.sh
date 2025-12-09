#!/bin/zsh


WIDTH=500
HEIGHT=500
SAMPLES=1000


for IMGSIZE in "24" "24ptr" "32";do

./../build/ppm $IMGSIZE $WIDTH $HEIGHT $SAMPLES

done
