#include"fft.h"
void fft(std::complex<float>* coefficients, unsigned long long int n, std::complex<float>* out) {
	std::complex<float> xk[n];
	unsigned long long indices[n], i;
	float x, y;
	float theta;
	// proceso para generar los xk.
	for(theta = 0.0f, i = 0; theta < 2*M_PI; theta+= 2*M_PI/n, i++) {
		x = cosf(theta);
		y = sinf(theta);
		xk[i].real(x);
		xk[i].imag(y);
	}
	for(i = 0; i < n; i++) {
		indices[i] = i;
	}
	fft_r(coefficients, n, 1, xk, indices, out);
}
void fft_r(std::complex<float>* coefficients, unsigned long long int n, unsigned long long int factor, std::complex<float>* xk, unsigned long long int* coeff_indices, std::complex<float>* out) {
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
	std::complex<float> last_out[n] = {0.0f + 1j*0.0f};
	fft_r(coefficients, n, factor*2, xk, coeff_pares, last_out);
	fft_r(coefficients, n, factor*2, xk, coeff_impares, last_out);
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
