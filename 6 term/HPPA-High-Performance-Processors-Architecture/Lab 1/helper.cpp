#include <cstdlib>
#include <ctime>
#include <immintrin.h>
#include "helper.h"

using namespace std;

float** CreateMatrix(const int rows, const int coloumns)
{
	// using calloc instead of new to work with raw memmory
	float** matrix = static_cast<float**>(calloc(coloumns, sizeof(float)));

	for (int i = 0; i < rows; i++)
	{
		matrix[i] = static_cast<float*>(calloc(coloumns, sizeof(float)));
	}

	return matrix;
}

void InitializeMatrix(float** matrix, const int rows, const int coloumns)
{
	srand(static_cast<unsigned int>(time(nullptr)));

	for (int i = 0; i < rows; i++) 
	{
		for (int j = 0; j < coloumns; j++)
		{
			matrix[i][j] = static_cast<float>((rand() % 10000));
		}
	}
}

bool CompareMatrices(float** first_matrix, float** second_matrix, const int rows, const int columns)
{
	for (int row = 0; row < rows; row++)
	{
		for (int column = 0; column < columns; column++)
		{
			if (first_matrix[row][column] != second_matrix[row][column])
			{
				return false;
			}
		}
	}

	return true;
}

void __vec_MultipyMatrices(float** first_matrix, float** second_matrix, float** result_matrix, const int dimension)
{
	for (int first_matrix_row = 0; first_matrix_row < dimension; first_matrix_row++) 
	{
		float* result_row = result_matrix[first_matrix_row];

		for (int common_dimension_index = 0; common_dimension_index < dimension; common_dimension_index++)
		{

			const float first_matrix_temp = first_matrix[first_matrix_row][common_dimension_index];
			float* second_matrix_temp_row = second_matrix[common_dimension_index];

			for (int second_matrix_column = 0; second_matrix_column < dimension; second_matrix_column++)
			{
				result_row[second_matrix_column] +=
					first_matrix_temp * second_matrix_temp_row[second_matrix_column];
			}
		}
	}
}

void __no_vec_MultipyMatrices(float** first_matrix, float** second_matrix, float** result_matrix, const int dimension)
{
	for (int first_matrix_row = 0; first_matrix_row < dimension; first_matrix_row++)
	{

		float* result_row = result_matrix[first_matrix_row];

		for (int common_dimension_index = 0; common_dimension_index < dimension; common_dimension_index++)
		{

			const float first_matrix_temp = first_matrix[first_matrix_row][common_dimension_index];
			float* second_matrix_temp_row = second_matrix[common_dimension_index];

			#pragma loop(no_vector)
			for (int second_matrix_column = 0; second_matrix_column < dimension; second_matrix_column++)
			{
				result_row[second_matrix_column] +=
					first_matrix_temp * second_matrix_temp_row[second_matrix_column];
			}
		}
	}
}

void __avx_MultipyMatrices(float** first_matrix, float** second_matrix, float** result_matrix, const int dimension)
{
	for (int row_first_matrix = 0; row_first_matrix < dimension; row_first_matrix++)
	{
		float* result_temp_row = result_matrix[row_first_matrix];

		for (int common_dimension_index = 0; common_dimension_index < dimension; common_dimension_index++)
		{
			const float first_matrix_temp = first_matrix[row_first_matrix][common_dimension_index];
			float* second_matrix_temp_row = second_matrix[common_dimension_index];

			for (int column_second_matrix = 0; column_second_matrix < dimension; column_second_matrix += 8)
			{
				// _mm256_set1_ps : Broadcast single-precision (32-bit) floating-point value a to all elements of dst.
				const __m256 first_matrix_multiplier = _mm256_set1_ps(first_matrix_temp);

				// load the result row to accumulate the multiplication result
				 __m256 prev_result = _mm256_load_ps(result_temp_row + column_second_matrix);

				 // load the row from the second matrix
				__m256 second_matrix_row = _mm256_load_ps(second_matrix_temp_row + column_second_matrix);

				// multiply vectors
				__m256 mult_result = _mm256_mul_ps(first_matrix_multiplier, second_matrix_row);

				// accumulate the result
				__m256 sum_result = _mm256_add_ps(prev_result, mult_result);

				// store the result
				_mm256_store_ps(result_temp_row + column_second_matrix, sum_result);
			}
		}
	}
}

void CacheInnerLoop(float* result_row, float* second_matrix_temp_row, const int L1_CACHE_SIZE, const int mm_multiplier, const int cm_column_start)
{
	for (int cm_column_index = 0; cm_column_index < L1_CACHE_SIZE; cm_column_index++)
	{
		result_row[cm_column_start + cm_column_index] += mm_multiplier * second_matrix_temp_row[cm_column_start + cm_column_index];
	}
}

void __cache_opt_MatrixMultiplication(float** first_matrix, float** second_matrix, float** result_matrix, const int dimension)
{  
	// single
	const int L3_CACHE_SIZE = 1260;
	// per core
	const int L2_CACHE_SIZE = 210;
	const int L1_CACHE_SIZE = 72;
	//

	const int L3_IN_MATRIX = dimension / L3_CACHE_SIZE;
	const int L2_IN_L3 = L3_CACHE_SIZE / L2_CACHE_SIZE;
	const int L1_IN_L2 = L2_CACHE_SIZE / L1_CACHE_SIZE;

	// mm - moving matrix i.e. the first one
	// cm - Cached Matrix i.e. the second one
	for (int cm_row_index_L3 = 0; cm_row_index_L3 < L3_IN_MATRIX; cm_row_index_L3++)
	{
		for (int cm_column_index_L3 = 0; cm_column_index_L3 < L3_IN_MATRIX; cm_column_index_L3++)
		{
			for (int cm_row_index_L2 = 0; cm_row_index_L2 < L2_IN_L3; cm_row_index_L2++)
			{
				for (int cm_column_index_L2 = 0; cm_column_index_L2 < L2_IN_L3; cm_column_index_L2++)
				{
					for (int cm_row_index_L1 = 0; cm_row_index_L1 < L1_IN_L2; cm_row_index_L1++)
					{
						for (int cm_column_index_L1 = 0; cm_column_index_L1 < L1_IN_L2; cm_column_index_L1++)
						{
							for (int mm_row_index = 0; mm_row_index < dimension; mm_row_index++)
							{
								float* result_row = result_matrix[mm_row_index];

								const int common_dimension_start = (L3_CACHE_SIZE * cm_row_index_L3)
									+ (L2_CACHE_SIZE * cm_row_index_L2)
									+ (L1_CACHE_SIZE * cm_row_index_L1);

								const int common_dimension_end = common_dimension_start + L1_CACHE_SIZE;

								for (int common_dimension_index = common_dimension_start; common_dimension_index < common_dimension_end; common_dimension_index++)
								{
									float mm_multiplier = first_matrix[mm_row_index][common_dimension_index];
									float* second_matrix_temp_row = second_matrix[common_dimension_index];

									const int cm_column_start = (L3_CACHE_SIZE * cm_column_index_L3)
										+ (L2_CACHE_SIZE * cm_column_index_L2)
										+ (L1_CACHE_SIZE * cm_column_index_L1);

									// DIRTY HACK made to vectorize the inner loop (otherwise we get a compiler bug: not beign able to
									// vectorize the loop even though it is vectorizable)
									CacheInnerLoop(result_row, second_matrix_temp_row, L1_CACHE_SIZE, mm_multiplier, cm_column_start);
								}
							}
						}
					}
				}
			}
		}
	}
}