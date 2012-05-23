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

	BuildFeature (".\\TestRepo\\00.Test_Images\\lena.bmp", -1, ".\\TestRepo\\01.Training");

	FEATURES train;
	FILE *featureFile = fopen (".\\TestRepo\\01.Training\\07.Angle_Keys\\train.bin","rb");	
	fread (&(train.FeatureVectorLength),sizeof (int), 1, featureFile);
	cout << train.FeatureVectorLength << endl;
	fread (&(train.Number_of_Features),sizeof (int), 1, featureFile);
	cout << train.Number_of_Features << endl;
	train.features = new FEATURE [ train.Number_of_Features ];

	for (int index = 0; index < train.Number_of_Features; index ++) {
		fread ( &(train.features[index].x), sizeof (double), 1 , featureFile);
		fread ( &(train.features[index].y), sizeof (double), 1 , featureFile);
		fseek (featureFile, 2 * sizeof (double), SEEK_CUR); // Skip Reading scale and key orientation
		train.features[index].FeatureVector = new int[train.FeatureVectorLength];
		fread ( train.features[index].FeatureVector, sizeof (int), train.FeatureVectorLength , featureFile);
	}

	fclose (featureFile);


	FEATURES test;
	char *filename;
	COORDS* coordinateMappings;
	COORDS initial,final;
	for ( int frameIndex = 50; frameIndex < 51; frameIndex++ ) {

		// Extract Sift Features
		filename = new char[100];
		sprintf (filename, ".\\TestRepo\\00.Test_Images\\lena.bmp(%d).bmp",frameIndex);
		cout << endl << filename << endl;
		BuildFeature (filename, frameIndex, ".\\TestRepo\\02.Test");

		// Read SiftFeatures (if this works perfectly we could build this in RAM instead of writing to disk to make it faster
		sprintf (filename, ".\\TestRepo\\02.Test\\07.Angle_Keys\\testFeature(%d).bin",frameIndex);
		featureFile = fopen (filename,"rb");	
		fread (&(test.FeatureVectorLength),sizeof (int), 1, featureFile);
		fread (&(test.Number_of_Features),sizeof (int), 1, featureFile);
		
		test.features = new FEATURE [ test.Number_of_Features ];

		for (int featureIndex = 0; featureIndex < test.Number_of_Features; featureIndex ++) {
			fread ( &(test.features[featureIndex].x), sizeof (double), 1 , featureFile);
			fread ( &(test.features[featureIndex].y), sizeof (double), 1 , featureFile);
			fseek (featureFile, 2 * sizeof (double), SEEK_CUR); // Skip Reading scale and key orientation
			test.features[featureIndex].FeatureVector = new int[test.FeatureVectorLength];
			fread ( test.features[featureIndex].FeatureVector, sizeof (int), test.FeatureVectorLength , featureFile);
		}
		fclose (featureFile);

		coordinateMappings = findNearestNeighbor (train, test, threshold);
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

		delete filename;
	}	
/*
	// Garbage Collection: Train Feature
	for (int featureIndex = 0; featureIndex < test.Number_of_Features; featureIndex ++)
		delete train.features[featureIndex].FeatureVector;
	delete train.features;
*/
}