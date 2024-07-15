# FFT Signal processor and Image Compressor
This is my own implementation of the FFT algorithm and a built-in application of it as an image compressor.
## Build and installation
The program only depends on the C++ standard libraries and it comes with a makefile to build it.
Command to build it:
```bash
make
```
## How to use it.
### Image Compression
The package comes with a python script which converts images to the format the application uses.
```bash
./tests/scripts/generate_raw.py input_image.jpg generated_image.img
```
To compress the image use the following command.
```bash
./fft -c generated_image.img
```
The command will generate the file with the compressed file (compressed\_image.imgc).
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
The next steps is input the size of the signal (it must be a power of two) and the a pair of complex coefficcients which form the input signal.
For example an eight coefficients rectangular signal:
```
8
1 0
1 0
1 0
1 0
0 0
0 0
0 0
0 0
```
Note: if the length of the signal isn't a power of two it can always be zero-padded.
### Use files as input
The input can also be in the form of a file using input redirection.
(File contains the input in the same format)
```bash
./fft -f < signal_file
```
or
```bash
./fft -i < signal_file
```
