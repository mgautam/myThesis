#include <stdlib.h>
#include <Matrix/matrix.h>

//Constructors
MATRIX::MATRIX (void) {
	
}

MATRIX::MATRIX (int width, int height) {
	this->width = width;
	this->height = height;

	this->data = new MATRIX_CONTENT_TYPE*[width];	
	for (int col = 0; col < width; col++)
		this->data[col] = new MATRIX_CONTENT_TYPE[height];
}

MATRIX::MATRIX (MATRIX_CONTENT_TYPE *vector, int height) {
	this->width = 1;
	this->height = height;

	this->data = new MATRIX_CONTENT_TYPE*[this->width];// can this be just new MATRIX_CONTENT_TYPE* ? What happens to next line?
	this->data[0] = vector;
}



// View
int MATRIX::getWidth (void) {
	return this->width;
}

int MATRIX::getHeight (void) {
	return this->height;
}

MATRIX_CONTENT_TYPE MATRIX::getValue (int col, int row) {
	return this->data[col][row];
}

MATRIX_CONTENT_TYPE** MATRIX::getDataPtr (void) {
	return this->data;
}

ostream& operator<< (ostream& outStream, const MATRIX matrix) {
	
	for ( int row = 0; row < matrix.height; row++) {
		for ( int col = 0; col < matrix.width; col++)		
			outStream << matrix.data[col][row] << "   ";
		outStream << endl;
	}
    return outStream;
}

void MATRIX::print (FILE *logFile)
{
	for ( int row = 0; row < this->height; row++) {
		fprintf (logFile,"\t");
		for ( int col = 0; col < this->width; col++)		
			fprintf (logFile, "%lf   ", this->data[col][row]) ;
		fprintf (logFile,"\n");
	}    
}


// Controllers
void MATRIX::makeZeroMatrix (void) {
	for ( int col = 0; col < this->width; col++)
		for ( int row = 0; row < this->height; row++)
			this->data[col][row] = 0;
}

void MATRIX::setValue (int col, int row, MATRIX_CONTENT_TYPE value) {
	this->data[col][row] = value;
}

void MATRIX::operator= (MATRIX OtherMatrix) {
	this->width = OtherMatrix.width;
	this->height = OtherMatrix.height;

	if (this->data != NULL) {
		for ( int col = 0; col < OtherMatrix.width; col++)
			delete this->data[col];
		delete this->data;
	}

	this->data = new MATRIX_CONTENT_TYPE*[this->width];
	for ( int col = 0; col < OtherMatrix.width; col++)
		this->data[col] = new MATRIX_CONTENT_TYPE[this->height];;

	for ( int col = 0; col < OtherMatrix.width; col++)
		for ( int row = 0; row < OtherMatrix.height; row++)
			this->data[col][row] = OtherMatrix.data[col][row];
}



// Arithmetic Operations
MATRIX MATRIX::operator+ (MATRIX OtherMatrix) {
	MATRIX sum (OtherMatrix.width , OtherMatrix.height);

	for ( int col = 0; col < OtherMatrix.width; col++)
		for ( int row = 0; row < OtherMatrix.height; row++)
			sum.data[col][row] = this->data[col][row] + OtherMatrix.data[col][row]; 

	return sum;
}

MATRIX MATRIX::operator- (MATRIX OtherMatrix) {
	MATRIX diff (OtherMatrix.width , OtherMatrix.height);

	for ( int col = 0; col < OtherMatrix.width; col++)
		for ( int row = 0; row < OtherMatrix.height; row++)
			diff.data[col][row] = this->data[col][row] - OtherMatrix.data[col][row]; 

	return diff;
}

MATRIX MATRIX::operator* (MATRIX OtherMatrix) {
	if ( this->width != OtherMatrix.height) {
		fprintf (stderr, "Error: Matrix Dimensions Don't Match\n");
		exit(-1) ;
	}

	MATRIX product = MATRIX(OtherMatrix.width , this->height);
	product.makeZeroMatrix ();

	for (int row = 0; row < this->height; row++)
		for (int col = 0; col < OtherMatrix.width; col++) {			
			for (int mIndex = 0; mIndex < this->width; mIndex++)
				product.data[col][row] += this->data[mIndex][row] * OtherMatrix.data[col][mIndex]; 
		}

	return product;
}



// Linear Algebra Methods
MATRIX transpose (const MATRIX matrix) {
	int height = ((MATRIX)matrix).getHeight ();
	int width = ((MATRIX)matrix).getWidth ();
	// width and height positions are exchanged
	MATRIX transposedMatrix = MATRIX (height,width);

	MATRIX_CONTENT_TYPE** transposedMatrixData = transposedMatrix.getDataPtr ();
	MATRIX_CONTENT_TYPE** matrixData = ((MATRIX)matrix).getDataPtr ();

	for ( int col = 0; col < width; col++)
		for ( int row = 0; row < height; row++)
			transposedMatrixData[row][col] = matrixData[col][row]; 

	return transposedMatrix;
}



// Destructor
MATRIX::~MATRIX (void) {
	/*fprintf (logFile, "%d\n",this->width);
	if (this->data != NULL) {
		for (int col = 0; col < this->width; col++)
			delete this->data[col];
		delete this->data;
	}
	this->height = 0;
	this->width = 0;*/
}
