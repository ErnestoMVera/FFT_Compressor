#! /bin/sh
FILE=$1
PERCENTAGE=${2:-10}
make clean
make
./fft -p $PERCENTAGE -c $FILE 
./fft -d imagen_comprimida.imgc 
./tests/scripts/show_original.py $FILE &
./tests/scripts/show_original.py imagen_descomprimida.img &

