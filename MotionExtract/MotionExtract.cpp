#include <stdio.h>
#include <math.h>

#include <MotionExtract/MotionExtract.h>

MATRIX* showMotion (MATRIX AffineMatrix, FILE *output, FILE *logFile) 
{	
	MATRIX_CONTENT_TYPE** AffineMatrixData = AffineMatrix.getDataPtr();
	
	double rawCosX = (AffineMatrixData[0][0]+AffineMatrixData[1][1]);
	double rawSinX = (AffineMatrixData[1][0]-AffineMatrixData[0][1]);	
	double Normalizer = sqrt ( pow (rawCosX,2) + pow (rawSinX,2) );

	double cosX = rawCosX / Normalizer;
	double sinX = rawSinX / Normalizer;

	double theta = atan2 ( sinX, cosX );//acos (cosX) //asin (sinX),

	//fprintf (logFile,"\t\tEstimated: %6.3lf\n\n", theta);
	fprintf (logFile,"\t\tEstimated: %6.3lf   Translation: ( x: %8.3lf , y: %8.3lf )\n\n", theta, AffineMatrixData[2][0], AffineMatrixData[2][1]);
	
	if (output) {
		fwrite (&theta, sizeof (double), 1, output);
		fflush (output);
	}

	MATRIX *roTrans = new MATRIX (3,2);
	MATRIX_CONTENT_TYPE **rotData = roTrans->getDataPtr ();

	rotData[0][0] = cosX;	rotData[1][0] = -sinX;	rotData[2][0] = AffineMatrixData[2][0];
	rotData[0][1] = sinX;	rotData[1][1] = cosX;	rotData[2][1] = AffineMatrixData[2][1];

	return roTrans;
}
