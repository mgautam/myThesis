#include <bmpAccess/testImageEngine.h>
#include <DSP/testDSP.h>
#include <CriticalPoints/testCriticalPoints.h>
#include <Matrix/testMatrix.h>
#include <AffineTransformer/testFrames.h>
#include <ModelAffine/testAffineFit.h>
#include <MotionExtract/testMotionExtract.h>
#include <Features/testMatchFeatures.h>

#include <API/videoProcessor.h>

#include <ModelAffine/coordinates.h>
#include <ModelAffine/fitAffineMatrix.h>
#include <MotionExtract/MotionExtract.h>
#include <iostream>
using namespace std;

#include <stdlib.h>

int main (int argc, char **argv) {
	//Create Rotation & Translation of smaller objects in a larger image
	//testImageEngine ();
	//testDSP ();
	//testCriticalPoints ();
	//testMatrix ();
	testFrames ();
	//testAffineModel ();
	//testMotionExtract ();
	//testMatchFeatures ();
	
	processFrames (-1,0,false);

	system ("pause");

	return 0;
}