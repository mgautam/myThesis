#ifndef BMPWRITE_H
#define BMPWRITE_H

#include<iostream>
#include <bmpAccess/bmpStructs.h>

using namespace std;

void writeFileHeader(FILE *fhandle, FileHeader fileHeader, FILE *logFile);
void writeDataHeader(FILE *fhandle, BMPHeader bmpHeader, FILE *logFile);
void writeGreyPalette(FILE *fhandle);
void writeRaster(FILE *fhandle, IMAGE *image, FILE *logFile);

#endif
