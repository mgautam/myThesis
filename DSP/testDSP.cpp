#include <DSP/resample.h>
#include <DSP/Pyramids.h>
#include <DSP/testDSP.h>
#include <bmpAccess/bmpEngine.h>

#define TRAIN_FOLDER ".\\TestRepo\\01.Training"
#define TEST_FOLDER ".\\TestRepo\\02.Test"
#define sigma 1/1.4142
#define numOctaves 4
#define numBlurs 5


void testDSP (void) {
	char *srcFile = ".\\TestRepo\\00.Test_Images\\lena.bmp";
	char *ProjectFolder = TRAIN_FOLDER;
	IMAGE *visual;
	GIMAGE *inImage;
	
	
	inImage = createImage(1024,1024,1);
	visual = readGrey(srcFile);
	resample(Gtype(visual),2,1,inImage);
	//inImage = Gtype(readGrey(imageName));//".\\TestRepo\\00.Test_Images\\testSquare.bmp"));
	GIMAGE**** Pyramid = LaplacianPyramid(inImage, sigma,  numOctaves, numBlurs, ProjectFolder);
	releaseImage(inImage);
}