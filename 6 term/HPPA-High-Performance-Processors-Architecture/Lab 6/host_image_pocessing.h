#ifndef HOST_IMAGE_PROCESSING_H
#define HOST_IMAGE_PROCESSING_H

typedef struct pixel
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
};

pixel* PadDataByOnePixel(pixel *input_data, const int width, const int height);

void ApplySquareAverageFilter(
	pixel *input_matrix,
	pixel *output_matrix,
	const int width,
	const int height
);

void PrintMatrix(unsigned char *data, const int width, const int height);

#endif