#include <API/videoProcessor.h>

#include <Features/featureStruct.h>
#include <Features/FeatureEngine.h>
#include <Features/matchFeatures.h>
#include <ModelAffine/coordinates.h>
#include <ModelAffine/fitAffineMatrix.h>
#include <MotionExtract/MotionExtract.h>

#include <iostream>
using namespace std;

void processFrames (double threshold, FILE *RotationDataFile, bool printInfo) {

	
	FEATURES train = BuildFeature (".\\TestRepo\\00.Test_Images\\lena.bmp");;
	

	FEATURES test;
	char filename[100];
	COORDS* coordinateMappings;
	COORDS initial,final;
	for ( int frameIndex = 0; frameIndex < 100; frameIndex++ ) {
		sprintf (filename, ".\\TestRepo\\00.Test_Images\\Frames\\%d.bmp",frameIndex);
		test = BuildFeature (filename);	


		coordinateMappings = findNearestNeighbor (train,test,threshold);
		initial = coordinateMappings[0];
		final = coordinateMappings[1];
		
		if (printInfo) {
			cout << "Number of Features: " << (int) test.Number_of_Features  << "\t";
			cout << "Below Threshold Features: " << (int)final.Number_of_Coordinates << endl;
			cout << "Matches:" << endl;
			for (int matchIndex = 0; matchIndex < final.Number_of_Coordinates; matchIndex++) {
				cout << "(" << initial.x[matchIndex] << "," << initial.y[matchIndex] << ")";
				cout << " => ";
				cout << "(" << final.x[matchIndex] << "," << final.y[matchIndex] << ")";
				cout << " : ";
				cout << final.scores[matchIndex];
				cout << endl;
			}
		}

		if (printInfo) cout << endl << endl << "AffineFit:" << endl;
		MATRIX affine = fitAffineMatrix ( initial, final );
		if (printInfo) cout << affine << endl;

		printf ("Frame: %2d  ", frameIndex);
		showMotion (affine,  RotationDataFile);

		// Garbage Collection: Test Feature
		for (int featureIndex = 0; featureIndex < test.Number_of_Features; featureIndex ++)
			delete test.features[featureIndex].FeatureVector;
		delete test.features;

		delete coordinateMappings->x;
		delete coordinateMappings->y;
		delete coordinateMappings->scores;
		delete coordinateMappings;

	}	

	// Garbage Collection: Train Feature
/*
	for (int featureIndex = 0; featureIndex < test.Number_of_Features; featureIndex ++)
		delete train.features[featureIndex].FeatureVector;
	delete train.features;
*/

}