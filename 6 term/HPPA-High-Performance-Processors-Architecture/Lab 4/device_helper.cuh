#ifndef DEVICE_HELPER_CUH
#define DEVICE_HELPER_CUH

#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>

const int MATRIX_TILE = 16;
const int NUM_REPS = 1000;
//const int BLOCK_HEGHT = 16;

__global__ void __gpu_MirrorMatrix(short* input_data, short* output_data, int width, int height);
__global__ void __gpu_RotateMatrix(short* input_data, short* output_data, int width, int height);

#endif DEVICE_HELPER_CUH