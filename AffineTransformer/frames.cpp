#include <Matrix/matrix.h>
#include <bmpAccess/bmpEngine.h>
#include <AffineTransformer/frames.h>

#include <stdio.h>
#include <math.h>

#define OUT_WIDTH 512
#define OUT_HEIGHT 512

#define SCALE 1



void createFrames (char *fileName, char *FramesFolder, int numFrames, char *backgroundFile) {
	double translation [2] = {-OUT_WIDTH/2,-OUT_HEIGHT/2};

	GIMAGE* input = Gtype(readGrey(fileName));
	GIMAGE* output;
	if (backgroundFile == 0)
		output = createImage(OUT_WIDTH, OUT_HEIGHT, input->numColors);
	
	double theta,sintheta,costheta;
	double rowIn,colIn;
	double temp_rowIn, temp_colIn;
	char filename[100];
	for (int iter = 0; iter < numFrames; iter++) {
	
		if (backgroundFile == 0)
			for (rowIn = 0; rowIn < OUT_HEIGHT; rowIn++)
				for (colIn = 0; colIn < OUT_WIDTH; colIn++)
					output->imageData[((int)rowIn)*OUT_WIDTH+(int)colIn] = 0.499; // 0.49 -> White Background; +-0.5-> Black Background
		else
			output =  Gtype(readGrey(backgroundFile));

		theta = 2*3.14*(double)iter/(double)numFrames;
		
		sintheta = sin(theta);
		costheta = cos(theta);

		GTYPE calculated_pixel;
		for (int colOut = -OUT_HEIGHT/2; colOut < OUT_HEIGHT/2; colOut++)
			for (int rowOut = -OUT_WIDTH/2; rowOut < OUT_WIDTH/2; rowOut++) {
				temp_rowIn = rowOut - translation[0];
				temp_colIn = colOut - translation[1];
				rowIn = (temp_rowIn*costheta - temp_colIn * sintheta)/SCALE ;
				colIn = (temp_rowIn*sintheta + temp_colIn * costheta)/SCALE ;
	
		
				if (rowIn < input->height/2 && rowIn >= -input->height/2)
					if (colIn < input->width/2 && colIn >= -input->width/2)
					{
						calculated_pixel = input->imageData[((int)rowIn+input->height/2)*input->width+((int)colIn+input->width/2)];
						if ( calculated_pixel < 0.496 ) // Transparency instead of white
							output->imageData[(rowOut+OUT_HEIGHT/2)*OUT_WIDTH+colOut+OUT_WIDTH/2] = calculated_pixel;
					}				
			}

		sprintf (filename,"%s/%d.bmp",FramesFolder,iter);
		cout << filename << endl;
		writeImage (filename, output);

		translation[0] += (double)OUT_HEIGHT/(double)numFrames;//sqrt (pow ((double)OUT_WIDTH,2) + pow ((double)OUT_HEIGHT,2))
		translation[1] = translation[0];
	}

	releaseImage (input);
	releaseImage (output);
}