#ifndef BMPREAD_H
#define BMPREAD_H

#include <iostream>
#include <bmpAccess/bmpStructs.h>

void readFileHeader(FILE *fhandle, FileHeader *fileHeader, bool printInfo);
void readDataHeader(FILE *fhandle, BMPHeader *bmpHeader, bool printInfo);

IMAGE* readRaster(FILE *fhandle, FileHeader *fileHeader, BMPHeader *bmpHeader, bool printInfo);
IMAGE* readLuminance(FILE *fhandle, FileHeader *fileHeader, BMPHeader *bmpHeader, bool printInfo);
#endif
