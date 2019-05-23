#include <cstdlib>
#include <malloc.h>
#include <iostream>
#include <iomanip>
#include <future>
#include <windows.h>

using namespace std;

const long long int MB = 1024 * 1024;
const long long int CACHE_LINE_SIZE_BYTES = 64;
const long long int CACHE_L3_SIZE_BYTES = 8 * MB;
const long long int OFFSET_BYTES = 88 * MB;
const long long int OFFSET_SIZE_INT = OFFSET_BYTES / sizeof(long long int);
const long long int MAX_ASSOCIATION = 20;

void initialize(long long int* arr, const long long int associativity, const long long int elements_in_block)
{
	for (long long int element_index = 0; element_index < elements_in_block; element_index++)
	{
		for (long long int block_index = 0; block_index < associativity - 1; block_index++)
		{
			arr[block_index * OFFSET_SIZE_INT + element_index] = (block_index + 1) * OFFSET_SIZE_INT + element_index;
		}
		if (element_index == elements_in_block - 1)
		{
			arr[(associativity - 1) * OFFSET_SIZE_INT + element_index] = 0;
		}
		else
		{
			arr[(associativity - 1) * OFFSET_SIZE_INT + element_index] = element_index + 1;
		}
	}
}

unsigned long long int read_array_through(long long int array[])
{
	long long int index = 0;
	const int tries = 200;
	const long long int start_time = __rdtsc();

	for (int i = 0; i < tries; i++)
	{
		do
		{
			index = array[index];
		} while (index != 0);
	}

	const long long int end_time = __rdtsc();

	return (end_time - start_time) / tries;
}

int main() 
{
	const int SIZE_IN_BYTES = OFFSET_BYTES * MAX_ASSOCIATION;
	const int LONG_LONG_COUNT = SIZE_IN_BYTES / sizeof(long long int);

	for (auto associativity = 1; associativity < MAX_ASSOCIATION + 1; associativity++)
	{
		long long int* _array = static_cast<long long int*>(_aligned_malloc(SIZE_IN_BYTES, CACHE_LINE_SIZE_BYTES));

		for (long long int i = 0; i < LONG_LONG_COUNT; i++)
		{
			_array[i] = 0;
		}

		const long long int elements_in_block = ceil((double)CACHE_L3_SIZE_BYTES / (sizeof(long long int) * (associativity)));

		initialize(_array, associativity, elements_in_block);
		unsigned long long int time = read_array_through(_array);

		_aligned_free(_array);

		cout << left << setw(5) << setfill(' ') << associativity << time << " clock cycles" << endl;
	}

	system("pause");

	return 0;
}