#include <stdlib.h>
#include <math.h>

#include <Matrix/amatrix.h>
static double dotproduct (double *vector1, double *vector2, int size);
static double L2Norm (double *vector, int size);
static void invMult (MATRIX *upperTriangular, double* vector);

double dotproduct (double *vector1, double *vector2, int size) {
	double result = 0;
	for (int index = 0; index < size; index++)
		result += vector1[index]*vector2[index];
	return result;
}

double L2Norm (double *vector, int size) {
	double l2norm2 = 0;
	for (int index = 0; index < size; index++)
		l2norm2 += pow (vector[index],2);
	return sqrt(l2norm2);
}

// Linear Algebra Methods
AMATRIX transpose (const AMATRIX matrix) {
	int height = ((MATRIX)matrix).getHeight ();
	int width = ((MATRIX)matrix).getWidth ();
	// width and height positions are exchanged
	AMATRIX transposedMatrix = AMATRIX (height,width);

	MATRIX_CONTENT_TYPE** transposedMatrixData = transposedMatrix.getDataPtr ();
	MATRIX_CONTENT_TYPE** matrixData = ((MATRIX)matrix).getDataPtr ();

	for ( int col = 0; col < width; col++)
		for ( int row = 0; row < height; row++)
			transposedMatrixData[row][col] = matrixData[col][row]; 

	return transposedMatrix;
}

void AMATRIX::decomposeQR (void) {
	if (this->Q_ptr != NULL) delete this->Q_ptr;
	if (this->R_ptr != NULL) delete this->R_ptr;
	
	int width = this->getWidth ();
	int height = this->getHeight ();

	this->Q_ptr = new MATRIX (width,height);
	this->R_ptr = new MATRIX (width,width);
	
	this->R_ptr->makeZeroMatrix ();
	double **thisDataPtr = this->getDataPtr ();
	double **QDataPtr = Q_ptr->getDataPtr ();
	double **RDataPtr = R_ptr->getDataPtr ();

	// QR Engine
	for (int col = 0; col < width; col++) {
		for (int row = 0; row < height; row++)
			QDataPtr[col][row] = thisDataPtr[col][row];
		
		for (int axis = 0; axis < col; axis++) {
			RDataPtr[col][axis] = dotproduct (QDataPtr[col],QDataPtr[axis],height);
			for (int row = 0; row < height; row++)
				QDataPtr[col][row] -= RDataPtr[col][axis]*QDataPtr[axis][row];
		}

		RDataPtr[col][col] = L2Norm (QDataPtr[col],height);
		for (int row = 0; row < height; row++)
			QDataPtr[col][row] /= RDataPtr[col][col];

	}

	//cout << "Q Matrix is" << endl << *Q_ptr << endl;
	//cout << "R Matrix is" << endl << *R_ptr << endl;

	decomposed = true;
}

void invMult (MATRIX *upperTriangular, double* vector) {
	int matWidth = upperTriangular->getWidth ();
	int matHeight = upperTriangular->getHeight ();
	MATRIX_CONTENT_TYPE **content = upperTriangular->getDataPtr ();

	//cout << "Inverse..." << endl;
	if (matWidth == matHeight) {
		for (int col = matWidth-1; col >= 0; col--) {
			vector[col] /= content[col][col];
			for (int row = col-1; row >= 0; row--)
				vector[row] -= content[col][row]*vector[col];
		}
	}
	else {
		cout << "Not a Square - Triangular Matrix!" << endl;
		exit (-1);
	}
	//cout << "ends" << endl;	
}

double* AMATRIX::invert (double *vector) {
	this->decomposeQR();
	MATRIX tempVector = *(new MATRIX(1,this->getHeight()));
	double *tempVectorData = *(tempVector.getDataPtr());
	for (int row = 0; row < this->getHeight(); row++)
		tempVectorData[row] = vector[row];

	MATRIX Q = *(this->Q_ptr);
	tempVector = transpose(Q) * tempVector;
	invMult (this->R_ptr,tempVectorData);
	return tempVectorData;
}
