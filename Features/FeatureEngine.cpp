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
#define NUM_OCTAVES 4
#define NUM_BLURS 5

// Frame Index whose intermediate images
// to be written to Disk
#define FRAME_TO_WRITE -1

void BuildFeature (char* imFile, int frameIndex, char* ProjectFolder, FILE *logFile) 
{
	IMAGE *visual = readGrey(imFile);
	GIMAGE *gvisual = Gtype(visual);
	releaseImage (visual);
	
	GIMAGE *inImage = createImage(2*gvisual->width,2*gvisual->width,1);
	resample(gvisual,2,1,inImage);
	releaseImage (gvisual);

	GIMAGE**** Pyramid;
	
	if (frameIndex == FRAME_TO_WRITE) // Write Pyramid to disk only for training image
		Pyramid = LaplacianPyramid(inImage, sigma,  NUM_OCTAVES, NUM_BLURS, logFile, ProjectFolder);
	else
		Pyramid = LaplacianPyramid(inImage, sigma,  NUM_OCTAVES, NUM_BLURS, logFile);

	releaseImage(inImage);



	fprintf (logFile, "\n\tBuilding Feature Keys...\n");

	GIMAGE *lowImage,*highImage;
	IMAGE *extremeImage;
	char fileName[100];//44
	int numExtrema=0,totKeys =0,numKeys;
	
	for(int i = 0; i < NUM_OCTAVES; i++)
	{
		fprintf (logFile, "\t   Octave %d/%d", i+1, NUM_OCTAVES) ;
		for(int j = 1; j < NUM_BLURS-2; j++)
		{

			fprintf (logFile, "\t Blur %d/%d", j, NUM_BLURS);

			highImage = Pyramid[1][i][j-1];

			lowImage = Pyramid[1][i][j+1];

			extremeImage = createimage( Pyramid[1][i][j]->width, Pyramid[1][i][j]->height , 1 );
			numExtrema += findExtrema(Pyramid[1][i][j],lowImage,highImage,extremeImage);

			if (frameIndex == FRAME_TO_WRITE) // Write Extrema file only for training
			{
				sprintf(fileName,"%s/03.Extreme_Pyramid/Image_Xterm(%d%d).bmp",ProjectFolder,i,j);
				writeImage(fileName,extremeImage);
			}

			inImage = Pyramid[0][i][j];//j-1?// This should also be laplacian right?No
			totKeys += featureCalc(inImage,extremeImage,i,j,ProjectFolder); // numKeys => Number of stable keys
			
			//visualExtreme(extremeImage, i,j,ProjectFolder);
			//visual2(inImage->width, inImage->height, i,j,ProjectFolder);	
			releaseImage(extremeImage);			
		}
		fprintf (logFile, "\n");
	}
		
	// Garbage collection:Pyramid
	releaseLaplacianPyramid (Pyramid, NUM_OCTAVES, NUM_BLURS);

	writeAllFeatures(sigma,NUM_OCTAVES,NUM_BLURS,ProjectFolder,frameIndex);
	//visual3(*visual,ProjectFolder);
	fprintf (logFile, "\tTotal Number of Keys: %d  Extrema: %d\n", totKeys, numExtrema);
}
