#ifndef GRADIENTDESCENT_H
#define GRADIENTDESCENT_H

#include <ModelAffine/coordinates.h>
#include <Matrix/matrix.h>

double* gradient_descent ( int num_datapoints, MATRIX *datapoint_matrix, double *function_value, int num_parameters, double* weights = 0, double* initial_parameters = 0 );
MATRIX fitAffineMatrixGD ( COORDS initialPosition, COORDS finalPosition );

#endif