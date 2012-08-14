#ifndef VIDEOPROCESSOR_H
#define VIDEOPROCESSOR_H

#include <stdio.h>

void processFrames (double threshold = -1,  FILE *RotationDataFile = 0, FILE *logFile = stdout);

#endif