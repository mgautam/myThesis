#include <AffineTransformer/testFrames.h>
#include <AffineTransformer/frames.h>

void testFrames (void) {
	
	int translation[2] = {0,0};
	
	createFrames (".\\TestRepo\\00.Test_Images\\checkeredball.bmp", ".\\TestRepo\\00.Test_Images\\Frames", translation, 100);

}