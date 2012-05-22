#ifndef FILTER_H
#define FILTER_H

#include <bmpAccess/bmpStructs.h>

void GaussianFilter (GTYPE mean, GTYPE stdev, int start, int length, GTYPE *filter_coefficients);
void GaussianFilter2 (GTYPE mean, GTYPE std, int start, int length, GTYPE *filter_coefficients);
void SincFilter (int scale, int start, int length, GTYPE multiplier, GTYPE *filter_coefficients);
void imFilter (GIMAGE *inImage, GTYPE *filter_coefficients, int filter_start, int filter_length, GIMAGE *outImage);

#endif