//Not Optimized

#include <bmpAccess/bmpEngine.h>
#include <DSP/filter.h>
#include <DSP/resample.h>
#include <DSP/Pyramids.h>

#include <stdio.h>
#include <iostream>
#include <math.h>
using namespace std;

 GIMAGE** GaussianPyramid(GIMAGE *inImage, GTYPE stepSize, int numLayers, char *PROJECT_FOLDER) {
	cout << "\t Gaussian Pyramid Formation Begins Now" << endl;

	GIMAGE **Pyramid = new GIMAGE*[numLayers];

	int filter_length = 9;
	int filter_start = -filter_length/2;
	GTYPE *filter_cofficients = new GTYPE[filter_length];
	GaussianFilter(0, (GTYPE)1.4142, filter_start, filter_length, filter_cofficients);
	
	char *filename = new char[100];

	cout << "\t   Level " << 1 << "/" << numLayers << endl;
	int outWidth, outHeight;
		
	outHeight = (int) floor((GTYPE) (inImage->height<<1));
	outWidth = (int) floor((GTYPE) inImage->width/0.5);
	//cout << outHeight << " " << outWidth << endl;	
	Pyramid[0] = createImage( outWidth, outHeight, 1 );	
	resample(inImage,2,1,Pyramid[0]);
	
	if(PROJECT_FOLDER != NULL) {
		sprintf(filename,"%s/01.Gaussian_Pyramid/Image_Gauss(%d).bmp",PROJECT_FOLDER,1);
		writeImage(filename,Pyramid[0]);
	}
	
	GIMAGE *tmpImage;
	for(int i=0; i < numLayers-1; i++) {	
		cout << "\t   Level " << i+2 << "/" << numLayers << endl;

		tmpImage = createImage(Pyramid[i]->width, Pyramid[i]->height,1);
		imFilter(Pyramid[i], filter_cofficients, filter_start, filter_length, tmpImage);

		outHeight = (int) floor((GTYPE) (Pyramid[i]->height<<1)/3);
		outWidth = (int) floor((GTYPE) Pyramid[i]->width/1.5);
	//	cout << outHeight << " " << outWidth << endl;
		Pyramid[i+1] = createImage( outWidth, outHeight, 1 );		
		resample(tmpImage,2,3,Pyramid[i+1]);
		releaseImage(tmpImage);

		if(PROJECT_FOLDER != NULL) {
			sprintf(filename,"%s/01.Gaussian_Pyramid/Image_Gauss(%d).bmp",PROJECT_FOLDER,i+2);
			writeImage(filename,Pyramid[i+1]);
		}		
	}	
		
	cout << "\t Gaussian Pyramid Formation Successfully Completed!\n" << endl;
	
	return Pyramid;
}

/*
 GIMAGE*** LaplacianPyramid(GIMAGE *inImage, GTYPE stepSize, int numLayers, char *PROJECT_FOLDER){
	cout << "\t Laplacian Pyramid Formation Begins Now" << endl;

	GIMAGE*** Pyramid = new GIMAGE**[3];
	Pyramid[0] = new GIMAGE*[numLayers];
	Pyramid[1] = new GIMAGE*[numLayers-1];
	Pyramid[2] = new GIMAGE*[numLayers-1];	
	
	int filter_length = 9;
	int filter_start = -4;
	GTYPE *filter_cofficients = new GTYPE[filter_length];
	GaussianFilter(0, 1.4142, filter_start, filter_length, filter_cofficients);
		
	char *filename = new char[100];	
	int outWidth, outHeight;
	
	cout << "\t   Level " << 1 << "/" << numLayers << endl;
	outHeight = (int) floor((double) (inImage->height<<1));
	outWidth = (int) floor((double) inImage->width/0.5);
	//cout << outHeight << " " << outWidth << endl;
	//Pyramid[0][0] = createImage( outWidth, outHeight, 1 );
	//resample(inImage,2,1,Pyramid[0][0]);
	Pyramid[0][0] = cloneImage(inImage);
	
	if(PROJECT_FOLDER != NULL) 
	{
		sprintf(filename,"%s/01.Gaussian_Pyramid/Image_Gauss(%d).bmp",PROJECT_FOLDER,1);
		writeImageNorm(filename,Pyramid[0][0]);
	}

	GIMAGE* tmpImage = new GIMAGE;
	for(int i=0; i < numLayers-1; i++) {	
		cout << "\t   Level " << i+2 << "/" << numLayers << endl;
		
		tmpImage = createImage(Pyramid[0][i]->width,Pyramid[0][i]->height,1);
		imFilter(Pyramid[0][i], filter_cofficients, filter_start, filter_length, tmpImage);

		outHeight = (int) floor((double) (Pyramid[0][i]->height<<1)/3);
		outWidth = (int) floor((double) Pyramid[0][i]->width/1.5);
		//	cout << outHeight << " " << outWidth << endl;			
		Pyramid[0][i+1] = createImage( outWidth, outHeight, 1 );
		resample(tmpImage,2,3,Pyramid[0][i+1]);	
		if(PROJECT_FOLDER != NULL) 
		{
			sprintf(filename,"%s/01.Gaussian_Pyramid/Image_Gauss(%d).bmp",PROJECT_FOLDER,i+2);
			writeImageNorm(filename,Pyramid[0][i+1]);
		}

		Pyramid[2][i] = createImage(Pyramid[0][i]->width,Pyramid[0][i]->height,1);
		for(int n1=0; n1 < Pyramid[0][i]->height; n1++)
			for(int n2=0; n2 < Pyramid[0][i]->width; n2++)
				Pyramid[2][i]->imageData[n1*Pyramid[0][i]->width+n2] = Pyramid[0][i]->imageData[n1*Pyramid[0][i]->width+n2] - tmpImage->imageData[n1*Pyramid[0][i]->width+n2];
		releaseImage(tmpImage);
		
		Pyramid[1][i] = createImage(outWidth,outHeight,1);
		resample(Pyramid[2][i],2,3,Pyramid[1][i]);
				
		if(PROJECT_FOLDER != NULL)
		{
			sprintf(filename,"%s/02.Laplacian_Pyramid/Image_Lapla(%d).bmp",PROJECT_FOLDER,i+1);
			writeImageNorm(filename,Pyramid[2][i]);
		}		
		
	}	
		
	cout << "\t Laplacian Pyramid Formation Successfully Completed!" << endl;

	return Pyramid;
}
*/

GIMAGE**** LaplacianPyramid(GIMAGE *inImage, GTYPE sigma, int octaves, int numBlurs, char *PROJECT_FOLDER){
	cout << "\t Laplacian Pyramid Formation Begins Now" << endl;

	GIMAGE**** Pyramid = new GIMAGE***[2];
	Pyramid[0] = new GIMAGE**[octaves];
	Pyramid[1] = new GIMAGE**[octaves];

	for(int j=0; j<octaves; j++) {
		Pyramid[0][j] = new GIMAGE*[numBlurs];
		Pyramid[1][j] = new GIMAGE*[numBlurs-1];		
	}
		

	int filter_length = 9;
	int filter_start = -filter_length/2;
	GTYPE *filter_coefficients = new GTYPE[filter_length];
	
		
	char *filename = new char[100];	
	GIMAGE* tmpImage = new GIMAGE;
	tmpImage = cloneImage(inImage);
	for(int i=0; i < octaves; i++) {
		//cout << "\t   Octave " << i+1 << "/" << octaves << "\t Blur " << 1 << "/" << numBlurs << endl;
		cout << "\t" << sigma*pow(2.0,(GTYPE)i) << " ";
		Pyramid[0][i][0] = createImage( tmpImage->width, tmpImage->height, 1 );
		GaussianFilter(0, sigma, filter_start, filter_length, filter_coefficients);
		imFilter(tmpImage, filter_coefficients, filter_start, filter_length, Pyramid[0][i][0]);
		if(PROJECT_FOLDER != NULL){
			sprintf(filename,"%s/01.Gaussian_Pyramid/Image_Gauss(%d%d).bmp",PROJECT_FOLDER,i,0);
			writeImageNorm(filename,Pyramid[0][i][0]);
		}

		for(int j=0; j < numBlurs-1; j++) {	
			//cout << "\t   Octave " << i+1 << "/" << octaves << "\t Blur " << j+2 << "/" << numBlurs << endl;
			cout << sigma*pow(2.0,(GTYPE)i+(j+1)/(GTYPE)(numBlurs-3)) << " ";

			Pyramid[0][i][j+1] = createImage( tmpImage->width, tmpImage->height, 1 );			
			GaussianFilter(0, sigma*pow(2.0,(GTYPE)(j+1)/(GTYPE)(numBlurs-3)), filter_start, filter_length, filter_coefficients);
			imFilter(tmpImage, filter_coefficients, filter_start, filter_length, Pyramid[0][i][j+1]);
			if(PROJECT_FOLDER != NULL){
				sprintf(filename,"%s/01.Gaussian_Pyramid/Image_Gauss(%d%d).bmp",PROJECT_FOLDER,i,j+1);
				writeImageNorm(filename,Pyramid[0][i][j+1]);
			}

			Pyramid[1][i][j] = createImage(tmpImage->width,tmpImage->height,1);
			for(int n1=0; n1 < tmpImage->height; n1++)
				for(int n2=0; n2 < tmpImage->width; n2++)
					Pyramid[1][i][j]->imageData[n1*tmpImage->width+n2] = Pyramid[0][i][j]->imageData[n1*Pyramid[0][i][j]->width+n2] - Pyramid[0][i][j+1]->imageData[n1*Pyramid[0][i][j+1]->width+n2];			
			if(PROJECT_FOLDER != NULL){
				sprintf(filename,"%s/02.Laplacian_Pyramid/Image_Lapla(%d%d).bmp",PROJECT_FOLDER,i,j);
				writeImageNorm(filename,Pyramid[1][i][j]);
			}
		}
		releaseImage(tmpImage);
		tmpImage = createImage((Pyramid[0][i][numBlurs-3]->width) / 2,(Pyramid[0][i][numBlurs-3]->height) / 2,1);
		resample(Pyramid[0][i][numBlurs-3], 1, 2, tmpImage);
		cout << endl;
	}
	releaseImage(tmpImage);

	cout << "\t Laplacian Pyramid Formation Successfully Completed!" << endl;

	return Pyramid;
}
