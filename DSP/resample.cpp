#include <DSP/filter.h>
#include <DSP/resample.h>
#include <bmpAccess/bmpEngine.h>

#include <iostream>
using namespace std;

void upScale(GIMAGE *inImage, int scale, GIMAGE *upScaled) {
	int loc;
	for(int row = 0; row < inImage->height; row++)
		for(int col = 0; col < inImage->width; col++) {
			loc = scale*row*upScaled->width+scale*col;
			upScaled->imageData[loc] = inImage->imageData[row*inImage->width+col];
			for(int k2 = 1; k2 < scale; k2++)
				upScaled->imageData[loc+k2] = 0;
		}
	
	for(int row = 0; row < upScaled->height; row+=scale)
		for(int col = 0; col < upScaled->width; col++) {
			loc = row*upScaled->width+col;
			for(int k1=1; k1 < scale; k1++)
				upScaled->imageData[loc+k1*upScaled->width] = 0;
		}
}

void downScale(GIMAGE *inImage, int scale, GIMAGE *downScaled) {
	for(int row = 0; row < downScaled->height; row++)
		for(int col = 0; col < downScaled->width; col++)
			downScaled->imageData[row*downScaled->width+col] = inImage->imageData[scale*row*inImage->width+scale*col];
}

void resample(GIMAGE *inImage, int expandMag, int compressMag, GIMAGE *outImage) {
	//cout << expandMag << " " << compressMag <<  endl;

	int outHeight, outWidth;

	outHeight = inImage->height * expandMag;
	outWidth = inImage->width * expandMag;
	GIMAGE *upScaled = createImage(outWidth,outHeight,1);
	upScale(inImage,expandMag,upScaled);
	//writeImage("C:/Users/Gautam/Desktop/upScaled.bmp", upScaled);

	int filter_length = 7;
	int filter_start = -filter_length/2;
	GTYPE *filter_coefficients = new GTYPE[filter_length];
	if(expandMag > compressMag) SincFilter(expandMag, filter_start, filter_length, 1, filter_coefficients);
	else SincFilter(compressMag, filter_start, filter_length, 1, filter_coefficients);
	imFilter(upScaled, filter_coefficients, filter_start, filter_length, upScaled);
	//writeImage("C:/Users/Gautam/Desktop/upScaledFiltered.bmp", upScaled);

	outHeight = upScaled->height / compressMag;
	outWidth = upScaled->width / compressMag;
	//GIMAGE *downScaled = createImage(outWidth,outHeight,1);	
	downScale(upScaled,compressMag,outImage);
	//writeImage("C:/Users/Gautam/Desktop/downScaled.bmp", outImage);
	
	releaseImage(upScaled);
}
