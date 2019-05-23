#ifndef HOST_IMAGE_PROCESSING_H
#define HOST_IMAGE_PROCESSING_H

unsigned char* PadDataByOneByte(unsigned char *input_data, const int width, const int height);
unsigned char* PadDataByFourBytes(unsigned char *input_data, const int width, const int height);

void ApplySquareAverageFilter(
	unsigned char *input_matrix,
	unsigned char *output_matrix,
	const int width,
	const int height
);

void PrintMatrix(unsigned char *data, const int width, const int height);

#endif