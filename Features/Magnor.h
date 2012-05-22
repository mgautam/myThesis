#ifndef MAGNOR_H
#define MAGNOR_H

#include <bmpAccess/bmpStructs.h>

#define FEATURE_LENGTH 128

struct siftKey {
	double x, y;
	double scale;
	double KeyOrientation;
	GTYPE Descriptor[FEATURE_LENGTH];
};

int magoriCalc(GIMAGE *GaussPix, IMAGE *ExPix, int octave, int blur, char *Project_Folder);
void siftKeyCalc(IMAGE *ExPix, int numKeys, int octave, int blur, char* Project_Folder);
void writeAllSift(GTYPE sigma, int numOctaves,int numBlurs,char *PROJECT_FOLDER, int frameIndex);

#endif
