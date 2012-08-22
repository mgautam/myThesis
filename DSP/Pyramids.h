#ifndef PYRAMIDS_H
#define PYRAMIDS_H

#include <stdio.h>
#include <bmpAccess/bmpStructs.h>

#define NULL 0

GIMAGE** GaussianPyramid(GIMAGE *inImage, GTYPE stepSize, int numLayers, char *PROJECT_FOLDER = NULL, FILE *logFile = stdout);
GIMAGE**** LaplacianPyramid(GIMAGE *inImage, GTYPE sigma, int octaves, int numBlurs, char *PROJECT_FOLDER = NULL, FILE *logFile = stdout);
void releaseLaplacianPyramid (GIMAGE**** Pyramid, int numOctaves, int numBlurs);
#endif