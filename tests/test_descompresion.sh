#! /bin/sh
FILE=$1
PERCENTAGE=${2:-10}
make clean
make
./fft -p $PERCENTAGE -c $FILE 
./fft -d compressed_image.imgc 
./tests/scripts/show_original.py $FILE &
./tests/scripts/show_original.py decompressed_image.img &

