#ifndef FEATURESTRUCT_H
#define FEATURESTRUCT_H

typedef struct {
	double x,y;
	int *FeatureVector;
} FEATURE;

typedef struct {
	int FeatureVectorLength;
	int Number_of_Features;
	FEATURE *features;
} FEATURES;

#endif