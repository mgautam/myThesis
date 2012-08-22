#include <Matrix/matrix.h>
#include <bmpAccess/bmpEngine.h>
#include <DSP/filter.h>
#include <DSP/noise.h>
#include <AffineTransformer/frames.h>

#include <stdio.h>
#include <math.h>

#include <string.h>

#define SCALE 1
// NOISE_AMP must be between 0(=128) to 1/2 (=255)
#define NOISE_AMP 0.1
#define NOISE_LENGTH 100.0

void createFrames (char *fileName, char *FramesFolder, int numFrames, bool addNoise, FILE *logFile, bool color_frames, char *backgroundFile) {
	double translation [2] = {0.0,0.0};//{-output->width/2,-output->height/2};

	IMAGE* input;
	if (color_frames) input = readRGB (fileName);
	else input = readGrey(fileName);
	
	IMAGE* background;
	if (backgroundFile == 0)
	{
		background = createimage(input->width, input->height, input->numColors);
		memset (background->imageData,255,background->width*background->height*background->numColors);
	}
	else
	{
		if (color_frames) background = readRGB(backgroundFile);
		else background = readGrey(backgroundFile);		
	}
	
	IMAGE* output;		
	double theta,sintheta,costheta;
	double rowIn,colIn;
	double temp_rowIn, temp_colIn;
	char filename[100];
	
	double *pdf = new double [NOISE_LENGTH];
	GaussianFilter (NOISE_LENGTH/2, (double)NOISE_LENGTH/3.0, 0, NOISE_LENGTH, pdf); // Gaussian white noise
	double *inv_cdf = inverse_cdf (-NOISE_AMP, NOISE_AMP, NOISE_LENGTH, pdf);
	
	for (int iter = 0; iter < numFrames; iter++) 
	{	
		output =  cloneImage (background);
		
		theta = 2*3.14*(double)iter/(double)numFrames;
		
		sintheta = sin(theta);
		costheta = cos(theta);

		unsigned char calculated_pixel;
		for (int colOut = -output->height/2; colOut < output->height/2; colOut++)
			for (int rowOut = -output->width/2; rowOut < output->width/2; rowOut++) {
				temp_rowIn = rowOut - translation[0];
				temp_colIn = colOut - translation[1];
				rowIn = (temp_rowIn*costheta - temp_colIn * sintheta)/SCALE ;
				colIn = (temp_rowIn*sintheta + temp_colIn * costheta)/SCALE ;
	
		
				if (rowIn < input->height/2 && rowIn >= -input->height/2)
					if (colIn < input->width/2 && colIn >= -input->width/2)
					{
						for (int colorIndex = 0; colorIndex < input->numColors; colorIndex++)
						{
							calculated_pixel = input->imageData[(((int)rowIn+input->height/2)*input->width+((int)colIn+input->width/2))*input->numColors+colorIndex];
							if ( calculated_pixel != 0 ) // Transparency instead of white
								output->imageData[((rowOut+output->height/2)*output->width+colOut+output->width/2)*output->numColors+colorIndex] = calculated_pixel;
						}
					}				
			}

		if (addNoise)
			addWhiteNoise (output, NOISE_LENGTH, inv_cdf, output);

		sprintf (filename,"%s/%d.bmp",FramesFolder,iter);
		fprintf (logFile, "%s\n", filename);
		writeImage (filename, output);

		//translation[0] += (double)output->height/(double)numFrames;//sqrt (pow ((double)output->width,2) + pow ((double)output->height,2))
		//translation[1] = translation[0];
		releaseImage (output);
	}

	releaseImage (input);
}