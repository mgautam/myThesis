#ifndef VIDEOPROCESSOR_H
#define VIDEOPROCESSOR_H

#include <stdio.h>
#include <Matrix/matrix.h>

void processFrames (MATRIX *selection_box, double threshold = -1,  FILE *RotationDataFile = 0, FILE *logFile = stdout);

#endif