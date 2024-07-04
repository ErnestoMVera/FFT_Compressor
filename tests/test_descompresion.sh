#! /bin/sh
make clean
make
./fft -c tests/img_test1.img
./fft -d imagen_comprimida.imgc > out_d.py
