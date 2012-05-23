#ifndef NEIGHBORSPY_H
#define NEIGHBORSPY_H 1

#define FEATURE_LENGTH 10

struct angleKey {
	double x, y;
	double scale;
	double KeyOrientation;
	int sectorCount[FEATURE_LENGTH];
};

int magoriCalc (GIMAGE *GaussPix, IMAGE *ExPix, int octave, int blur, char *Project_Folder);
void angleKeyCalc (IMAGE *ExPix, int numKeys, int octave, int blur, char* Project_Folder);
void writeAllSift (GTYPE sigma, int numOctaves,int numBlurs,char *PROJECT_FOLDER, int frameIndex);

#endif
