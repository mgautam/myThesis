#include <API/videoProcessor.h>
#include <bmpAccess/bmpEngine.h>
#include <Features/featureStruct.h>
#include <Features/FeatureEngine.h>
#include <Features/matchFeatures.h>
#include <ModelAffine/coordinates.h>
#include <ModelAffine/fitAffineMatrix.h>
#include <MotionExtract/MotionExtract.h>
#include <AffineTransformer/blocks.h>

#include <string.h>
#include <iostream>
using namespace std;

extern int TrainObject_x;
extern int TrainObject_y;

void processFrames (MATRIX *selection_box, double threshold, FILE *RotationDataFile, FILE *logFile)
{
	char *filename = new char[100];
	sprintf (filename,"./TestRepo/01.Training/train.bmp");//subject.bmp
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

		int n1 =  (int)train.features[index].y;// + trainImage->height / 2 ;//Why Reflection?trainImage->height-1 -
		int n2 =  (int)train.features[index].x;// + trainImage->width / 2;//Why Reflection?trainImage->width-1 -		
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
	IMAGE *testImage, *testkeyImage;	
	COORDS* coordinateMappings;
	COORDS initial,final;
	for ( int frameIndex = 2; frameIndex < 100; frameIndex+=1 )
	{
		// Extract Sift Features
		//filename = new char[100];
		sprintf (filename, "./TestRepo/02.Test/00.Frames/%d.bmp",frameIndex);
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

		sprintf (filename,"./TestRepo/02.Test/00.Frames/%d.bmp",frameIndex);
		//cout << filename << endl;
		testImage = readGrey(filename);		
		testkeyImage = cloneImage (testImage);

		for (int featureIndex = 0; featureIndex < test.Number_of_Features; featureIndex ++)
		{
			fread ( &(test.features[featureIndex].x), sizeof (double), 1 , featureFile);
			fread ( &(test.features[featureIndex].y), sizeof (double), 1 , featureFile);
			/*double sc;
			fread ( &sc, sizeof (double), 1 , featureFile);
			cout << sc << endl;
			fseek (featureFile, sizeof (double), SEEK_CUR); // Skip Reading key orientation*/
			fseek (featureFile, 2 * sizeof (double), SEEK_CUR); // Skip Reading scale and key orientation

			int n1 =  (int)test.features[featureIndex].y;// + testImage->height / 2 ;//Why Reflection?testImage->height-1 -
			int n2 =  (int)test.features[featureIndex].x;// + testImage->width / 2;//Why Reflection?testImage->width-1 -
			if (testkeyImage->imageData[n1 * testkeyImage->width + n2] > 250)
				testkeyImage->imageData[n1 * testkeyImage->width + n2] = 0;
			else
				testkeyImage->imageData[n1 * testkeyImage->width + n2] = 255;

			test.features[featureIndex].FeatureVector = new int[test.FeatureVectorLength];
			fread ( test.features[featureIndex].FeatureVector, sizeof (int), test.FeatureVectorLength , featureFile);
		}
		fclose (featureFile);

		sprintf (filename,".\\TestRepo\\02.Test\\09.KeysinFrame\\keyImage(%d).bmp",frameIndex);
		//cout << filename << endl;
		writeImage (filename, testkeyImage);
		releaseImage (testkeyImage);

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
		MATRIX affine = fitAffineMatrix ( initial, final, TrainObject_x, TrainObject_y );
		affine.print(logFile);
		fprintf (logFile, "\n");

		fprintf (logFile,"\tFrame: %2d  Actual: %6.3lf   Translation: ( x: %8.3lf , y: %8.3lf )\n", frameIndex,2*3.14*(double)frameIndex/(double)100,0.0,0.0);
		MATRIX *roTrans = showMotion (affine, RotationDataFile, logFile);

		
		IMAGE *boxed = render_selection (selection_box, roTrans, testImage);
		releaseImage (testImage);
		sprintf (filename,"./TestRepo/02.Test/10.Result/%d.bmp",frameIndex);
		writeImage (filename,boxed);
		releaseImage (boxed);

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