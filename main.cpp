#include <bmpAccess/testImageEngine.h>
#include <DSP/testDSP.h>
#include <CriticalPoints/testCriticalPoints.h>
#include <Matrix/testMatrix.h>
#include <AffineTransformer/testFrames.h>
#include <ModelAffine/testAffineFit.h>
#include <MotionExtract/testMotionExtract.h>
#include <Features/testMatchFeatures.h>

#include <API/videoProcessor.h>

#include <ModelAffine/coordinates.h>
#include <ModelAffine/fitAffineMatrix.h>
#include <MotionExtract/MotionExtract.h>
#include <iostream>
using namespace std;

int main (int argc, char **argv) {
	//testImageEngine ();
	//testDSP ();
	//testCriticalPoints ();
	//testMatrix ();
	// testFrames ();
	//testAffineModel ();
	//testMotionExtract ();
	//testMatchFeatures ();
/*
	COORDS initial,final;
	
	FILE *matchFile = fopen ("C:\\Users\\Gautam\\Desktop\\Project\\Matlab Code\\siftmatches.bin","rb");
	
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


	FILE *outputRotation;
	char FileName[100];
	for (int Threshold = 51; Threshold < 900; Threshold+=10) {
		sprintf (FileName, "C:\\Users\\Gautam\\Desktop\\Project\\02.Test\\Rotation\\Threshold(%d).bin",Threshold);
		outputRotation = fopen (FileName, "wb");
		processFrames (Threshold,outputRotation);
		fclose (outputRotation);
	}
	
	
	return 0;
}