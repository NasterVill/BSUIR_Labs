#ifndef HELPER_H
#define HELPER_H

float** CreateMatrix(const int rows, const int coloumns);
void InitializeMatrix(float** matrix, const int rows, const int coloumns);
bool CompareMatrices(float** first_matrix, float** second_matrix, const int rows, const int columns);
void __vec_MultipyMatrices(float** first_matrix, float** second_matrix, float** result_matrix, const int dimension);
void __no_vec_MultipyMatrices(float** first_matrix, float** second_matrix, float** result_matrix, const int dimension);
void __avx_MultipyMatrices(float** first_matrix, float** second_matrix, float** result_matrix, const int dimension);
void __cache_opt_MatrixMultiplication(float** first_matrix, float** second_matrix, float** result_matrix, const int dimension);

#endif HELPER_H