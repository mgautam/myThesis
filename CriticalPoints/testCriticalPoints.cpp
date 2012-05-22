#include <DSP/resample.h>
#include <DSP/Pyramids.h>
#include <CriticalPoints/Extreme.h>
#include <CriticalPoints/testCriticalPoints.h>
#include <bmpAccess/bmpEngine.h>

#include <stdio.h>
#include <iostream>
using namespace std;

#define TRAIN_FOLDER "C:\\Users\\Gautam\\Desktop\\Project\\01.Training"
#define TEST_FOLDER "C:\\Users\\Gautam\\Desktop\\Project\\02.Test"
#define sigma 1/1.4142
#define numOctaves 4
#define numBlurs 5

void testCriticalPoints (void) {
	char *srcFile = "C:\\Users\\Gautam\\Desktop\\Project\\00.Test_Images\\lena.bmp";
	char *ProjectFolder = TRAIN_FOLDER;
	IMAGE *visual;
	GIMAGE *inImage,*lowImage,*highImage;
	IMAGE *extremeImage;
	char fileName[100];//44
	int numExtrema=0;
	
	
	inImage = createImage(1024,1024,1);
	visual = readGrey(srcFile);
	resample(Gtype(visual),2,1,inImage);
	//inImage = Gtype(readGrey(imageName));//"C:\\Users\\Gautam\\Desktop\\Project\\00.Test_Images\\testSquare.bmp"));
	GIMAGE**** Pyramid = LaplacianPyramid(inImage, sigma,  numOctaves, numBlurs, ProjectFolder);
	releaseImage(inImage);

	for(int i = 0; i < numOctaves; i++)
		for(int j = 1; j < numBlurs-2; j++){

			cout << "\t   Octave " << i+1 << "/" << numOctaves << "\t Blur " << 1 << "/" << numBlurs << endl;

			highImage = Pyramid[1][i][j-1];

			lowImage = Pyramid[1][i][j+1];

			extremeImage = createimage( Pyramid[1][i][j]->width, Pyramid[1][i][j]->height , 1 );
			numExtrema += findExtrema(Pyramid[1][i][j],lowImage,highImage,extremeImage);
			sprintf(fileName,"%s\\03.Extreme_Pyramid\\Image_Xterm(%d%d).bmp",ProjectFolder,i,j);
			writeImage(fileName,extremeImage);
		}

}