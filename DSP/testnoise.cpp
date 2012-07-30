#include <DSP/noise.h>
#include <DSP/testnoise.h>

#include <bmpAccess/bmpEngine.h>
#include <DSP/filter.h>

#include <string.h>

// NOISE_AMP must be between 0(=128) to 1/2 (=255)
#define NOISE_AMP 0.1
#define NOISE_LENGTH 100.0

void testNoise (void) {
	
	GIMAGE* input = Gtype(readGrey("./TestRepo/00.Test_Images/lena.bmp"));
	GIMAGE* output = createImage(input->width,input->height,1);
	
	double *pdf = new double [NOISE_LENGTH];
	GaussianFilter (NOISE_LENGTH/2, (double)NOISE_LENGTH/3.0, 0, NOISE_LENGTH, pdf); // Gaussian white noise
	
	// Uniform White noise
	//for (int i = 0; i < NOISE_LENGTH; i++)
	// 	pdf [i] = 1.0/NOISE_LENGTH;
		
	// Impulse
	//for (int i = 0; i < NOISE_LENGTH; i++)
	//	pdf [i] = 0;
	//pdf [(int)NOISE_LENGTH-1] = 1;

	double *inv_cdf = inverse_cdf (-NOISE_AMP, NOISE_AMP, NOISE_LENGTH, pdf);
	
	addWhiteNoise (input, NOISE_LENGTH, inv_cdf, output);

	writeImage ("./TestRepo/noise.bmp", output);		
}