#ifndef BMPREAD_H
#define BMPREAD_H

#include <iostream>
#include <bmpAccess/bmpStructs.h>

void readFileHeader(FILE *fhandle, FileHeader *fileHeader, FILE *logFile);
void readDataHeader(FILE *fhandle, BMPHeader *bmpHeader, FILE *logFile);

IMAGE* readRaster(FILE *fhandle, FileHeader *fileHeader, BMPHeader *bmpHeader, FILE *logFile);
IMAGE* readLuminance(FILE *fhandle, FileHeader *fileHeader, BMPHeader *bmpHeader, FILE *logFile);
#endif
