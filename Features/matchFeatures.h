#ifndef MATCHFEATURES_H
#define MATCHFEATURES_H

#include <Features/featureStruct.h>
#include <Features/coordinates.h>

#include <stdio.h>

COORDS* findNearestNeighbor (FEATURES TrainFeature,FEATURES TestFeature, FILE *logFile = stdout, double Threshold = -1);

#endif