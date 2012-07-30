#include <DSP/noise.h>
#include <stdio.h>
#include <stdlib.h>

double* inverse_cdf (double domainInit, double domainEnd, int domainlen, double *pdf) {
	double *inv_cdf = new double [domainlen];
	double stepSize = (domainEnd - domainInit) / (double) (domainlen-2);// Think more about why -2
	
	double *cdf = new double [domainlen];

	cdf [0] = pdf [0];//not just zero
	for (int i = 1; i < domainlen; i++) {		
		cdf [i] = cdf [i-1] + pdf [i];
		//printf ("%d -> %lf | %lf\n",i,pdf [i], cdf[i]);
	}

	/*
	// Normalization
	for (int i = 0; i < domainlen; i++) {
		cdf [i] /= cdf [domainlen-1];
		printf ("%lf\n",cdf[i]);
	}
	*/

	for (int i = 0, j = 0; i < domainlen; i++) {
		for (; j < domainlen; j++) {
			//printf ("%d -> %lf | %lf\n",j,cdf[j],(double)i/(double)domainlen);
			if (cdf [j] > (double)i/(double)domainlen) {
				inv_cdf [i] = domainInit + stepSize * (double) j;
				printf ("%d -> %lf\n\n",i,inv_cdf[i]);
				//j--;//Not Required
				break;
			}
		}
	}

	delete cdf;

	return inv_cdf;
}

void addWhiteNoise (GIMAGE *inImage, int cdf_length, double *noise_cdf_inverse, GIMAGE *outImage) {
	for (int row = 0; row < inImage->height; row++)
		for (int col = 0; col < inImage->width; col++) {
			outImage->imageData[row*inImage->width+col] = inImage->imageData[row*inImage->width+col]+ noise_cdf_inverse[(int)( ( (double)rand ()/(double)RAND_MAX) * cdf_length)];
		}
}