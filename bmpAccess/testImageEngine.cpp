#include <bmpAccess/bmpEngine.h>

void testImageEngine (void) {
	GIMAGE *input = Gtype(readGrey("./TestRepo/00.Test_Images/lena.bmp"));
	writeImage ("./TestRepo/00.Test_Images/lenax.bmp",input);
}