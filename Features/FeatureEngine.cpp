#include <stdio.h>
#include <iostream>
using namespace std;
#include <math.h>

#include <Features/FeatureEngine.h>

#include <bmpAccess/bmpStructs.h>
#include <bmpAccess/bmpEngine.h>

#include <DSP/resample.h>
#include <DSP/Pyramids.h>

#include <CriticalPoints/Extreme.h>

#include <Features/neighborSpy.h>

#define sigma 1/sqrt(2.0)
#define numOctaves 4
#define numBlurs 5

void BuildFeature (char* imFile, int frameIndex, char* ProjectFolder) {
	IMAGE *visual;
	GIMAGE *inImage,*lowImage,*highImage;
	IMAGE *extremeImage;
	char fileName[100];//44
	int numExtrema=0,totKeys =0,numKeys;

	
	inImage = createImage(1024,1024,1);
	visual = readGrey(imFile);
	resample(Gtype(visual),2,1,inImage);
	releaseImage (visual);

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

			inImage = Pyramid[0][i][j];//j-1?
			numKeys = magoriCalc(inImage,extremeImage,i,j,ProjectFolder);
			angleKeyCalc(extremeImage, numKeys, i,j,ProjectFolder);		
			
		
			//visualExtreme(extremeImage, i,j,ProjectFolder);
			//visual2(inImage->width, inImage->height, i,j,ProjectFolder);	
			releaseImage(extremeImage);
			totKeys += numKeys;
	}
		
	// Garbage collection:Pyramid
	for (int i = 0; i < numOctaves; i++) {
		int j =0;
		for(; j < numBlurs-2; j++) {
			releaseImage (Pyramid[0][i][j]);
			releaseImage (Pyramid[1][i][j]);
		}
		delete Pyramid[0][i][j]; // Gaussian Pyramid has one extra Layer
		delete Pyramid[0][i];
		delete Pyramid[1][i];			
	}
	delete Pyramid[0];
	delete Pyramid[1];
	delete Pyramid;

	writeAllSift(sigma,numOctaves,numBlurs,ProjectFolder,frameIndex);
	//visual3(*visual,ProjectFolder);
	cout << "Total Number of Keys: "<< totKeys << " Extrema: " << numExtrema << endl;
		
}
