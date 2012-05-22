#ifndef BMPWRITE_H
#define BMPWRITE_H

#include<iostream>
#include <bmpAccess/bmpStructs.h>

using namespace std;

void writeFileHeader(FILE *fhandle, FileHeader fileHeader, bool printInfo);
void writeDataHeader(FILE *fhandle, BMPHeader bmpHeader, bool printInfo);
void writeGreyPalette(FILE *fhandle);
void writeRaster(FILE *fhandle, IMAGE *image, bool printInfo);

#endif
