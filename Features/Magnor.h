#ifndef MAGNOR_H
#define MAGNOR_H

#include <bmpAccess/bmpStructs.h>

#define FEATURE_LENGTH 128

struct siftKey {
	double x, y;
	double scale;
	double KeyOrientation;
	double Descriptor[FEATURE_LENGTH];
};

int featureCalc(GIMAGE *GaussPix, IMAGE *ExPix, int octave, int blur, char *Project_Folder);
void writeAllFeatures (GTYPE sigma, int numOctaves, int numBlurs, char *PROJECT_FOLDER, int frameIndex);

#endif
