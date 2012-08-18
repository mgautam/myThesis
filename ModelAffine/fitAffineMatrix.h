#ifndef FITAFFINEMATRIX_H
#define FITAFFINEMATRIX_H

#include <ModelAffine/coordinates.h>
#include <Matrix/amatrix.h>

MATRIX fitAffineMatrix ( COORDS initialPosition, COORDS finalPosition, double init_object_x, double init_object_y );

#endif