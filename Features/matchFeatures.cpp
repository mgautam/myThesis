#include <Features/matchFeatures.h>
#include <math.h>

COORDS* findNearestNeighbor (FEATURES TrainFeature,FEATURES TestFeature, double Threshold) {
	int FeatureVectorLength = TrainFeature.FeatureVectorLength; // == TestFeature.FeatureVectorLength
	int numTrainFeatures = TrainFeature.Number_of_Features;
	int numTestFeatures = TestFeature.Number_of_Features;

	COORDS *coordinates = new COORDS[2];
	coordinates[0].Number_of_Coordinates = numTrainFeatures; // Number of Corresponding Training object 2D Position vectors
	coordinates[0].x = new double [numTrainFeatures];
	coordinates[0].y = new double [numTrainFeatures];
	coordinates[0].scores = new double [numTrainFeatures];
	coordinates[1].Number_of_Coordinates = numTestFeatures; // Number of Testing object 2D Position vectors
	coordinates[1].x = new double [numTestFeatures];
	coordinates[1].y = new double [numTestFeatures];
	coordinates[1].scores = new double [numTestFeatures];


	double Distance,minDistance;
	int nearNeighborIndex = 0;
	for ( int TestIndex = 0; TestIndex < numTestFeatures; TestIndex++) {

		coordinates[1].x[nearNeighborIndex] = TestFeature.features[TestIndex].x;
		coordinates[1].y[nearNeighborIndex] = TestFeature.features[TestIndex].y;
		minDistance = 99999999999;
		
		for ( int TrainIndex = 0; TrainIndex < numTrainFeatures; TrainIndex++) {
			
			Distance = 0;
			for (int vectorIndex = 0; vectorIndex < FeatureVectorLength; vectorIndex++)
				Distance += pow ((double)(TrainFeature.features[TrainIndex].FeatureVector[vectorIndex] - TestFeature.features[TestIndex].FeatureVector[vectorIndex]) , 2);
			Distance = sqrt (Distance);
			
			if (Distance < minDistance) {
				coordinates[0].x[nearNeighborIndex] = TrainFeature.features[TrainIndex].x;
				coordinates[0].y[nearNeighborIndex] = TrainFeature.features[TrainIndex].y;
				minDistance = Distance;
			}
		}
		coordinates[0].scores[nearNeighborIndex] = minDistance;
		coordinates[1].scores[nearNeighborIndex] = minDistance;
		if ( Threshold > 0) {
			// User has defined a Threshold
			if (minDistance < Threshold) {
				// minDistance is within the Thresholded limit
				nearNeighborIndex++;
				// This is equivalent to only adding the nodes which are within the Threshold distance
			}				
		}
		else {
			// User has not given any specific distance threshold
			nearNeighborIndex++;
			// This is equivalent to adding the nodes no matter how apart the training and test vectors are
		}				


	}

	coordinates[0].Number_of_Coordinates = nearNeighborIndex; // Only the Number of Corresponding nearest Neighbor Training object 2D Position vectors
	coordinates[1].Number_of_Coordinates = nearNeighborIndex; // Only the Number of Corresponding nearest Neighbor Test object 2D Position vectors


	return coordinates;
}