#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "device_image_pocessing.cuh"

#include <stdio.h>

__global__ void ApplySquareAverageFilter(
	unsigned char *input_data,
	unsigned char *output_data,
	const int width,
	const int height,
	const int padded_width,
	const int padded_height,
	const int in_pitch,
	const int out_pitch
)
{
	const int x = blockIdx.x * AMOUNT_OF_THREADS_X + threadIdx.x;
	const int y = blockIdx.y * AMOUNT_OF_THREADS_Y + threadIdx.y;

	const int int_widht = in_pitch / sizeof(int);
	const int width_border = (width + sizeof(int) - 1) / sizeof(int);

	uchar4 *reintterpreted_input = reinterpret_cast<uchar4*>(input_data);
	uchar4 *reintterpreted_output = reinterpret_cast<uchar4*>(output_data);

	// padding memory so that we could read a bit more than int per thread
	__shared__ uchar4 shared_memory[AMOUNT_OF_THREADS_Y + 2][AMOUNT_OF_THREADS_X + 2];

	if (x < int_widht || y < padded_height)
	{
		uchar4 *reintterpreted_input = reinterpret_cast<uchar4*>(input_data);
		uchar4 *reintterpreted_output = reinterpret_cast<uchar4*>(output_data);

		// initialized shared memory (first part)
		shared_memory[threadIdx.y][threadIdx.x] = reintterpreted_input[y * int_widht + x];

		if (threadIdx.y < 2)
		{
			// copying 2 additional bottom rows (beyond AMOUNT_OF_THREADS_Y - 1)
			if (y + AMOUNT_OF_THREADS_Y <= height)
			{
				shared_memory[AMOUNT_OF_THREADS_Y + threadIdx.y][threadIdx.x] = reintterpreted_input[(AMOUNT_OF_THREADS_Y + y) * int_widht + x];
			}
			if (threadIdx.x < AMOUNT_OF_THREADS_Y + 2)
			{
				int temp_x = blockIdx.x * AMOUNT_OF_THREADS_X + threadIdx.y;
				int temp_y = blockIdx.y * AMOUNT_OF_THREADS_Y + threadIdx.x;

				if (temp_x < int_widht && temp_y < padded_height)
				{
					shared_memory[threadIdx.x][AMOUNT_OF_THREADS_X + threadIdx.y] = reintterpreted_input[temp_y * int_widht + AMOUNT_OF_THREADS_X + temp_x];
				}
			}
		}
	}

	__syncthreads();

	if (y >= height || x >= width_border)
	{
		return;
	}

	// till now every thread was assigned to a specific int,
	// but now 1 thread will work with 6 ints , so we could form a 
	// transaction to a global memory

	uchar4 generated_int = { 0 };

	uchar4 first_int = shared_memory[threadIdx.y][threadIdx.x];
	uchar4 second_int = shared_memory[threadIdx.y][threadIdx.x + 1];
	uchar4 third_int = shared_memory[threadIdx.y + 1][threadIdx.x];
	uchar4 fourth_int = shared_memory[threadIdx.y + 1][threadIdx.x + 1];
	uchar4 fifth_int = shared_memory[threadIdx.y + 2][threadIdx.x];
	uchar4 sixth_int = shared_memory[threadIdx.y + 2][threadIdx.x + 1];

	generated_int.x = (first_int.x + first_int.y + first_int.z +
					third_int.x + third_int.y + third_int.z +
					fifth_int.x + fifth_int.y + fifth_int.z) / 9;

	generated_int.y = (first_int.y + first_int.z + first_int.w +
					third_int.y + third_int.z + third_int.w +
					fifth_int.y + fifth_int.z + fifth_int.w) / 9;

	generated_int.z = (first_int.z + first_int.w + second_int.x +
					third_int.z + third_int.w + fourth_int.x +
					fifth_int.z + fifth_int.w + sixth_int.x) / 9;

	generated_int.w = (first_int.w + second_int.x + second_int.y +
					third_int.w + fourth_int.x + fourth_int.y +
					fifth_int.w + sixth_int.x + sixth_int.y) / 9;

	const int output_int_width = out_pitch / sizeof(int);

	reintterpreted_output[y * output_int_width + x] = generated_int;
}