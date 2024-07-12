#! /bin/sh
make clean
make
./fft -c $1 
./fft -d imagen_comprimida.imgc 
./tests/scripts/show_original.py $1 &
./tests/scripts/show_process.py imagen_descomprimida.img &

