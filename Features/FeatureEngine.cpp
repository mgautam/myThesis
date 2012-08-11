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

#include <Features/Magnor.h>

#define sigma 1/sqrt(2.0)
#define NUM_OCTAVES 4
#define NUM_BLURS 5

void BuildFeature (char* srcFile, int frameIndex, char* ProjectFolder) {
	
	IMAGE *visual = readGrey(srcFile);
	GIMAGE *gvisual = Gtype(visual);
	releaseImage (visual);

	GIMAGE *inImage = createImage(2*gvisual->width,2*gvisual->width,1);
	resample(gvisual,2,1,inImage);
	releaseImage (gvisual);

	GIMAGE**** Pyramid = LaplacianPyramid(inImage, sigma,  NUM_OCTAVES, NUM_BLURS, ProjectFolder);
	releaseImage(inImage);

	GIMAGE *lowImage,*highImage;
	IMAGE *extremeImage;
	char fileName[100];//44
	int numExtrema=0,totKeys =0,numKeys;

	for(int i = 0; i < NUM_OCTAVES; i++)
	{
		cout << "\t   Octave " << i+1 << "/" << NUM_OCTAVES ;
		for(int j = 1; j < NUM_BLURS-2; j++)
		{

			cout << "\t Blur " << j << "/" << NUM_BLURS;

			highImage = Pyramid[1][i][j-1];

			lowImage = Pyramid[1][i][j+1];

			extremeImage = createimage( Pyramid[1][i][j]->width, Pyramid[1][i][j]->height , 1 );
			numExtrema += findExtrema(Pyramid[1][i][j],lowImage,highImage,extremeImage);
			sprintf(fileName,"%s\\03.Extreme_Pyramid\\Image_Xterm(%d%d).bmp",ProjectFolder,i,j);
			writeImage(fileName,extremeImage);

			inImage = Pyramid[0][i][j];//j-1?
			totKeys += featureCalc(inImage,extremeImage,i,j,ProjectFolder);
					
			//visualExtreme(extremeImage, i,j,ProjectFolder);
			//visual2(inImage->width, inImage->height, i,j,ProjectFolder);	
			releaseImage(extremeImage);			
		}
		cout << endl;
	}

	// Garbage collection:Pyramid
	releaseLaplacianPyramid (Pyramid, NUM_OCTAVES, NUM_BLURS);

	writeAllFeatures(sigma,NUM_OCTAVES,NUM_BLURS,ProjectFolder,frameIndex);
	
	//visual3(*visual,ProjectFolder);
	cout << "Total Number of Keys: "<< totKeys << " Extrema: " << numExtrema << endl;
		
}
