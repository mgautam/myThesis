#include <DSP/resample.h>
#include <DSP/Pyramids.h>
#include <DSP/testDSP.h>
#include <bmpAccess/bmpEngine.h>

#define TRAIN_FOLDER "C:\\Users\\Gautam\\Desktop\\Project\\01.Training"
#define TEST_FOLDER "C:\\Users\\Gautam\\Desktop\\Project\\02.Test"
#define sigma 1/1.4142
#define numOctaves 4
#define numBlurs 5


void testDSP (void) {
	char *srcFile = "C:\\Users\\Gautam\\Desktop\\Project\\00.Test_Images\\lena.bmp";
	char *ProjectFolder = TRAIN_FOLDER;
	IMAGE *visual;
	GIMAGE *inImage;
	
	
	inImage = createImage(1024,1024,1);
	visual = readGrey(srcFile);
	resample(Gtype(visual),2,1,inImage);
	//inImage = Gtype(readGrey(imageName));//"C:\\Users\\Gautam\\Desktop\\Project\\00.Test_Images\\testSquare.bmp"));
	GIMAGE**** Pyramid = LaplacianPyramid(inImage, sigma,  numOctaves, numBlurs, ProjectFolder);
	releaseImage(inImage);
}