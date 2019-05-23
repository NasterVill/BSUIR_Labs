#include <iostream>
#include <iomanip>

#include "host_image_pocessing.h"

using namespace std;

pixel* PadDataByOnePixel(pixel *input_data, int width, int height)
{
	const int new_width = width + 2;
	const int new_height = height + 2;

	pixel *output_data = new pixel[new_width * new_height];

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

void PrintPixelMatrix(pixel *data, const int width, const int height)
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			cout << setw(15) << "{" << +data[i * width + j].r << "," << +data[i * width + j].g << "," << +data[i * width + j].b << "},";
			if (j == width - 1)
			{
				cout << endl;
			}
		}
	}

	cout << endl << endl;
}

// Use PadDataByOneByte transforamtion for input before using this function
void SquareAverageFilter(
	pixel *input_matrix,
	pixel *output_matrix,
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
			pixel average = { 0 };

			average.r = (input_matrix[y * padded_width + x].r + input_matrix[y * padded_width + x + 1].r + input_matrix[y * padded_width + x + 2].r +
				input_matrix[(y + 1) * padded_width + x].r + input_matrix[(y + 1) * padded_width + x + 1].r + input_matrix[(y + 1) * padded_width + x + 2].r +
				input_matrix[(y + 2) * padded_width + x].r + input_matrix[(y + 2) * padded_width + x + 1].r + input_matrix[(y + 2) * padded_width + x + 2].r) / 9;

			average.g = (input_matrix[y * padded_width + x].g + input_matrix[y * padded_width + x + 1].g + input_matrix[y * padded_width + x + 2].g +
				input_matrix[(y + 1) * padded_width + x].g + input_matrix[(y + 1) * padded_width + x + 1].g + input_matrix[(y + 1) * padded_width + x + 2].g +
				input_matrix[(y + 2) * padded_width + x].g + input_matrix[(y + 2) * padded_width + x + 1].g + input_matrix[(y + 2) * padded_width + x + 2].g) / 9;

			average.b = (input_matrix[y * padded_width + x].b + input_matrix[y * padded_width + x + 1].b + input_matrix[y * padded_width + x + 2].b +
				input_matrix[(y + 1) * padded_width + x].b + input_matrix[(y + 1) * padded_width + x + 1].b + input_matrix[(y + 1) * padded_width + x + 2].b +
				input_matrix[(y + 2) * padded_width + x].b + input_matrix[(y + 2) * padded_width + x + 1].b + input_matrix[(y + 2) * padded_width + x + 2].b) / 9;

			output_matrix[y * width + x] = average;
			
		}
	}
}

void ApplySquareAverageFilter(
	pixel *input_matrix,
	pixel *output_matrix,
	const int width,
	const int height
)
{
	pixel *padded_input_matrix = PadDataByOnePixel(input_matrix, width, height);

	const int padded_width = width + 2;
	const int padded_height = height + 2;

	SquareAverageFilter(padded_input_matrix, output_matrix, width, height, padded_width, padded_height);
}