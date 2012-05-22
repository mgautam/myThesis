#ifndef AMATRIX_H
#define AMATRIX_H

#include <Matrix/matrix.h>

#include <iostream>
using namespace std;

// Advanced Matrix Class
class AMATRIX:public MATRIX {
	private:
		MATRIX *Q_ptr, *R_ptr;
		bool decomposed;

	public:
	// Model
		// Constructors
		AMATRIX (int width, int height);
		AMATRIX (MATRIX matrix);

		// Matrix Initializations
		void makeZeroMatrix (void);
		void operator= (AMATRIX OtherMatrix);
		void operator= (MATRIX OtherMatrix);
		void setValue (int col, int row, MATRIX_CONTENT_TYPE value);
		

	// View
		//ostream& operator<< (ostream& outStream);
		MATRIX *getRptr (void);

	// Control
		// Arithmetic Operation Methods	
		AMATRIX operator+ (AMATRIX OtherMatrix);
		AMATRIX operator- (AMATRIX OtherMatrix);
		AMATRIX operator* (AMATRIX OtherMatrix);

		// Linear Algebra methods
		friend AMATRIX transpose (const AMATRIX matrix);
		void decomposeQR (void);
		double* invert (double *vector);

		// Destructor
	//	~AMATRIX (void); 
};

typedef class AMATRIX *AMATRIX_PTR;

#endif