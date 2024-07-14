/*
 * fft.h
 * Copyright (C) 2023  <@Unidad01>
 *
 * Distributed under terms of the MIT license.
 */
#ifndef FFT_H
#define FFT_H
#include<complex>
#include<cmath>
void fft(std::complex<float>* coefficients, unsigned long long int n, std::complex<float>* out);
void ifft(std::complex<float>* coefficients, unsigned long long int n, std::complex<float>* out);
void fft_r(std::complex<float>* coefficients, unsigned long long int n, unsigned long long int factor, std::complex<float>* xk, unsigned long long int* coeff_indices, std::complex<float>* out);
void fft2(std::complex<float>** coefficients, unsigned long long int n, unsigned long long int m, std::complex<float>** out);
void ifft2(std::complex<float>** coefficients, unsigned long long int n, unsigned long long int m, std::complex<float>** out);
#endif
