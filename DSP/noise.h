#ifndef NOISE_H
#define NOISE_H

#include <bmpAccess/bmpStructs.h>

double* inverse_cdf (double domainInit, double domainEnd, int domainlen, double *pdf);
void addWhiteNoise (GIMAGE *inImage, int cdf_length, double *noise_cdf_inverse, GIMAGE *outImage);

#endif