#include <ModelAffine/fitAffineMatrix.h>

MATRIX fitAffineMatrix (COORDS initialPosition, COORDS finalPosition ) {
	AMATRIX initialAMatrix = AMATRIX (3,initialPosition.Number_of_Coordinates);

	MATRIX initialMatrix = initialAMatrix;
	MATRIX_CONTENT_TYPE **initialData = initialMatrix.getDataPtr ();
	for (int row = 0; row < initialPosition.Number_of_Coordinates; row++) {
		initialData[0][row] = initialPosition.x[row];
		initialData[1][row] = initialPosition.y[row];
		initialData[2][row] = 1;
	}

	MATRIX TransposedInitMat = transpose (initialMatrix);
	
	MATRIX finalX = MATRIX (finalPosition.x, finalPosition.Number_of_Coordinates);
	MATRIX finalY = MATRIX (finalPosition.y, finalPosition.Number_of_Coordinates);
	
	MATRIX ProjectionX = TransposedInitMat * finalX;
	MATRIX ProjectionY = TransposedInitMat * finalY;

	AMATRIX ProjectionMatrix = AMATRIX (3,3);
	MATRIX tmpMatrix = TransposedInitMat * initialMatrix;
	MATRIX_CONTENT_TYPE **ProjectMatrixData = ProjectionMatrix.getDataPtr ();
	MATRIX_CONTENT_TYPE **tmpMatrixData = tmpMatrix.getDataPtr ();
	for (int row = 0; row < 3; row++)
		for (int col = 0; col < 3; col++) {
			ProjectMatrixData[col][row] = tmpMatrixData[col][row];
		}


	
	double *affineX = ProjectionMatrix.invert (ProjectionX.getDataPtr()[0]);
	double *affineY = ProjectionMatrix.invert (ProjectionY.getDataPtr()[0]);

	MATRIX AffineMatrix = MATRIX (3,2);
	MATRIX_CONTENT_TYPE **AffineMatrixData = AffineMatrix.getDataPtr ();

	for (int col = 0; col < 3; col++) {
		AffineMatrixData[col][0] = affineX[col];
		AffineMatrixData[col][1] = affineY[col];
	}

	return AffineMatrix;
}