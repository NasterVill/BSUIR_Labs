#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "host_helper.h"
#include "device_helper.cuh"

#include <cstdlib>
#include <cstdio>
#include <cassert>

#include <iostream>
#include <iomanip>
#include <chrono>

using namespace std;

#define CUDA_DEBUG

// Convenience function for checking CUDA runtime API results
// can be wrapped around any runtime API call. No-op in release builds.
inline
cudaError_t checkCuda(cudaError_t result)
{
#if defined(DEBUG) || defined(_DEBUG) || defined(CUDA_DEBUG)
	if (result != cudaSuccess)
	{
		cerr << "CUDA Runtime Error: " << cudaGetErrorString(result) << endl;
		assert(result == cudaSuccess);
	}
#endif
	return result;
}

// Check errors
void postprocess(const short *in_data, const short *out_data, int size, float ms)
{
	for (int i = 0; i < size; i++)
	{
		if (in_data[i] != out_data[i])
		{
			cout << endl << "*** FAILED ***" << endl;
			cout << "Index: " << i << " exhibit: " << +in_data[i] << " result: " << +out_data[i] << endl << endl;
			break;
		}
	}
	printf("Time spent: %f\n" ,ms);
}

int main()
{
	const int width = 1222;
	const int height = 144;
	const int size = width * height * sizeof(short);

	int aligned_width = (width + MATRIX_TILE - 1) / MATRIX_TILE * (height + MATRIX_TILE - 1) / MATRIX_TILE;
	dim3 dimGrid(aligned_width, 1, 1);
	dim3 dimBlock(MATRIX_TILE * MATRIX_TILE, 1, 1);

	short *host_input_data = (short*)malloc(size);
	short *device_to_host_memory = (short*)malloc(size);
	short *cpu_rotated_data = (short*)malloc(size);

	short *device_input_data, *device_rotated_data;
	checkCuda(cudaMalloc(&device_input_data, size));
	checkCuda(cudaMalloc(&device_rotated_data, size));

	// host
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{ 
			host_input_data[j * width + i] = j * width + i;
		}
	}

	// ********************************************************************************************************

	cout << "Rotating via CPU" << endl;
	auto start_cpu = chrono::steady_clock::now();
	for (int i = 0; i < NUM_REPS; i++)
	{
		MirrotAndRotateMatrix(host_input_data, cpu_rotated_data, width, height);
	}
	auto end_cpu = chrono::steady_clock::now();
	auto cpu_time = end_cpu - start_cpu;
	cout << "CPU time: " << chrono::duration<double, milli>(cpu_time).count() << endl;

	checkCuda(cudaMemcpy(device_input_data, host_input_data, size, cudaMemcpyHostToDevice));

	cudaEvent_t startEvent, stopEvent;
	checkCuda(cudaEventCreate(&startEvent));
	checkCuda(cudaEventCreate(&stopEvent));
	float ms;

	cout << "Rotating via GPU" << endl;
	checkCuda(cudaMemset(device_rotated_data, 0, size));
	memset(device_to_host_memory, 0, size);

	__gpu_RotateMatrix <<<dimGrid, dimBlock>>> (device_input_data, device_rotated_data, width, height);
	checkCuda(cudaEventRecord(startEvent, 0));
	for (int i = 0; i < NUM_REPS; i++)
	{
		__gpu_RotateMatrix <<<dimGrid, dimBlock>>> (device_input_data, device_rotated_data, width, height);
	}
	checkCuda(cudaEventRecord(stopEvent, 0));
	checkCuda(cudaEventSynchronize(stopEvent));
	checkCuda(cudaEventElapsedTime(&ms, startEvent, stopEvent));
	checkCuda(cudaMemcpy(device_to_host_memory, device_rotated_data, size, cudaMemcpyDeviceToHost));
	postprocess(cpu_rotated_data, device_to_host_memory, width * height, ms);

	/*for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			cout << setw(5) << device_to_host_memory[i * width + j];
			if (j == width - 1) cout << endl;
		}
	}

	cout << endl << endl;

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			cout << setw(5) << cpu_rotated_data[i * width + j];
			if (j == width - 1) cout << endl;
		}
	}*/

	// *******************************************************************************************************

	checkCuda(cudaEventDestroy(startEvent));
	checkCuda(cudaEventDestroy(stopEvent));
	checkCuda(cudaFree(device_rotated_data));
	checkCuda(cudaFree(device_input_data));
	free(host_input_data);
	free(device_to_host_memory);
	free(cpu_rotated_data);
}