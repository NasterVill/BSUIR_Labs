#include "host_helper.h"
#include <cstdio>
#include <iostream>
#include <iomanip>

using namespace std;

void MirrotAndRotateMatrix(short* in_matrix, short* out_matrix, const int in_width, const int in_height)
{
	const int out_width = in_height;
	const int out_height = in_width;
	int size = in_width * in_height;
	for (int y = 0; y < in_height; y++)
	{
		for (int x = 0; x < in_width; x++)
		{
			// In this case mirroring and rotation is composed from:
			// * transposition (swapping x and y indices)
			// and
			// * flip (via refleciting the indiex against size - 1)
			out_matrix[size - 1 - LPIM(y, x, out_width)] = in_matrix[LPIM(x, y, in_width)];
		}
	}
}

void ShowHostFunctionsWork(const int width, const int height)
{
	const int size = width * height;

	short* in_array = new short[size];
	short* out_array = new short[size];

	for (int i = 0; i < size; i++)
	{
		in_array[i] = i + 1;
	}

	MirrotAndRotateMatrix(in_array, out_array, width, height);

	cout << "Exodus matrix" << endl;
	for (int i = 0; i < size; i++)
	{
		cout << setw(5) << +in_array[i] << " ";
		if ((i + 1) % width == 0) 
		{
			cout << endl;
		}
	}

	cout << endl << endl;

	cout << "Mirrored  and Rotated matrix" << endl;
	for (int i = 0; i < size; i++)
	{
		cout << setw(5) << +out_array[i] << " ";
		if ((i + 1) % height == 0)
		{
			cout << endl;
		}
	}

	delete in_array;
	delete out_array;
}
