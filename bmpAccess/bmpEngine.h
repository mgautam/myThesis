#ifndef BMPENGINE_H
#define BMPENGINE_H

#include <bmpAccess/bmpStructs.h>
#include <stdio.h>

// File Operations
 IMAGE* readRGB(char *fileName, FILE *logFile = false);
 IMAGE* readGrey(char *fileName, FILE *logFile = false);
 void writeImage(char *fileName, IMAGE *image, FILE *logFile = false);
 void writeImage(char *fileName, GIMAGE *image, FILE *logFile = false);
 void writeImageNorm(char *fileName, GIMAGE *image, FILE *logFile = false);

// Memory Allocations
 IMAGE* createimage(int width, int height, int numColors);
 GIMAGE* createImage(int width, int height, int numColors);
 IMAGE* cloneImage(IMAGE* inImage);
 GIMAGE* cloneImage(GIMAGE* inImage);

// Format Conversions
 IMAGE* Greylize(IMAGE *inImage);
 GIMAGE* Gtype(IMAGE *inImage);
 IMAGE* uChar(GIMAGE *inImage);
 IMAGE* uCharNorm(GIMAGE *inImage);

// Memory Deallocators
 void releaseImage(IMAGE *image);
 void releaseImage(GIMAGE *image);

#endif