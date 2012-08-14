#include <API/videoProcessor.h>
#include <bmpAccess/bmpEngine.h>
#include <Features/featureStruct.h>
#include <Features/FeatureEngine.h>
#include <Features/matchFeatures.h>
#include <ModelAffine/coordinates.h>
#include <ModelAffine/fitAffineMatrix.h>
#include <MotionExtract/MotionExtract.h>

#include <string.h>
#include <iostream>
using namespace std;

void processFrames (double threshold, FILE *RotationDataFile, FILE *logFile) {

	char *filename = new char[100];
	sprintf (filename,"./TestRepo/00.Test_Images/Frames/0.bmp");//subject.bmp
	
	BuildFeature (filename, -1, "./TestRepo/01.Training", logFile);

	FEATURES train;
	FILE *featureFile = fopen ("./TestRepo/01.Training/07.Feature_Keys/train.bin","rb");	
	fread (&(train.FeatureVectorLength),sizeof (int), 1, featureFile);
	fread (&(train.Number_of_Features),sizeof (int), 1, featureFile);
		
	fprintf (logFile, "\tTrain Feature Length as Read: %d\n", train.FeatureVectorLength);
	fprintf (logFile, "\tNumber of Train Features as Read: %d\n", train.Number_of_Features);

	train.features = new FEATURE [ train.Number_of_Features ];

	
	//fprintf (logFile,"%s\n", filename);
	IMAGE *trainImage = readGrey(filename);
	IMAGE *keyImage = createimage (trainImage->width,trainImage->height,trainImage->numColors);
	memset (keyImage->imageData,255,trainImage->width*trainImage->height*trainImage->numColors);

	for (int index = 0; index < train.Number_of_Features; index ++)
	{
		fread ( &(train.features[index].x), sizeof (double), 1 , featureFile);
		fread ( &(train.features[index].y), sizeof (double), 1 , featureFile);
		fseek (featureFile, 2 * sizeof (double), SEEK_CUR); // Skip Reading scale and key orientation

		int n1 =  (int)train.features[index].y;//Why Reflection?trainImage->height-1 -
		int n2 =  (int)train.features[index].x;//Why Reflection?trainImage->width-1 -		
		if (trainImage->imageData[n1 * trainImage->width + n2] > 250)
			trainImage->imageData[n1 * trainImage->width + n2] = 0;
		else
			trainImage->imageData[n1 * trainImage->width + n2] = 255;

		keyImage->imageData[n1 * keyImage->width + n2] = 0;

		train.features[index].FeatureVector = new int[train.FeatureVectorLength];
		fread ( train.features[index].FeatureVector, sizeof (int), train.FeatureVectorLength , featureFile);
	}

	fclose (featureFile);

	sprintf (filename,"./TestRepo/01.Training/keyImage.bmp");
	//logFile << filename << endl;
	writeImage (filename, trainImage);
	releaseImage (trainImage);

	sprintf (filename,"./TestRepo/01.Training/keyImage1.bmp");
	//logFile << filename << endl;
	writeImage (filename, keyImage);
	releaseImage (keyImage);



	FEATURES test;
	IMAGE* JustGrey;
	GIMAGE *testImage;	
	COORDS* coordinateMappings;
	COORDS initial,final;
	for ( int frameIndex = 1; frameIndex < 100; frameIndex+=1 ) {

		// Extract Sift Features
		//filename = new char[100];
		sprintf (filename, "./TestRepo/00.Test_Images/Frames/%d.bmp",frameIndex);
		cout << endl << filename << endl;
		BuildFeature (filename, frameIndex, "./TestRepo/02.Test");

		// Read SiftFeatures (if this works perfectly we could build this in RAM instead of writing to disk to make it faster
		sprintf (filename, "./TestRepo/02.Test/07.Feature_Keys/testFeature(%d).bin",frameIndex);
		featureFile = fopen (filename,"rb");	
		fread (&(test.FeatureVectorLength),sizeof (int), 1, featureFile);
		fread (&(test.Number_of_Features),sizeof (int), 1, featureFile);

		cout << "Test Feature Length as Read: " << test.FeatureVectorLength << endl;
		cout << "Number of Test Features as Read: " << test.Number_of_Features << endl;

		test.features = new FEATURE [ test.Number_of_Features ];

		sprintf (filename,".\\TestRepo\\00.Test_Images\\Frames\\%d.bmp",frameIndex);
		//cout << filename << endl;
		JustGrey = readGrey(filename);
		testImage = Gtype (JustGrey);
		releaseImage (JustGrey);

		for (int featureIndex = 0; featureIndex < test.Number_of_Features; featureIndex ++) {
			fread ( &(test.features[featureIndex].x), sizeof (double), 1 , featureFile);
			fread ( &(test.features[featureIndex].y), sizeof (double), 1 , featureFile);
			/*double sc;
			fread ( &sc, sizeof (double), 1 , featureFile);
			cout << sc << endl;
			fseek (featureFile, sizeof (double), SEEK_CUR); // Skip Reading key orientation*/
			fseek (featureFile, 2 * sizeof (double), SEEK_CUR); // Skip Reading scale and key orientation

			int n1 =  (int)test.features[featureIndex].y;//Why Reflection?testImage->height-1 -
			int n2 =  (int)test.features[featureIndex].x;//Why Reflection?testImage->width-1 -
			if (testImage->imageData[n1 * testImage->width + n2] > 0.48)
				testImage->imageData[n1 * testImage->width + n2] = -0.48;
			else
				testImage->imageData[n1 * testImage->width + n2] = 0.49;

			test.features[featureIndex].FeatureVector = new int[test.FeatureVectorLength];
			fread ( test.features[featureIndex].FeatureVector, sizeof (int), test.FeatureVectorLength , featureFile);
		}
		fclose (featureFile);

		sprintf (filename,".\\TestRepo\\02.Test\\KeysinFrame\\keyImage(%d).bmp",frameIndex);
		//cout << filename << endl;
		writeImage (filename, testImage);
		releaseImage (testImage);

		coordinateMappings = findNearestNeighbor (train, test, logFile, threshold);
		initial = coordinateMappings[0];
		final = coordinateMappings[1];
		
		//if (logFile) {
			fprintf (logFile, "\tNumber of Features: %d\n", (int) test.Number_of_Features);
			fprintf (logFile, "\tWithin Neighborhood Features: %d\n", (int)final.Number_of_Coordinates);
			/*logFile << "Matches:" << endl;
			for (int matchIndex = 0; matchIndex < final.Number_of_Coordinates; matchIndex++) {
				logFile << "(" << initial.x[matchIndex] << "," << initial.y[matchIndex] << ")";
				logFile << " => ";
				logFile << "(" << final.x[matchIndex] << "," << final.y[matchIndex] << ")";
				logFile << " : ";
				logFile << final.scores[matchIndex];
				logFile << endl;
			}*/
		//}

		
		fprintf (logFile, "\n\tAffineFit:\n");
		MATRIX affine = fitAffineMatrix ( initial, final );
		affine.print(logFile);
		fprintf (logFile, "\n");

		fprintf (logFile,"\tFrame: %2d  Actual: %6.3lf  ", frameIndex,2*3.14*(double)frameIndex/(double)100);
		showMotion (affine,  logFile, RotationDataFile);

		// Garbage Collection: Test Feature		
		for (int featureIndex = 0; featureIndex < test.Number_of_Features; featureIndex ++)
			delete test.features[featureIndex].FeatureVector;
		delete test.features;
/*
		delete coordinateMappings->x;
		delete coordinateMappings->y;
		delete coordinateMappings->scores;
		delete coordinateMappings;
*/
		//delete filename;
		
	}	

	// Garbage Collection: Train Feature
	for (int featureIndex = 0; featureIndex < train.Number_of_Features; featureIndex ++)
		delete train.features[featureIndex].FeatureVector;
	delete train.features;

}