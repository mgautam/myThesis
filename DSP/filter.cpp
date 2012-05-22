#include <DSP/filter.h>
#include <bmpAccess/bmpEngine.h>

#include <iostream>
#include <math.h>
using namespace std;

void GaussianFilter(GTYPE mean, GTYPE stdev, int start, int length, GTYPE *filter_coefficients) {
	GTYPE Norm = 0;
	for(int index = start; index < length+start; index++)	{
		filter_coefficients[index-start] = exp(-pow((GTYPE)(index-mean),2)/(2.0*pow(stdev,2)))/(stdev*sqrt(2.0*pi));
		//filter_coefficients[index-start] = (-(index-mean)*exp(-pow((double)index-mean,2)/(2*pow(stdev,2))))/(sqrt(2*pi)*stdev);
		Norm += filter_coefficients[index-start];
		//cout << filter_coefficients[index-start] << "   ";
	}
	for(int index = 0; index < length; index++) 
		filter_coefficients[index] /= Norm;
	//cout << endl << Norm << endl;
}

void GaussianFilter2(GTYPE mean, GTYPE std, int start, int length, GTYPE *filter_coefficients) {
	GTYPE *filter_1D = new GTYPE[length];
	GaussianFilter(mean,std,start,length,filter_1D);
	for(int row = 0; row < length; row++)
		for(int col = 0; col < length; col++)
			filter_coefficients[row*length+col] = filter_1D[row] * filter_1D[col];
	delete filter_1D;
}

void SincFilter(int scale, int start, int length, GTYPE multiplier, GTYPE *filter_coefficients){
	GTYPE Norm = 0;
	GTYPE angFreq = pi/scale;
	for(int index = start; index < length+start; index++) {
		if(index == 0) filter_coefficients[index-start] = 1.0/(GTYPE)scale;//2.0/(double)scale;
		else filter_coefficients[index-start] = sin(angFreq*index)/(pi*index);//(angFreq*n);/
		Norm += filter_coefficients[index-start];
		//cout << filter_coefficients[index-start] << "   ";
	}
	//cout << endl;
	for(int index = 0; index < length; index++) {
		filter_coefficients[index] *= multiplier/Norm;	
		//cout << filter_coefficients[index] << "   ";
	}
	//cout << endl << Norm << endl;
}

void imFilter(GIMAGE *inImage, GTYPE *filter_coefficients, int filter_start, int filter_length, GIMAGE *outImage) {
	
	GTYPE *mid_outImage = new GTYPE[inImage->width*inImage->height];
	for(int row = 0; row < inImage->height; row++)
		for(int col = 0; col < inImage->width; col++)
			mid_outImage[row*inImage->width+col] = 0;
	
	for(int row = 0; row < inImage->height; row++) {
		for(int col = 0; col < -filter_start; col++) {
			for(int subCol = filter_start; subCol <= col; subCol++)
				mid_outImage[row*inImage->width+col] += inImage->imageData[row*inImage->width+col-subCol]*filter_coefficients[subCol-filter_start];
			for(int subCol = col+1; subCol < filter_start+filter_length; subCol++)
				mid_outImage[row*inImage->width+col] += inImage->imageData[row*inImage->width-col+subCol]*filter_coefficients[subCol-filter_start];//Odd Symmetry
		}


		for(int col = -filter_start; col < inImage->width+filter_start; col++)
			for(int subCol = filter_start; subCol < filter_start+filter_length; subCol++)
				mid_outImage[row*inImage->width+col] += inImage->imageData[row*inImage->width+col-subCol]*filter_coefficients[subCol-filter_start];

		for(int col = inImage->width+filter_start; col < inImage->width; col++) {
			for(int subCol = filter_start; subCol <= col-(inImage->width); subCol++)
				mid_outImage[row*inImage->width+col] += inImage->imageData[(row+2)*inImage->width-(col-subCol+1)]*filter_coefficients[subCol-filter_start];//odd symmetry			
			for(int subCol = col-inImage->width+1; subCol < filter_start+filter_length; subCol++)
				mid_outImage[row*inImage->width+col] += inImage->imageData[row*inImage->width+col-subCol]*filter_coefficients[subCol-filter_start];
		}
	}
	
	
	for(int row = 0; row < inImage->height; row++)
		for(int col = 0; col < inImage->width; col++)
			outImage->imageData[row*inImage->width+col] = 0;

	for(int col = 0; col < inImage->width; col++) {
		for(int row = 0; row < -filter_start; row++) {
			for(int subRow = filter_start; subRow <= row; subRow++)
				outImage->imageData[row*inImage->width+col] += mid_outImage[(row-subRow)*inImage->width+col]*filter_coefficients[subRow-filter_start];
			for(int subRow=row+1; subRow < filter_start+filter_length; subRow++)
				outImage->imageData[row*inImage->width+col] += mid_outImage[(subRow-row)*inImage->width+col]*filter_coefficients[subRow-filter_start];//odd symmetry
		}

		for(int row = -filter_start; row < inImage->height+filter_start; row++)
			for(int subRow = filter_start; subRow < filter_start+filter_length; subRow++)
				outImage->imageData[row*inImage->width+col] += mid_outImage[(row-subRow)*inImage->width+col]*filter_coefficients[subRow-filter_start];

		for(int row = inImage->height+filter_start; row < inImage->height; row++) {
			for(int subRow = filter_start; subRow <= row-inImage->height ; subRow++)
				outImage->imageData[row*inImage->width+col] += mid_outImage[(2*inImage->height-(row-subRow+1))*inImage->width+col]*filter_coefficients[subRow-filter_start];//odd symmetry
			for(int subRow = row-inImage->height+1; subRow < filter_start+filter_length ; subRow++)
				outImage->imageData[row*inImage->width+col] += mid_outImage[(row-subRow)*inImage->width+col]*filter_coefficients[subRow-filter_start];
		}
	}
	delete mid_outImage;
}
