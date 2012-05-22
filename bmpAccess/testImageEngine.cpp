#include <bmpAccess/bmpEngine.h>

void testImageEngine (void) {
	GIMAGE *input = Gtype(readGrey("C:\\Users\\Gautam\\Desktop\\Project\\00.Test_Images\\lena.bmp",true));
	writeImage ("C:\\Users\\Gautam\\Desktop\\Project\\00.Test_Images\\lenax.bmp",input);
}