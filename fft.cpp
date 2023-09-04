#define _USE_MATH_DEFINES
#include<iostream>
#include<complex>
#include<cmath>
using namespace std;
void fft(complex<float>* coefficients, unsigned long long int n, unsigned long long int factor, complex<float>* xk, unsigned long long int* coeff_indices, complex<float>* out);
int main() {
	unsigned long long int n, i; // NUMBER OF COEFFICIENTS, MUST BE AN INTEGER 2^n
	float real;
	float im;
	cin >> n;
	unsigned long long indices[n];
	complex<float> coefficients[n];
	complex<float> xk[n];
	complex<float> out[n] = {0.0f + 0if};
	float x, y;
	float theta;
	// proceso para generar los xk.
	for(theta = 0.0f, i = 0; theta < 2*M_PI; theta+= 2*M_PI/n, i++) {
		x = cosf(theta);
		y = sinf(theta);
		xk[i] = x + 1if*y;
	}
	for(i = 0; i < n; i++) {
		cin >> real;
		cin >> im;
		coefficients[i] = real + 1if*im;
		indices[i] = i;
	}
	fft(coefficients, n, 1, xk, indices, out);
	cout << "[";
	for(i = 0; i < n; i++) {
		cout << abs(out[i]) << ", ";
	}
	cout << "]";
	return 0;
}
void fft(complex<float>* coefficients, unsigned long long int n, unsigned long long int factor, complex<float>* xk, unsigned long long int* coeff_indices, complex<float>* out) {
	unsigned long long int i, j, k, n_i = n/factor; // numero de elementos en el conjunto actual
	if(n_i == 1) {
		i = coeff_indices[0];
		out[i] = coefficients[i];
		return;
	}
	unsigned long long int coeff_pares[n_i/2];
	unsigned long long int coeff_impares[n_i/2];
	// DIVIDIR LOS PARES E IMPARES
	for(i = 0; i < n_i; i+=2) {
		coeff_pares[i/2] = coeff_indices[i];
		coeff_impares[i/2] = coeff_indices[i + 1];
	}
	complex<float> last_out[n] = {0.0f + 0if};
	fft(coefficients, n, factor*2, xk, coeff_pares, last_out);
	fft(coefficients, n, factor*2, xk, coeff_impares, last_out);
	unsigned long long int par, impar;
	// para todos los xk evaluar
	for(k = 0, i = 0; k < n; k += factor, i++) {
		j = coeff_indices[i];
		par = coeff_pares[i % (n_i/2)];
		impar = coeff_impares[i % (n_i/2)];
		out[j] = last_out[par] + xk[k]*last_out[impar];
	}
	return;
}
