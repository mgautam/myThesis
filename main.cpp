#include <bmpAccess/testImageEngine.h>
#include <DSP/testDSP.h>
#include <DSP/testnoise.h>
#include <CriticalPoints/testCriticalPoints.h>
#include <Matrix/testMatrix.h>
#include <AffineTransformer/testFrames.h>
#include <AffineTransformer/testBlocks.h>
#include <ModelAffine/testAffineFit.h>
#include <MotionExtract/testMotionExtract.h>
#include <Features/testMatchFeatures.h>

#include <bmpAccess/bmpEngine.h>
#include <AffineTransformer/blocks.h>
#include <API/videoProcessor.h>
#include <AffineTransformer/frames.h>
#include <ModelAffine/coordinates.h>
#include <ModelAffine/fitAffineMatrix.h>
#include <MotionExtract/MotionExtract.h>

#include <iostream>
using namespace std;

#include <stdlib.h>

extern int TrainObject_x;
int TrainObject_x = 256;
extern int TrainObject_y;
int TrainObject_y = 256;

int main (int argc, char **argv)
{
	//Create Rotation & Translation of smaller objects in a larger image
	//testImageEngine ();
	//testNoise ();
	//testDSP ();
	//testCriticalPoints ();
	//testMatrix ();
	//testFrames ();
	//testBlocks ();
	//testAffineModel ();
	//testMotionExtract ();
	//testMatchFeatures ();
	
	
	int x[4] = {0,0,512,512};//{156,356,356,156};
	int y[4] = {0,512,0,512};//{156,156,356,356};

	IMAGE *inImage = readRGB ("./TestRepo/00.Test_Images/lena.bmp");
	IMAGE *train = extract_selection ( 4, x, y, inImage); 
	writeImage ("./TestRepo/01.Training/train.bmp", train);
	releaseImage (inImage);
	//releaseImage (train);

	createFrames ("./TestRepo/00.Test_Images/lena.bmp", "./TestRepo/02.Test/00.Frames", 100);//,"./TestRepo/00.Test_Images/lena.bmp");

	MATRIX *selection_box = new MATRIX (4,3);
	double **dataptr = selection_box->getDataPtr();
	for (int col = 0; col < selection_box->getWidth(); col++)
	{
		dataptr[col][0] = x[col] - TrainObject_x;
		dataptr[col][1] = y[col] - TrainObject_y;
		dataptr[col][2] = 1;
	}

	FILE *angles = fopen ("./TestRepo/02.Test/estimated_rotation.bin","wb");
	FILE *logFile = fopen ("./TestRepo/log.txt","w");
	processFrames (selection_box,-1,angles);//(stdout, -1, angles);
	fclose (logFile);
	fclose (angles);
	
	system ("pause");

	return 0;
}