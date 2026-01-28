#!/bin/zsh


WIDTH=800
HEIGHT=600
MAX=4000
NTIMES=5



for (( I=25; I<=45; I+=5 ))
do

for j in {1..$NTIMES}; do

echo $j eme de $I
BOUNCES=26 ./build/ppm 32 $WIDTH $HEIGHT $I
done

done

for (( I=50; I<=150; I+=50 ))
do

for j in {1..$NTIMES}; do

echo $j eme de $I
BOUNCES=26 ./build/ppm 32 $WIDTH $HEIGHT $I
done

done

for (( I=200; I<=$MAX; I+=200 ))
do

for j in {1..$NTIMES}; do

echo $j eme de $I
BOUNCES=26 ./build/ppm 32 $WIDTH $HEIGHT $I
done

done

for (( I=200; I<=MAX; I+=200 ))
do

for j in {1..$NTIMES}; do

echo $j eme de $I
BOUNCES=26 ./build/ppm 32 $WIDTH $HEIGHT $I
done

done
