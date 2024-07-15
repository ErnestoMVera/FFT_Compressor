#! /bin/sh
make clean
make
./fft -p $2 -c $1 
./fft -d imagen_comprimida.imgc 
./tests/scripts/show_original.py $1 &
./tests/scripts/show_original.py imagen_descomprimida.img &

