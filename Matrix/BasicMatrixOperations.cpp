#include <stdlib.h>

#include <Matrix/amatrix.h>


// Constructors
AMATRIX::AMATRIX (int width, int height) : MATRIX (width,height) {
	R_ptr = NULL;
	Q_ptr = NULL;
	this->decomposed = false;
}

AMATRIX::AMATRIX (MATRIX matrix) {
	
	MATRIX::operator=(matrix);
	
	R_ptr = NULL;
	Q_ptr = NULL;
	this->decomposed = false;

}



// Matrix Initializations
void AMATRIX::makeZeroMatrix (void) {
	 MATRIX::makeZeroMatrix ();
	this->decomposed = false;
}

void AMATRIX::operator= (AMATRIX matrix) {
	
	MATRIX::operator=(matrix);
	int width = matrix.getWidth ();
	int height = matrix.getHeight ();

	if (matrix.decomposed) {
		if (this->Q_ptr != NULL) delete this->Q_ptr;
		if (this->R_ptr != NULL) delete this->R_ptr;
	
		this->Q_ptr = new MATRIX ( width, height);
		this->R_ptr = new MATRIX ( width, width);
		
		for ( int col = 0; col < width; col++) {
			for ( int row = 0; row < height; row++)
				(this->Q_ptr)->setValue (col, row, (matrix.Q_ptr)->getValue(col,row));				
		
			for ( int row = 0; row < width; row++)
				(this->R_ptr)->setValue (col, row, (matrix.R_ptr)->getValue(col,row));				
		}		
	}	
	this->decomposed = matrix.decomposed;
}

void AMATRIX::operator= (MATRIX matrix) {
	
	MATRIX::operator=(matrix);

	R_ptr = NULL;
	Q_ptr = NULL;
	this->decomposed = false;
}

void AMATRIX::setValue (int col, int row, MATRIX_CONTENT_TYPE value) {
	MATRIX::setValue (col, row, value);	
	this->decomposed = false;
}

// Matrix Exports
MATRIX* AMATRIX::getRptr (void) {
	return R_ptr;
}

//ostream& MATRIX::operator<< (ostream& outStream) {
//	for ( int row = 0; row < (this->matrix_ds_ptr)->height; row++) {
//		for ( int col = 0; col < (this->matrix_ds_ptr)->width; col++)		
//			outStream << this->matrix_ds_ptr->data[col][row] << "   ";
//		outStream << endl;
//	}
//    return outStream;
//}


// Arithmetic Operations
AMATRIX AMATRIX::operator+ (AMATRIX OtherMatrix) {
	int width = OtherMatrix.getWidth ();
	int height = OtherMatrix.getHeight ();
	AMATRIX sum (width , height);

	MATRIX_CONTENT_TYPE **thisData = this->getDataPtr ();
	MATRIX_CONTENT_TYPE **OtherMatrixData = OtherMatrix.getDataPtr ();	
	MATRIX_CONTENT_TYPE **sumData = sum.getDataPtr ();

	for ( int col = 0; col < width; col++)
		for ( int row = 0; row < height; row++)
			sumData[col][row] = thisData[col][row] + OtherMatrixData[col][row]; 

	return sum;
}

AMATRIX AMATRIX::operator- (AMATRIX OtherMatrix) {
	
	int width = OtherMatrix.getWidth ();
	int height = OtherMatrix.getHeight ();
	AMATRIX diff (width , height);

	MATRIX_CONTENT_TYPE **thisData = this->getDataPtr ();
	MATRIX_CONTENT_TYPE **OtherMatrixData = OtherMatrix.getDataPtr ();	
	MATRIX_CONTENT_TYPE **diffData = diff.getDataPtr ();
	

	for ( int col = 0; col < width; col++)
		for ( int row = 0; row < height; row++)
			diffData[col][row] = thisData[col][row] - OtherMatrixData[col][row]; 

	return diff;
}

AMATRIX AMATRIX::operator* (AMATRIX OtherMatrix) {
	int ownWidth = this->getWidth ();	
	if ( ownWidth != OtherMatrix.getHeight ()) {
		cout << "Error: Matrix Dimensions Don't Match" << endl;
		exit(-1) ;
	}
	int height = this->getHeight ();
	int width = OtherMatrix.getWidth ();
	AMATRIX product = AMATRIX(width,height);
	product.makeZeroMatrix ();

	MATRIX_CONTENT_TYPE **thisData = this->getDataPtr ();
	MATRIX_CONTENT_TYPE **OtherMatrixData = OtherMatrix.getDataPtr ();
	MATRIX_CONTENT_TYPE **productData = product.getDataPtr ();

	for (int row = 0; row < height; row++)
		for (int col = 0; col < width; col++)	
			for (int mIndex = 0; mIndex < ownWidth; mIndex++)
				productData[col][row] += thisData[mIndex][row] * OtherMatrixData[col][mIndex];

	return product;
}
