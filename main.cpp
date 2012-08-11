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

int main (int argc, char **argv) {
	//testImageEngine ();
	//testDSP ();
	//testCriticalPoints ();
	//testMatrix ();
	//testFrames ();
	//testAffineModel ();
	//testMotionExtract ();
	//testMatchFeatures ();
	
	processFrames ();

	system ("pause");

	return 0;
}