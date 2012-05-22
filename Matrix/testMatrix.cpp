#include <iostream>
#include <stdlib.h>

#include <Matrix/amatrix.h>
#include <Matrix/testMatrix.h>

using namespace std;

void testMatrix (void) {
	cout << "Matrix Testing begins... " << endl;
	int testHeight = 3;
	int testWidth = 3;
	
	AMATRIX mat3 (testWidth,testHeight);	
	for ( int col = 0; col < testWidth; col++)
		for ( int row = 0; row < testHeight; row++)
			mat3.setValue (col, row, (MATRIX_CONTENT_TYPE) (int)((col+1)*rand()/10000));
	cout << mat3 << endl;
	mat3.decomposeQR ();
	
	double vector[3] = {1.0,2.0,3.0};
	double *inv = mat3.invert(vector);

	for (int i=0;i<3;i++)
		cout << inv[i] << endl;

}
