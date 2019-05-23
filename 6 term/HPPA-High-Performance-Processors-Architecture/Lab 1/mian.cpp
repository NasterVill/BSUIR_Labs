#include <cstdlib>
#include <cstdio>
#include <intrin.h>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include "helper.h"

using namespace std;

int main()
{
	const int MATRIX_DIMENSION = 2520;

	float** first_matrix = CreateMatrix(MATRIX_DIMENSION, MATRIX_DIMENSION);
	float** second_matrix = CreateMatrix(MATRIX_DIMENSION, MATRIX_DIMENSION);
	float** vec_result_matrix = CreateMatrix(MATRIX_DIMENSION, MATRIX_DIMENSION);
	float** no_vec_result_matrix = CreateMatrix(MATRIX_DIMENSION, MATRIX_DIMENSION);
	float** avx_result_matrix = CreateMatrix(MATRIX_DIMENSION, MATRIX_DIMENSION);
	float** cache_result_matrix = CreateMatrix(MATRIX_DIMENSION, MATRIX_DIMENSION);

	InitializeMatrix(first_matrix, MATRIX_DIMENSION, MATRIX_DIMENSION);
	InitializeMatrix(second_matrix, MATRIX_DIMENSION, MATRIX_DIMENSION);

	// With auto vectorization
	// Multiplying matrices m*n and n*k
	// in our particular case m = n = k = MATRIX_DIMENSION

	auto start_clocks = __rdtsc();

	__vec_MultipyMatrices(first_matrix, second_matrix, vec_result_matrix, MATRIX_DIMENSION);

	auto end_clocks = __rdtsc();

	cout << left << setw(50) << setfill(' ') << "Clock count with auto vectorization:  " << end_clocks - start_clocks << endl;

	// Without auto vectorization

	start_clocks = __rdtsc();

	__no_vec_MultipyMatrices(first_matrix, second_matrix, no_vec_result_matrix, MATRIX_DIMENSION);

	end_clocks = __rdtsc();

	cout << left << setw(50) << setfill(' ') << "Clock count without auto vectorization:  " << end_clocks - start_clocks << endl;

	// With AVX

	start_clocks = __rdtsc();

	__avx_MultipyMatrices(first_matrix, second_matrix, avx_result_matrix, MATRIX_DIMENSION);

	end_clocks = __rdtsc();

	cout << left << setw(50) << setfill(' ') << "Clock count with AVX Intrinsics commands: " << end_clocks - start_clocks << endl;

	// Cache optimization

	start_clocks = __rdtsc();

	__cache_opt_MatrixMultiplication(first_matrix, second_matrix, cache_result_matrix, MATRIX_DIMENSION);

	end_clocks = __rdtsc();

	cout << left << setw(50) << setfill(' ') << "Clock count with cache optimization : " << end_clocks - start_clocks << endl;

	// Compare results

	cout << "Compare: ";

	if (!CompareMatrices(vec_result_matrix, no_vec_result_matrix, MATRIX_DIMENSION, MATRIX_DIMENSION))
	{
		cout << "Vectorized matrix differs from non vectorized" << endl;
	}
	else if (!CompareMatrices(no_vec_result_matrix, avx_result_matrix, MATRIX_DIMENSION, MATRIX_DIMENSION))
	{
		cout << "Non vectorized matrix differs from AVX" << endl;
	}
	else if (!CompareMatrices(avx_result_matrix, cache_result_matrix, MATRIX_DIMENSION, MATRIX_DIMENSION))
	{
		cout << "AVX matrix differs from cache" << endl;
	}
	else
	{
		cout << "All matrices are identical" << endl;
	}

	// Clear allocated memmory

	for (int row = 0; row < MATRIX_DIMENSION; row++)
	{
		free(first_matrix[row]);
		free(second_matrix[row]);
		free(vec_result_matrix[row]);
		free(no_vec_result_matrix[row]);
		free(avx_result_matrix[row]);
		free(cache_result_matrix[row]);
	}
	free(first_matrix);
	free(second_matrix);
	free(vec_result_matrix);
	free(no_vec_result_matrix);
	free(avx_result_matrix);
	free(cache_result_matrix);

	system("pause");
}