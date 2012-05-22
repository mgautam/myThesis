#ifndef BMPCREATE_H
#define BMPCREATE_H

#include <bmpAccess/bmpStructs.h>

void createDataHeader(int width, int height, int numColors, BMPHeader *bmpHeader);
void createFileHeader(BMPHeader *bmpHeader, FileHeader *fileHeader);

#endif