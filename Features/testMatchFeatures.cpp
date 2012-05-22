#include <Features/testMatchFeatures.h>
#include <Features/matchFeatures.h>

#include <iostream>
using namespace std;

void testMatchFeatures (void) {
	
	int FeatureVectorLength = 10;
	int NumberofTrainFeatures = 10;
	int NumberofTestFeatures = 10;


	FEATURES TrainFeature;
	FEATURES TestFeature;

	TrainFeature.FeatureVectorLength = FeatureVectorLength;
	TestFeature.FeatureVectorLength = FeatureVectorLength;

	TrainFeature.Number_of_Features = NumberofTrainFeatures;
	TestFeature.Number_of_Features = NumberofTestFeatures;

	TrainFeature.features = new FEATURE [NumberofTrainFeatures];
	TestFeature.features = new FEATURE [NumberofTestFeatures];


	for (int FeatureIndex = 0; FeatureIndex < NumberofTrainFeatures; FeatureIndex++) {	

		TrainFeature.features[FeatureIndex].x = FeatureIndex;
		TrainFeature.features[FeatureIndex].y = FeatureIndex;
		TrainFeature.features[FeatureIndex].FeatureVector = new char [FeatureVectorLength];

		for (int vectorIndex = 0; vectorIndex < FeatureVectorLength; vectorIndex++)
			TrainFeature.features[FeatureIndex].FeatureVector[vectorIndex] = (FeatureIndex+1)*vectorIndex;
	
	}

	for (int FeatureIndex = 0; FeatureIndex < NumberofTestFeatures; FeatureIndex++) {

		TestFeature.features[FeatureIndex].x = FeatureIndex;
		TestFeature.features[FeatureIndex].y = FeatureIndex;
		TestFeature.features[FeatureIndex].FeatureVector = new char [FeatureVectorLength];
					
		for (int vectorIndex = 0; vectorIndex < FeatureVectorLength; vectorIndex++)
			TestFeature.features[FeatureIndex].FeatureVector[vectorIndex] = FeatureIndex*vectorIndex;

	}


	COORDS* coordinates = findNearestNeighbor (TrainFeature,TestFeature, 16);

	for (int FeatureIndex = 0; FeatureIndex < coordinates->Number_of_Coordinates; FeatureIndex++) {
		cout << "Train:(" << coordinates[0].x[FeatureIndex] << "," << coordinates[0].y[FeatureIndex] << ")";
		cout << " => " << "Test:(" << coordinates[1].x[FeatureIndex] << "," << coordinates[1].y[FeatureIndex] << ")" << endl;
	}
}