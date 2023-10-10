#define _USE_MATH_DEFINES
#include<iostream>
#include<complex>
#include<cmath>
#include<unistd.h>
#include"fft.h"
using namespace std;
int main(int argc, char** argv) {
	short int forward_inverse_flag = 0; // Flag to check whether were doing FFT or IFFT
	int c;
	while((c = getopt(argc, argv, "fi")) != -1) {
		switch(c) {
			case 'f':
				forward_inverse_flag = 0;
				break;
			case 'i':
				forward_inverse_flag = 1;
			default:
				break;
		}
	}
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
	return 0;
}
