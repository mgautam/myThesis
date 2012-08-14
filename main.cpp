#include <bmpAccess/testImageEngine.h>
#include <DSP/testDSP.h>
#include <DSP/testnoise.h>
#include <CriticalPoints/testCriticalPoints.h>
#include <Matrix/testMatrix.h>
#include <AffineTransformer/testFrames.h>
#include <AffineTransformer/testBlocks.h>
#include <ModelAffine/testAffineFit.h>
#include <MotionExtract/testMotionExtract.h>
#include <Features/testMatchFeatures.h>

#include <API/videoProcessor.h>
#include <AffineTransformer/frames.h>
#include <ModelAffine/coordinates.h>
#include <ModelAffine/fitAffineMatrix.h>
#include <MotionExtract/MotionExtract.h>

#include <iostream>
using namespace std;

#include <stdlib.h>

int main (int argc, char **argv)
{
	//Create Rotation & Translation of smaller objects in a larger image
	//testImageEngine ();
	//testNoise ();
	//testDSP ();
	//testCriticalPoints ();
	//testMatrix ();
	//testFrames ();
	//testBlocks ();
	//testAffineModel ();
	//testMotionExtract ();
	//testMatchFeatures ();
	
	//createFrames ("./TestRepo/00.Test_Images/lena.bmp", "./TestRepo/00.Test_Images/Frames", 100);//,"./TestRepo/00.Test_Images/lena.bmp");
	
	FILE *angles = fopen ("./TestRepo/02.Test/estimated_rotation.bin","wb");
	FILE *logFile = fopen ("./TestRepo/log.txt","w");
	processFrames (-1,angles);//(stdout, -1, angles);
	fclose (logFile);
	fclose (angles);
	
	system ("pause");

	return 0;
}