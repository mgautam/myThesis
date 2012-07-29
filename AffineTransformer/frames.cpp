#include <Matrix/matrix.h>
#include <bmpAccess/bmpEngine.h>
#include <AffineTransformer/frames.h>

#include <stdio.h>
#include <math.h>

#define OUT_WIDTH 512
#define OUT_HEIGHT 512

void createFrames (char *fileName, char *FramesFolder, int *translation, int numFrames) {

	GIMAGE* input = Gtype(readGrey(fileName));
	GIMAGE* output = createImage(OUT_WIDTH, OUT_HEIGHT, input->numColors);

	double theta,sintheta,costheta;
	double rowIn,colIn;
	double rowOut, colOut;
	char filename[100];
	for (int iter = 0; iter < numFrames; iter++) {
		
		for (rowIn = 0; rowIn < OUT_HEIGHT; rowIn++)
			for (colIn = 0; colIn < OUT_WIDTH; colIn++)
				output->imageData[((int)rowIn)*OUT_WIDTH+(int)colIn] = 0.49; // 0.49 -> White Background; +-0.5-> Black Background


		theta = 2*3.14*(double)iter/(double)numFrames;
		
		sintheta = sin(theta);
		costheta = cos(theta);

		for (int y = -OUT_HEIGHT/2; y < OUT_HEIGHT/2; y++)
			for (int x = -OUT_WIDTH/2; x < OUT_WIDTH/2; x++) {
				rowOut = y - translation[1];
				colOut = x - translation[0];
				rowIn = rowOut*costheta - colOut * sintheta;
				colIn = rowOut*sintheta + colOut * costheta;
		
		
				if (rowIn < input->height/2 && rowIn >= -input->height/2)
					if (colIn < input->width/2 && colIn >= -input->width/2)
						output->imageData[((int)rowOut+OUT_HEIGHT/2)*OUT_WIDTH+(int)colOut+OUT_WIDTH/2] 
														= input->imageData[((int)rowIn+input->height/2)*input->width+((int)colIn+input->width/2)];					
			}

		sprintf (filename,"%s/%d.bmp",FramesFolder,iter);
		cout << filename << endl;
		writeImage (filename, output);
	}

	releaseImage (input);
	releaseImage (output);
}