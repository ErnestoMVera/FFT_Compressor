#define _USE_MATH_DEFINES
#define BUF_SIZE 2048
#include<iostream>
#include<cstring>
#include<complex>
#include<cmath>
#include<unistd.h>
#include<fstream>
#include<algorithm>
#include"fft.h"
using namespace std;
void forward_or_inverse_fft(short int forward_inverse_flag);
void compression_fft(char* image_path, float percentage);
void decompression_fft(char* image_path); 
unsigned int round_to_power2(unsigned int n); // rounds to closer power of two
void free_matrix(complex<float>** &matrix, unsigned int n);
void allocate_matrix(complex<float>** &matrix, unsigned int n, unsigned int m);
int main(int argc, char** argv) {
	short int forward_inverse_flag = 0; // Flag to check whether were doing FFT or IFFT
	int c;
	float percentage = 0.1f;
	while((c = getopt(argc, argv, "fip:c:d:")) != -1) {
		switch(c) {
			case 'f': // Forward one dimensional FFT 
				forward_inverse_flag = 0;
				forward_or_inverse_fft(forward_inverse_flag);
				break;
			case 'i': // Inverse one dimensional FFT
				forward_inverse_flag = 1;
				forward_or_inverse_fft(forward_inverse_flag);
				break;
			case 'p': // compression percentage 10% is the default
				percentage = (float) atof(optarg)/100.0f;
				break;
			case 'c': // Compression two dimensional FFT
				compression_fft(optarg, percentage);
				break;
			case 'd': // Decompression two dimensional FFT
				decompression_fft(optarg);
				break;
			default:
				break;
		}
	}
	return 0;
}
// rounds to closer power of two
unsigned int round_to_power2(unsigned int n) {
	unsigned int closer = 1;
	while(n > closer) {
		closer <<= 1;
	}
	return closer;
}
void free_matrix(complex<float>** &matrix, unsigned int n) {
	unsigned int i;
	for(i = 0; i < n; i++) {
		delete[] matrix[i];
	}
	delete[] matrix;
}
void allocate_matrix(complex<float>** &matrix, unsigned int n, unsigned m) {
	unsigned int i;
	matrix = new complex<float>*[n];
	for(i = 0; i < n; i++) {
		matrix[i] = new complex<float>[m]();
	}
}
void decompression_fft(char* image_path) {
	unsigned int i, j, k, size, bytes_read, width, height;
	unsigned int *indices;
	unsigned char current;
	unsigned int padded_width, padded_height;
	complex<float>** coefficients, **descomprimida;
	float* real, *im;
	unsigned char buffer[BUF_SIZE];
	ifstream img_stream(image_path, ios_base::binary);
	if(!img_stream.is_open()) {
		cout << "File was not opened succesfully" << '\n' << image_path << endl;
		return;
	}
	img_stream.read((char*) &width, 4);
	img_stream.read((char*) &height, 4);
	img_stream.read((char*) &size, 4);
	padded_width = round_to_power2(width);
	padded_height = round_to_power2(height);
	k = 0, i = 0;
	real = new float[size];
	im = new float[size];
	indices = new unsigned int[size];
	while(!img_stream.eof()) {
		if(!img_stream.good()) {
			cout << "Error" << '\n';
			break;
		}
		img_stream.read((char*) buffer, BUF_SIZE);
		bytes_read = img_stream.gcount();
		for(j = 0; j < bytes_read; j += 4) {
			// esta linea convierte conjuntos de 4 bytes de unsigned char a floats de 4 bytes
			if(k % 3 == 0) {
				indices[i] = *((unsigned int*) &buffer[j]);
			}
			else if(k % 3 == 1) {
				real[i] = *((float*) &buffer[j]);
			}
			else if(k % 3 == 2) {
				im[i] = *((float*) &buffer[j]);
			}
			if(k % 3 == 2) i++;
			k++;
		}
	}
	img_stream.close();
	allocate_matrix(coefficients, padded_height, padded_width);
	k = 0;
	for(i = 0; i < padded_height; i++) {
		for(j = 0; j < padded_width; j++) {
			if(indices[k] == i*padded_width + j) {
				coefficients[i][j] = real[k] + 1j*im[k];
				k++;
			}
			else {
				coefficients[i][j] = 0.0f; 
			}
		}
	}
	delete[] real;
	delete[] im;
	delete[] indices;
	allocate_matrix(descomprimida, padded_height, padded_width);
	ifft2(coefficients, padded_height, padded_width, descomprimida);
	free_matrix(coefficients, padded_height);
	char decompressed_image_path[] = "imagen_descomprimida.img";
	ofstream img_stream_output(decompressed_image_path, ios_base::binary);
	if(!img_stream_output.is_open()) {
		cout << "File was not created succesfully" << decompressed_image_path << '\n';
		return;
	}
	// lo primero que va ir va a ser el tamanio de la imagen original
	img_stream_output.write(reinterpret_cast<const char*>(&width), sizeof(width));
	img_stream_output.write(reinterpret_cast<const char*>(&height), sizeof(height));
	// despues el tamanio de la imagen comprimida, siempre va a ser un cuadrado
	//img_stream_output.write(reinterpret_cast<const char*>(&length), sizeof(length));
	for(i = 0; i < padded_height; i++) {
		if(i >= height) break;
		for(j = 0; j < padded_width; j++) {
			if(j >= width) break;
			current = (unsigned int) abs(descomprimida[i][j]);
			img_stream_output.write(reinterpret_cast<const char*>(&current), sizeof(unsigned char));
		}
	}
	free_matrix(descomprimida, padded_height);
}
void compression_fft(char* image_path, float percentage) {
	unsigned char buffer[BUF_SIZE];
	unsigned char* img;
	unsigned int i, j, k, index, size, bytes_read, width, height, padded_width, padded_height;
	complex<float> **coefficients, **output;
	float threshold, *ordered;
	float real, im;
	ifstream img_stream(image_path, ios_base::binary);
	if(!img_stream.is_open()) {
		cout << "File was not opened succesfully" << '\n' << image_path << endl;
		return;
	}
	img_stream.read((char*) &width, 4);
	img_stream.read((char*) &height, 4);
	size = width*height;
	img = new unsigned char[size];
	// leer imagen
	padded_width = round_to_power2(width);
	padded_height = round_to_power2(height);
	i = 0;
	while(!img_stream.eof()) {
		if(img_stream.good()) {
			img_stream.read((char*) buffer, BUF_SIZE);
			bytes_read = img_stream.gcount();
			for(j = 0; j < bytes_read; j++) {
				if(i + j > size - 1) {
					break;
				}
				img[i + j] = buffer[j];
			}
			i += bytes_read;
			if(i > size - 1) {
				break;
			}
		}
		else {
			cout << "Error" << '\n';
			break;
		}
	}
	img_stream.close();
	// ZERO-PAD THE IMAGE AND MAKE COEFFICIENT MATRIX
	allocate_matrix(coefficients, padded_height, padded_width);
	for(i = 0; i < padded_height; i++) {
		if(i >= height) {
			for(j = 0; j < padded_width; j++) {
				coefficients[i][j] = 0;
			}
			continue;
		}
		for(j = 0; j < padded_width; j++) {
			if(j >= width) {
				coefficients[i][j] = 0;
			}
			else {
				coefficients[i][j] = (float) img[i*width + j];
			}
		}
	}
	delete[] img;
	allocate_matrix(output, padded_height, padded_width);
	// FFT the image 
	fft2(coefficients, padded_height, padded_width, output);
	free_matrix(coefficients, padded_height);
	ordered = new float[padded_height*padded_width];
	k = 0;
	for(i = 0; i < padded_height; i++) {
		for(j = 0; j < padded_width; j++) {
			ordered[k] = abs(output[i][j]);
			k++;	
		}
	}
	sort(ordered, ordered + padded_height*padded_height);
	index = (unsigned int) ((padded_width*padded_height - 1)*(1 - percentage));
	threshold = ordered[index];
	delete[] ordered;
	char compressed_image_path[] = "imagen_comprimida.imgc";
	ofstream img_stream_output(compressed_image_path, ofstream::binary);
	if(!img_stream_output.is_open()) {
		cout << "File was not created succesfully" << compressed_image_path << '\n';
		return;
	}
	// original image Width x Height
	img_stream_output.write(reinterpret_cast<const char*>(&width), sizeof(unsigned int));
	img_stream_output.write(reinterpret_cast<const char*>(&height), sizeof(unsigned int));
	// number of coeff to be written
	index = (padded_width*padded_height) - index;
	img_stream_output.write(reinterpret_cast<const char*>(&index), sizeof(unsigned int));
	k = 0;
	for(i = 0; i < padded_height; i++) {
		for(j = 0; j < padded_width; j++) {
			if(abs(output[i][j]) >= threshold) {
				real = output[i][j].real();
				im = output[i][j].imag();
				img_stream_output.write(reinterpret_cast<const char*>(&k), sizeof(unsigned int));
				img_stream_output.write(reinterpret_cast<const char*>(&real), sizeof(float));
				img_stream_output.write(reinterpret_cast<const char*>(&im), sizeof(float));
			}
			k++;
		}
	}
	img_stream_output.close();
	free_matrix(output, padded_height);
}
void forward_or_inverse_fft(short int forward_inverse_flag) {
	unsigned long long int n, i; // NUMBER OF COEFFICIENTS, MUST BE AN INTEGER 2^n
	float real;
	float im;
	cin >> n;
	complex<float>* coefficients;
	complex<float>* out;
	coefficients = new complex<float>[n];
	out = new complex<float>[n];
	for(i = 0; i < n; i++) {
		cin >> real;
		cin >> im;
		coefficients[i] = real + 1if*im;
		out[i] = 0.0f;
	}
	if(forward_inverse_flag == 0)
		fft(coefficients, n, out);
	else
		ifft(coefficients, n, out);
	cout << n << '\n';
	for(i = 0; i < n - 1; i++) {
		cout << out[i].real() << " " << out[i].imag() << '\n';
	}
	cout << out[n - 1].real() << " " << out[i].imag() << '\n';
	cout << "[";
	for(i = 0; i < n - 1; i++) {
		cout << abs(out[i]) << ", ";
	}
	cout << abs(out[n - 1]);
	cout << "]";
	delete[] coefficients;
	delete[] out;
	return;
}
