#include <bmpAccess/testImageEngine.h>
#include <DSP/testDSP.h>
#include <CriticalPoints/testCriticalPoints.h>
#include <Matrix/testMatrix.h>
#include <AffineTransformer/testFrames.h>
#include <AffineTransformer/testBlocks.h>
#include <ModelAffine/testAffineFit.h>
#include <MotionExtract/testMotionExtract.h>
#include <Features/testMatchFeatures.h>

#include <bmpAccess/bmpEngine.h>
#include <AffineTransformer/frames.h>
#include <AffineTransformer/blocks.h>
#include <API/videoProcessor.h>
#include <ModelAffine/coordinates.h>
#include <ModelAffine/fitAffineMatrix.h>
#include <MotionExtract/MotionExtract.h>

#include <iostream>
using namespace std;

extern int TrainObject_x;
int TrainObject_x = 256;
extern int TrainObject_y;
int TrainObject_y = 256;

int main (int argc, char **argv) {
	//testImageEngine ();
	//testDSP ();
	//testCriticalPoints ();
	//testMatrix ();
	//testBlocks ();
	//testFrames ();
	//testAffineModel ();
	//testMotionExtract ();
	//testMatchFeatures ();

	int x[4] = {0,0,512,512};//{156,356,356,156};
	int y[4] = {0,512,0,512};//{156,156,356,356};

/*
	IMAGE *inImage = readRGB ("./TestRepo/00.Test_Images/lena.bmp");
	IMAGE *train = extract_selection ( 4, x, y, inImage);
	writeImage ("./TestRepo/01.Training/train.bmp", train);
	releaseImage (inImage);
	releaseImage (train);

	createFrames ("./TestRepo/00.Test_Images/lena.bmp", "./TestRepo/02.Test/00.Frames", 100);//,"./TestRepo/00.Test_Images/lena.bmp");
*/
	
	MATRIX *selection_box = new MATRIX (4,3);
	double **dataptr = selection_box->getDataPtr();
	for (int col = 0; col < selection_box->getWidth(); col++)
	{
		dataptr[col][0] = x[col] - TrainObject_x;
		dataptr[col][1] = y[col] - TrainObject_y;
		dataptr[col][2] = 1;
	}


	FILE *outputRotation;
	char FileName[100];
	for (int Threshold = 100; Threshold < 150; Threshold+=10) {
		sprintf (FileName, "./TestRepo/02.Test/Rotation/Threshold(%d).bin",Threshold);
		outputRotation = fopen (FileName, "wb");
		processFrames (selection_box,Threshold,outputRotation,stdout);
		fclose (outputRotation);
	}


/*
	COORDS initial,final;
	
	FILE *matchFile = fopen ("./TestRepo/Matlab Code/siftmatches.bin","rb");
	
	fread ((void *) &initial.Number_of_Coordinates,sizeof (int),1,matchFile);
	final.Number_of_Coordinates = initial.Number_of_Coordinates;
	
	initial.x = new double[initial.Number_of_Coordinates];
	fread ((void *) initial.x,sizeof (double),initial.Number_of_Coordinates,matchFile);
	initial.y = new double[initial.Number_of_Coordinates];
	fread ((void *) initial.y,sizeof (double),initial.Number_of_Coordinates,matchFile);

	final.x = new double[final.Number_of_Coordinates];
	fread ((void *) final.x,sizeof (double),final.Number_of_Coordinates,matchFile);
	final.y = new double[final.Number_of_Coordinates];
	fread ((void *) final.y,sizeof (double),final.Number_of_Coordinates,matchFile);

	fclose (matchFile);


	cout << "length:" << (int)final.Number_of_Coordinates << endl;
	cout << "Matches:" << endl;
	for (int matchIndex = 0; matchIndex < final.Number_of_Coordinates; matchIndex++) {
		cout << "(" << final.x[matchIndex] << "," << final.y[matchIndex] << ")";
		cout << " => ";
		cout << "(" << initial.x[matchIndex] << "," << initial.y[matchIndex] << ")";
		cout << endl;
	}

	cout << endl << endl << "AffineFit:" << endl;
	MATRIX affine = fitAffineMatrix ( initial, final );
	cout << affine << endl;

	showMotion (affine);
*/

	system ("pause");
	return 0;
}