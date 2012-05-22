#ifndef MATCHFEATURES_H
#define MATCHFEATURES_H

#include <Features/featureStruct.h>
#include <Features/coordinates.h>

COORDS* findNearestNeighbor (FEATURES TrainFeature,FEATURES TestFeature, double Threshold = -1);

#endif