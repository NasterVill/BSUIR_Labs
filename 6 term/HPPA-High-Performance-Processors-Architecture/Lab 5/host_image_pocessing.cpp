#include <iostream>
#include <iomanip>

#include "host_image_pocessing.h"

using namespace std;

unsigned char* PadDataByOneByte(unsigned char *input_data, int width, int height)
{
	const int new_width = width + 2;
	const int new_height = height + 2;

	unsigned char *output_data = new unsigned char[new_width * new_height];

	// copy initial part
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			output_data[(y + 1) * new_width + x + 1] = input_data[y * width + x];
		}
	}

	// left upper corner
	output_data[0] = input_data[0];
	// right upper corner
	output_data[new_width - 1] = input_data[width - 1];
	// left down corner
	output_data[new_width * (new_height - 1)] = input_data[width * (height - 1)];
	// right down corner
	output_data[new_width * new_height - 1] = input_data[width * height - 1];

	// horizontal copy
	for (int x = 0; x < width; x++)
	{
		output_data[x + 1] = input_data[x];
		output_data[(new_height - 1) * new_width + x + 1] = input_data[width * (height - 1) + x];
	}

	// vertical copy
	for (int y = 0; y < height; y++)
	{
		output_data[(y + 1) * new_width] = input_data[y * width];
		output_data[(y + 1) * new_width + new_width - 1] = input_data[y * width + width - 1];
	}

	return output_data;
}

void PrintMatrix(unsigned char *data, const int width, const int height)
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			cout << setw(5) << +data[i * width + j];
			if (j == width - 1) cout << endl;
		}
	}

	cout << endl << endl;
}

// Use PadDataByOneByte transforamtion for input before using this function
void SquareAverageFilter(
	unsigned char *input_matrix,
	unsigned char *output_matrix,
	const int width, 
	const int height,
	const int padded_width,
	const int padded_height
)
{
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			unsigned char average = (input_matrix[y * padded_width + x] + input_matrix[y * padded_width + x + 1] + input_matrix[y * padded_width + x + 2] +
				input_matrix[(y + 1) * padded_width + x] + input_matrix[(y + 1) * padded_width + x + 1] + input_matrix[(y + 1) * padded_width + x + 2] +
				input_matrix[(y + 2) * padded_width + x] + input_matrix[(y + 2) * padded_width + x + 1] + input_matrix[(y + 2) * padded_width + x + 2]) / 9;

			output_matrix[y * width + x] = average;
			
		}
	}
}

int SquareAverageFilterGeneric(
	unsigned char *input_matrix,
	unsigned char *output_matrix,
	const int width,
	const int height,
	const int filter_dimension
)
{
	if (filter_dimension % 2 == 0)
	{
		return -1;
	}

	const int padded_width = width + 2 * (int)(filter_dimension / 2);
	const int padded_height = height + 2 * (int)(filter_dimension / 2);

	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			unsigned char average = 0;

			for (int i = 0; i < filter_dimension; i++)
			{
				for (int j = 0; j < filter_dimension; j++)
				{
					average += input_matrix[(y + i) * padded_width + x + j];
				}
			}

			average /= (filter_dimension * filter_dimension);

			output_matrix[y * width + x] = average;
		}
	}

	return 0;
}


void ApplySquareAverageFilter(
	unsigned char *input_matrix,
	unsigned char *output_matrix,
	const int width,
	const int height
)
{
	unsigned char *padded_input_matrix = PadDataByOneByte(input_matrix, width, height);

	const int padded_width = width + 2;
	const int padded_height = height + 2;

	SquareAverageFilter(padded_input_matrix, output_matrix, width, height, padded_width, padded_height);
}