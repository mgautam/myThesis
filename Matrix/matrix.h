#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
using namespace std;

// Defining type of storage of each element in the matrix
typedef double MATRIX_CONTENT_TYPE;

// Defining the Abstract Matrix Datatype
class MATRIX {
	private:
		int width;
		int height;	
		MATRIX_CONTENT_TYPE **data;

	public:
		MATRIX (void);
		MATRIX (int width, int height);
		MATRIX (MATRIX_CONTENT_TYPE *vector, int height);
		
		int getWidth (void);
		int getHeight (void);
		MATRIX_CONTENT_TYPE** getDataPtr (void);
		MATRIX_CONTENT_TYPE getValue (int col, int row);
		friend ostream& operator<< (ostream& outStream, const MATRIX OtherMatrix);
		
		void makeZeroMatrix (void);
		void setValue (int col, int row, double value);
		void operator= (MATRIX OtherMatrix);

		// Arithmetic Operation Methods	
		MATRIX operator+ (MATRIX OtherMatrix);
		MATRIX operator- (MATRIX OtherMatrix);
		MATRIX operator* (MATRIX OtherMatrix);

		// Linear Algebra methods
		friend MATRIX transpose (const MATRIX matrix);

		~MATRIX (void);
};

#endif
