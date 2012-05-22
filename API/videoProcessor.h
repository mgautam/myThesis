#ifndef VIDEOPROCESSOR_H
#define VIDEOPROCESSOR_H

#include <stdio.h>

void processFrames (double threshold = -1,  FILE *RotationDataFile = 0, bool printInfo = false);

#endif