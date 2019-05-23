#ifndef HOST_HELPER_H
#define HOST_HELPER_H

// Linear Position in Matrix
#define LPIM(x, y, width) (y * width + x)
// Mirrored Linear Position in Matrix
#define MLPIM(x, y, width) ((y + 1) * width - x - 1)

void MirrotAndRotateMatrix(short* input_matrix, short* output_matrix, const int width, const int height);
void MirrorMatrix(short* in_matrix, short* out_matrix, const int width, const int height);
void ShowHostFunctionsWork(const int width, const int height);

#endif HOST_HELPER_H