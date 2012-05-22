#include <MotionExtract/testMotionExtract.h>
#include <MotionExtract/MotionExtract.h>
#include <math.h>

void testMotionExtract (void) {
	MATRIX AffineMatrix (3,2);
	MATRIX_CONTENT_TYPE** AffineData = AffineMatrix.getDataPtr ();

	double theta = 3.14 / 6;
	cout << "theta: " << theta << endl;

	// row 1
	AffineData[0][0] = cos (theta);
	AffineData[1][0] = sin (theta);
	AffineData[2][0] = 1;

	// row 2
	AffineData[0][1] = - sin (theta);
	AffineData[1][1] = cos (theta);
	AffineData[2][1] = 2;

	showMotion (AffineMatrix);
}