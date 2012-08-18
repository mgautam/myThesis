#ifndef MOTIONEXTRACT_H
#define MOTIONEXTRACT_H

#include <Matrix/matrix.h>

MATRIX* showMotion (MATRIX AffineMatrix, FILE *output = 0, FILE *logFile = stdout);

#endif