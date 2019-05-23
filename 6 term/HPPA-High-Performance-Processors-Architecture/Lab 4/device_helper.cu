#include "device_helper.cuh"
#include "host_helper.h"

__global__ void __gpu_MirrorMatrix(short* input_data, short* output_data, int width, int height)
{
	__shared__ ushort2 tile[MATRIX_TILE * MATRIX_TILE];

	// x and y for int 'array' (which may be a bit jagged ('cause we can have 1 excessive short in the end of an array))
	int x = blockIdx.x * MATRIX_TILE * MATRIX_TILE + threadIdx.x;
	int max_x = (width * height + 1) / 2;

	if (x >= max_x)
	{
		return;
	}

	ushort2* interpreted_input = (ushort2*)input_data;

	int new_y = x * 2 / width;
	int new_x = x * 2 % width;

	if (x * 2 + 1 < width * height)
	{
		tile[threadIdx.x] = interpreted_input[x];
	}
	else
	{
		// process last (odd) element, if needed
		tile[threadIdx.x] = interpreted_input[max_x - 1];
		output_data[MLPIM(new_x, new_y, width)] = tile[threadIdx.x].x;
		return;
	}

	ushort2 res = tile[threadIdx.x];
	output_data[MLPIM(new_x, new_y, width)] = res.x;

	/*printf("x: %d, new_x:%d, new_y:%d, rex.x:%d, res.y:%d, MLPIM.y: %d, MLPIM.y :%d, width:%d, cond:%d  %d\n",
		x, new_x, new_y, res.x, res.y,
		MLPIM(new_x, new_y, width), (new_x == width - 1) ? MLPIM(0, new_y + 1, width) : MLPIM((new_x + 1), new_y, width),
		width, (new_x == width - 1), ((new_y + 1) * width - new_x - 1));*/

	if (new_x == width - 1)
	{
		output_data[MLPIM(0, (new_y + 1), width)] = res.y;
	}
	else
	{
		output_data[MLPIM((new_x + 1), new_y, width)] = res.y;
	}
}

__global__ void __gpu_RotateMatrix(short* input_data, short* output_data, int width, int height)
{
	__shared__ ushort2 tile[MATRIX_TILE * MATRIX_TILE];

	// x and y for int 'array' (which may be a bit jagged ('cause we can have 1 excessive short in the end of an array))
	int x = blockIdx.x * MATRIX_TILE * MATRIX_TILE + threadIdx.x;
	int size = width * height;
	int max_x = (size + 1) / 2;

	if (x >= max_x)
	{
		return;
	}

	ushort2* interpreted_input = (ushort2*)input_data;

	int new_y = x * 2 / width;
	int new_x = x * 2 % width;

	if (x * 2 + 1 < size)
	{
		tile[threadIdx.x] = interpreted_input[x];
	}
	else
	{
		// process last (odd) element, if needed
		tile[threadIdx.x].x = interpreted_input[max_x - 1].x;
		output_data[size - 1 - LPIM(new_y, new_x, height)] = tile[threadIdx.x].x;
		return;
	}

	ushort2 res = tile[threadIdx.x];
	output_data[size - 1 - LPIM(new_y, new_x, height)] = res.x;

	if (new_x == width - 1)
	{
		output_data[size - 1 - LPIM((new_y + 1), 0, height)] = res.y;
	}
	else
	{
		output_data[size - 1 - LPIM(new_y, (new_x + 1), height)] = res.y;
	}
}