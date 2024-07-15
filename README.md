# FFT Signal processor and Image Compressor
This is my own implementation of the FFT algorithm and a built-in application of it as an image compressor.
## Build and installation
The program only depends on the C++ standard libraries and it comes with a makefile to build it.
Command to build it:
```bash
make
```
## How to use it.
### FFT on regular signals.
To use the program to perform the FFT or the iFFT operations, use the program with the -f (forward FFT) and -i (inverse FFT) arguments.
The input to these modes is via the standard input.
```bash
./fft -f
```
or
```bash
./fft -i
```
