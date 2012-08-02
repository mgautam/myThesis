#include <stdio.h>
#include <math.h>

#include <MotionExtract/MotionExtract.h>

void showMotion (MATRIX AffineMatrix, FILE *output) {
	
	MATRIX_CONTENT_TYPE** AffineMatrixData = AffineMatrix.getDataPtr();
	
	double rawCosX = (AffineMatrixData[0][0]+AffineMatrixData[1][1]);
	double rawSinX = (AffineMatrixData[1][0]-AffineMatrixData[0][1]);	
	double Normalizer = sqrt ( pow (rawCosX,2) + pow (rawSinX,2) );

	double cosX = rawCosX / Normalizer;
	double sinX = rawSinX / Normalizer;

	double theta = acos (cosX);

	// Fix rotation making use of cosX and sinX and not tanX;

	double bx = AffineMatrixData[2][0] - (cosX*512 + sinX*512);
	double by = AffineMatrixData[2][1] - (-sinX*512 + cosX*512);

	//printf ("Rotation: %6.3lf = %6.3lf Translation: ( x: %8.3lf , y: %8.3lf )\n", theta, 6.28-theta, bx, by);//atan (sinX/cosX)//asin (sinX)
	printf ("Rotation: %6.3lf = %6.3lf = %6.3lf = %6.3lf \n", theta, asin (sinX), atan2 (sinX,cosX), 6.28-theta );
	
	if (output) 
		fwrite (&theta, sizeof (double), 1, output);

}
