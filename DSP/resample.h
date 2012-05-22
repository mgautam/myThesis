#ifndef RESAMPLE_H
#define RESAMPLE_H

#include <bmpAccess/bmpStructs.h>

void upScale(GIMAGE *inImage, int scale, GIMAGE *upScaled);
void downScale(GIMAGE *inImage, int scale, GIMAGE *downScaled);
void resample(GIMAGE *inImage, int expandMag, int compressMag, GIMAGE *outImage);

#endif