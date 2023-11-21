#define _USE_MATH_DEFINES
#define BUF_SIZE 2048
#include<iostream>
#include<cstring>
#include<complex>
#include<cmath>
#include<unistd.h>
#include<fstream>
#include"fft.h"
using namespace std;
void forward_or_inverse_fft(short int forward_inverse_flag);
void compression_fft(char* image_path);
void decompression_fft(char* image_path); 
unsigned int round_to_power2(unsigned int n); // rounds to closer power of two
int main(int argc, char** argv) {
	short int forward_inverse_flag = 0; // Flag to check whether were doing FFT or IFFT
	int c;
	while((c = getopt(argc, argv, "fic:d:")) != -1) {
		switch(c) {
			case 'f': // Forward one dimensional FFT 
				forward_inverse_flag = 0;
				forward_or_inverse_fft(forward_inverse_flag);
				break;
			case 'i': // Inverse one dimensional FFT
				forward_inverse_flag = 1;
				forward_or_inverse_fft(forward_inverse_flag);
				break;
			case 'c': // Compression two dimensional FFT
				compression_fft(optarg);
				break;
			case 'd': // Decompression two dimensional FFT
				decompression_fft(optarg);
			default:
				break;
		}
	}
	return 0;
}
// rounds to closer power of two
unsigned int round_to_power2(unsigned int n) {
	unsigned int closer = 1;
	while(n >= closer) {
		closer <<= 1;
	}
	return closer;
}
void decompression_fft(char* image_path) {
	ifstream img_stream(image_path, ios_base::binary);
	unsigned int i = 0, j, k, size, bytes_read, width, height;
	if(!img_stream.is_open()) {
		cout << "File was not opened succesfully" << '\n' << image_path << endl;
		return;
	}
	img_stream.read((char*) &width, 4);
	img_stream.read((char*) &height, 4);
	img_stream.read((char*) &size, 4);
	complex<float> coefficients[size][size];
	float temp[2*size*size];
	unsigned char buffer[BUF_SIZE];
	while(!img_stream.eof()) {
		if(img_stream.good()) {
			img_stream.read((char*) buffer, BUF_SIZE);
			bytes_read = img_stream.gcount();
			cout << bytes_read << '\n';
			for(j = 0, k = 0; j < bytes_read; j += 4, k++) {
				temp[i + k] = *((float*) &buffer[j]);
			}
			i += bytes_read/4;
		}
		else {
			cout << "Error" << '\n';
			break;
		}
	}
	k = 0;
	for(i = 0; i < size; i++) {
		for(j = 0; j < size; j++) {
			coefficients[i][j] = temp[k] + 1j*temp[k+1];
			k += 2;
		}
	}
	cout << "[[";
	for(i = 0; i < size; i++) {
		for(j = 0; j < size; j++) {
			if(j != (size - 1))
				cout << abs(coefficients[i][j]) << ',';
			else
				cout << abs(coefficients[i][j]);

		}
		if(i != (size - 1))
			cout << "],[";
		else
			cout << "]";
	}
	cout << "]";
}
void compression_fft(char* image_path) {
	ifstream img_stream(image_path, ios_base::binary);
	unsigned int i, j, size, bytes_read, width, height, padded_width, padded_height;
	if(!img_stream.is_open()) {
		cout << "File was not opened succesfully" << '\n' << image_path << endl;
		return;
	}
	img_stream.read((char*) &width, 4);
	img_stream.read((char*) &height, 4);
	// leer imagen
	padded_width = round_to_power2(width);
	padded_height = round_to_power2(height);
	size = width*height;
	unsigned char img[size];
	unsigned char buffer[BUF_SIZE];
	i = 0;
	while(!img_stream.eof()) {
		if(img_stream.good()) {
			img_stream.read((char*) buffer, BUF_SIZE);
			bytes_read = img_stream.gcount();
			for(j = 0; j < bytes_read; j++) {
				img[i + j] = buffer[j];
			}
			i += bytes_read;
		}
		else {
			cout << "Error" << '\n';
			break;
		}
	}
	img_stream.close();
	// ZERO-PAD THE IMAGE AND MAKE COEFFICIENT MATRIX
	complex<float> coefficients[padded_width][padded_height];
	complex<float> output[padded_width][padded_height];
	complex<float> output_aux[padded_width][padded_height];
	complex<float> temp_coef[padded_width];
	complex<float> temp_out[padded_width];
	for(i = 0; i < padded_width; i++) {
		if(i >= width) {
			memset(&coefficients[i], 0, padded_height);
			continue;
		}
		for(j = 0; j < padded_height; j++) {
			if(j >= height)
				coefficients[i][j] = 0;
			else
				coefficients[i][j] = (float) img[i*height + j];
		}
	}
	for(i = 0; i < padded_width; i++) {
		fft(coefficients[i], padded_height, output[i]);
	}
	for(i = 0; i < padded_height; i++) {
		for(j = 0; j < padded_width; j++) {
			temp_coef[j] = output[j][i];
		}
		fft(temp_coef, padded_width, temp_out);
		for(j = 0; j < padded_width; j++) {
			output[j][i] = temp_out[j];
		}
	}
	unsigned int r = 0, m = 0;
	for(i = padded_width/2; i < padded_width; i++) {
		m = 0;
		if(i == padded_width - 1) i = 0;
		if(i == padded_width/2 - 1) break;
		for(j = padded_height/2; j < padded_height; j++) {
			if(j == padded_height - 1) j = 0;
			if(j == padded_height/2 - 1) break;
			output_aux[r][m] = output[i][j];
			m++;
		}
		r++;

	}
	// recortar un cuadrado con el 10% de los coeficientes
	float porcentaje = 0.1f;
	unsigned int length = ceil(sqrt(width*height*porcentaje));
	unsigned int inicio_width = padded_width/2 - length/2;
	unsigned int fin_width = padded_width/2 + length/2;
	unsigned int inicio_height= padded_height/2 - length/2;
	unsigned int fin_height = padded_height/2 + length/2;
	char compressed_image_path[] = "imagen_comprimida.imgc";
	ofstream img_stream_output(compressed_image_path, ofstream::binary);
	if(!img_stream_output.is_open()) {
		cout << "File was not created succesfully" << compressed_image_path << '\n';
		return;
	}
	// lo primero que va ir va a ser el tamanio de la imagen original
	img_stream_output.write(reinterpret_cast<const char*>(&width), sizeof(width));
	img_stream_output.write(reinterpret_cast<const char*>(&height), sizeof(height));
	// despues el tamanio de la imagen comprimida, siempre va a ser un cuadrado
	img_stream_output.write(reinterpret_cast<const char*>(&length), sizeof(length));
	for(i = inicio_width; i < fin_width; i++) {
		for(j = inicio_height; j < fin_height; j++) {
			float r = output_aux[i][j].real();
			float im = output_aux[i][j].imag();
			img_stream_output.write(reinterpret_cast<const char*>(&r), sizeof(float));
			img_stream_output.write(reinterpret_cast<const char*>(&im), sizeof(float));
		}
	}
}
void forward_or_inverse_fft(short int forward_inverse_flag) {
	unsigned long long int n, i; // NUMBER OF COEFFICIENTS, MUST BE AN INTEGER 2^n
	float real;
	float im;
	cin >> n;
	complex<float> coefficients[n];
	complex<float> out[n] = {0.0f + 0if};
	for(i = 0; i < n; i++) {
		cin >> real;
		cin >> im;
		coefficients[i] = real + 1if*im;
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
	return;
}
