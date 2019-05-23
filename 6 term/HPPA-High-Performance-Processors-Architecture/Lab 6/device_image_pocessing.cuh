#ifndef DEVICE_IMAGE_PROCESSING_H
#define DEVICE_IMAGE_PROCESSING_H

const int AMOUNT_OF_THREADS_X = 32;
const int AMOUNT_OF_THREADS_Y = 16;

__global__ void ApplySquareAverageFilter(
	unsigned char *input_data,
	unsigned char *output_data,
	const int width,
	const int height,
	const int padded_width,
	const int padded_height,
	const int in_pitch,
	const int out_pitch
);

#endif