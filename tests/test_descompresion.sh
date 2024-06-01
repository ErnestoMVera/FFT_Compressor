#! /bin/sh
make clean
make
./fft.exe -c tests/img_bin3.img
./fft.exe -d imagen_comprimida.imgc > out_d.py
