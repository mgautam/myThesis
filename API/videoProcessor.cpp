#include <API/videoProcessor.h>
#include <bmpAccess/bmpEngine.h>
#include <Features/featureStruct.h>
#include <Features/matchFeatures.h>
#include <ModelAffine/coordinates.h>
#include <ModelAffine/fitAffineMatrix.h>
#include <MotionExtract/MotionExtract.h>

#include <iostream>
using namespace std;

void processFrames (double threshold, FILE *RotationDataFile, bool printInfo) {
	FEATURES train;
	FILE *featureFile = fopen (".\\TestRepo\\01.Training\\07.SIFT_Keys\\train.bin","rb");	
	fread (&(train.FeatureVectorLength),sizeof (int), 1, featureFile);
	fread (&(train.Number_of_Features),sizeof (int), 1, featureFile);
	
	train.features = new FEATURE [ train.Number_of_Features ];

	char filename[100];
	sprintf (filename,".\\TestRepo\\00.Test_Images\\checkeredball.bmp");
	cout << filename << endl;
	GIMAGE *trainImage = Gtype (readGrey(filename));

	for (int index = 0; index < train.Number_of_Features; index ++) {
		fread ( &(train.features[index].x), sizeof (double), 1 , featureFile);
		fread ( &(train.features[index].y), sizeof (double), 1 , featureFile);
		//cout << train.features[index].x << " " << train.features[index].y << endl;

		if (trainImage->imageData[(int)train.features[index].y * trainImage->width + (int)train.features[index].x] > 0.48)
			trainImage->imageData[(int)train.features[index].y * trainImage->width + (int)train.features[index].x] = -0.48;
		else
			trainImage->imageData[(int)train.features[index].y * trainImage->width + (int)train.features[index].x] = 0.49;

		train.features[index].FeatureVector = new char[train.FeatureVectorLength];
		fread ( train.features[index].FeatureVector, sizeof (char), train.FeatureVectorLength , featureFile);
	}

	fclose (featureFile);

	sprintf (filename,".\\TestRepo\\01.Training\\keyImage.bmp");
	cout << filename << endl;
	writeImage (filename, trainImage);
	releaseImage (trainImage);



	FEATURES test;
	GIMAGE *testImage;
	//char filename[100];
	COORDS* coordinateMappings;
	COORDS initial,final;
	for ( int frameIndex = 0; frameIndex < 100; frameIndex++ ) {
		sprintf (filename, ".\\TestRepo\\02.Test\\07.SIFT_Keys\\testfeature(%d).bin",frameIndex);
		featureFile = fopen (filename,"rb");	
		fread (&(test.FeatureVectorLength),sizeof (int), 1, featureFile);
		fread (&(test.Number_of_Features),sizeof (int), 1, featureFile);
		
		test.features = new FEATURE [ test.Number_of_Features ];

		sprintf (filename,".\\TestRepo\\00.Test_Images\\Frames\\%d.bmp",frameIndex);
		cout << filename << endl;
		testImage = Gtype (readGrey(filename));

		for (int featureIndex = 0; featureIndex < test.Number_of_Features; featureIndex ++) {
			fread ( &(test.features[featureIndex].x), sizeof (double), 1 , featureFile);
			fread ( &(test.features[featureIndex].y), sizeof (double), 1 , featureFile);

			if (testImage->imageData[(int)test.features[featureIndex].y * testImage->width + (int)test.features[featureIndex].x] > 0.48)
				testImage->imageData[(int)test.features[featureIndex].y * testImage->width + (int)test.features[featureIndex].x] = -0.48;
			else
				testImage->imageData[(int)test.features[featureIndex].y * testImage->width + (int)test.features[featureIndex].x] = 0.49;

			test.features[featureIndex].FeatureVector = new char[test.FeatureVectorLength];
			fread ( test.features[featureIndex].FeatureVector, sizeof (char), test.FeatureVectorLength , featureFile);
		}
		fclose (featureFile);

		sprintf (filename,".\\TestRepo\\02.Test\\KeysinFrame\\keyImage(%d).bmp",frameIndex);
		cout << filename << endl;
		writeImage (filename, testImage);
		releaseImage (testImage);

		coordinateMappings = findNearestNeighbor (train,test, threshold);
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

		//printf ("Frame: %2d  ", frameIndex);
		printf ("Frame: %2d  Actual: %6.3lf  ", frameIndex,2*3.14*(double)frameIndex/(double)100);
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