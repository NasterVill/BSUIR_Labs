#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "inc/helper_image.h"

#include <cstdlib>
#include <cstdio>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <chrono>

#include "host_image_pocessing.h"
#include "device_image_pocessing.cuh"

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
void postprocess(const unsigned char *in_data, const unsigned char *out_data, int size, float ms)
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
	cout << "Time spent: " << ms << endl;
}

const int NUM_REPS = 1;

int main()
{
	char file_name[] = "Creedence.pgm";
	// char file_name[] = "city.pgm";
	// char file_name[] = "ray.pgm";
	// char file_name[] = "CITY_LARGE.pgm";
	char cpu_resilt_file_name[] = "CPU_result.pgm";
	char gpu_resilt_file_name[] = "GPU_result.pgm";
	
	size_t width = 0;
	size_t height = 0;
	int channels = 0;

	unsigned char *input_data = nullptr;
	__loadPPM(
		file_name, &input_data,
		reinterpret_cast<unsigned int*>(&width), 
		reinterpret_cast<unsigned int*>(&height),
		reinterpret_cast<unsigned int*>(&channels)
	);

	cout << width << " " << height << " " << channels << endl << endl;

	const size_t padded_width = width + 2;
	const size_t padded_height = height + 2;

	const size_t size = width * height;
	const size_t padded_size = width * height;

	unsigned char *cpu_output_data = new unsigned char[size];
	unsigned char *gpu_output_data = new unsigned char[size];

	// ********************************************************************************************************

	cout << "Filtering via CPU" << endl;
	auto start_cpu = chrono::steady_clock::now();
	for (int i = 0; i < NUM_REPS; i++)
	{
		ApplySquareAverageFilter(input_data, cpu_output_data, width, height);
	}
	auto end_cpu = chrono::steady_clock::now();
	auto cpu_time = end_cpu - start_cpu;
	cout << "CPU time: " << chrono::duration<double, milli>(cpu_time).count() / NUM_REPS << endl;

	// ********************************************************************************************************

	// ********************************************************************************************************

	size_t input_pitch = 0;
	unsigned char *padded_input = PadDataByOneByte(input_data, width, height);
	unsigned char *pitched_input_data = nullptr;
	checkCuda(cudaMallocPitch(reinterpret_cast<void**>(&pitched_input_data), &input_pitch, padded_width, padded_height));
	checkCuda(cudaMemcpy2D(
		pitched_input_data,
		input_pitch,
		padded_input,
		padded_width,
		padded_width,
		padded_height,
		cudaMemcpyHostToDevice
	));

	size_t output_pitch = 0;
	unsigned char *pitched_output_data = nullptr;
	cudaMallocPitch(reinterpret_cast<void**>(&pitched_output_data), &output_pitch, width, height);

	float time = 0;
	cudaEvent_t startEvent, stopEvent;
	checkCuda(cudaEventCreate(&startEvent));
	checkCuda(cudaEventCreate(&stopEvent));

	//
	cout << "Filtering via GPU" << " pitch: " << input_pitch << endl;

	int aligned_width = (input_pitch + AMOUNT_OF_THREADS_X - 1) / AMOUNT_OF_THREADS_X;
	int aligned_height = (height + AMOUNT_OF_THREADS_Y - 1) / AMOUNT_OF_THREADS_Y;
	dim3 dimGrid(aligned_width, aligned_height, 1);
	dim3 dimBlock(AMOUNT_OF_THREADS_X, AMOUNT_OF_THREADS_Y, 1);
	
	// warm up
	ApplySquareAverageFilter << <dimGrid, dimBlock >> > (
		pitched_input_data, pitched_output_data,
		width, height,
		padded_width, padded_height,
		input_pitch, output_pitch
		);

	checkCuda(cudaEventRecord(startEvent, 0));
	for (int i = 0; i < NUM_REPS; i++)
	{
		ApplySquareAverageFilter <<<dimGrid, dimBlock>>> (
				pitched_input_data, pitched_output_data,
				width, height,
				padded_width, padded_height,
				input_pitch, output_pitch
			);
	}
	checkCuda(cudaEventRecord(stopEvent, 0));
	checkCuda(cudaEventSynchronize(stopEvent));
	checkCuda(cudaEventElapsedTime(&time, startEvent, stopEvent));
	time /= NUM_REPS;

	cout << "GPU time: " << time << endl;

	checkCuda(cudaMemcpy2D(
		gpu_output_data,
		width,
		pitched_output_data,
		output_pitch,
		width,
		height,
		cudaMemcpyDeviceToHost
	));

	// ********************************************************************************************************

	// check
	postprocess(cpu_output_data, gpu_output_data, size, time);

	__savePPM(cpu_resilt_file_name, cpu_output_data, width, height, channels);
	__savePPM(gpu_resilt_file_name, gpu_output_data, width, height, channels);
	
	checkCuda(cudaEventDestroy(startEvent));
	checkCuda(cudaEventDestroy(stopEvent));
	checkCuda(cudaFree(pitched_input_data));
	checkCuda(cudaFree(pitched_output_data));
	delete[] input_data;
	delete[] cpu_output_data;
	delete[] gpu_output_data;
}