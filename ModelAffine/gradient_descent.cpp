#include <ModelAffine/gradient_descent.h>
#include <string.h>
#define SMALL_NOISE 0.01

double* gradient_descent ( int num_datapoints, MATRIX *datapoint_matrix, double *function_value, int num_parameters, double *weights, double* initial_parameters )
{
	double learning_rate = 0.01;// / (double) num_datapoints;

	bool weights_created = false;

	if (!weights)
	{
		weights = new double [num_datapoints];
		
		for (int pointIndex = 0; pointIndex < num_datapoints; pointIndex++)
		{
			weights[pointIndex] = 1.0;
		}

		weights_created = true;
	}

	double* updated_parameters = new double [ num_parameters ];

	if (initial_parameters)
		memcpy ( updated_parameters, initial_parameters, sizeof (double) * num_parameters );
	else
		memset ( updated_parameters, 0, sizeof (double) * num_parameters );

	//cout << endl;
	//for (int paramIndex = 0; paramIndex < num_parameters; paramIndex++)
	//	printf ( "%9.4lf\t", updated_parameters[paramIndex] );
	//cout << endl;

	double **datapoints = datapoint_matrix->getDataPtr ();

	double estimated_value, error;
	for (int pointIndex = 0; pointIndex < num_datapoints; pointIndex++)
	{
		learning_rate = 0.01 / (pointIndex + 1) ;
		//printf ( "%6.4lf ", learning_rate);

		estimated_value = 0;
		for (int paramIndex = 0; paramIndex < num_parameters; paramIndex++)
		{
			estimated_value += updated_parameters[paramIndex] * datapoints[paramIndex][pointIndex];
			//printf ( "%9.4lf ",datapoints[paramIndex][pointIndex]);
		}
		
		error = function_value[pointIndex] - estimated_value;
		//printf ( "%9.4lf -> ", error);

		for (int paramIndex = 0; paramIndex < num_parameters; paramIndex++)
		{
			updated_parameters[paramIndex] += learning_rate * 2.0 * error * datapoints[paramIndex][pointIndex] * weights[pointIndex] / (double) num_datapoints;
			//printf ( "%9.4lf ", updated_parameters[paramIndex] );
		}
		//cout << endl;
	}

	if (weights_created)
		delete weights;

	return updated_parameters;
}

MATRIX fitAffineMatrixGD (COORDS initialPosition, COORDS finalPosition ) 
{
	MATRIX *initialMatrix = new MATRIX (3,initialPosition.Number_of_Coordinates);
	
	MATRIX_CONTENT_TYPE **initialData = initialMatrix->getDataPtr ();
	for (int row = 0; row < initialPosition.Number_of_Coordinates; row++)
	{
		initialData[0][row] = initialPosition.x[row];
		initialData[1][row] = initialPosition.y[row];
		initialData[2][row] = 1;		
	}

	double* x_parameters = gradient_descent ( initialPosition.Number_of_Coordinates, initialMatrix, finalPosition.x, 3);
	double* y_parameters = gradient_descent ( initialPosition.Number_of_Coordinates, initialMatrix, finalPosition.y, 3);
	

	MATRIX AffineMatrix = MATRIX (3,2);
	MATRIX_CONTENT_TYPE **AffineMatrixData = AffineMatrix.getDataPtr ();
	for (int col = 0; col < 3; col++)
	{
		AffineMatrixData[col][0] = x_parameters[col];
		AffineMatrixData[col][1] = y_parameters[col];
	}

	return AffineMatrix;
}