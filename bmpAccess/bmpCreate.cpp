#include <bmpAccess/bmpCreate.h>

void createDataHeader(int width, int height, int numColors, BMPHeader *bmpHeader) {
//	BMPHeader bmpHeader;
	
	bmpHeader->headerSize = 40;
	bmpHeader->BMPwidth = width;
	bmpHeader->BMPheight = height;
	bmpHeader->Compression = 0;
	bmpHeader->ImageDataSize = height*(width*numColors+(4-(width*numColors%4)));
	bmpHeader->WidthRes = 1024;
	bmpHeader->HeightRes = 1024;
	bmpHeader->NumColors = 0;
	bmpHeader->ImpColors=0;
	bmpHeader->CPlanes = 1;
	bmpHeader->bitCount = numColors*8;
	
//	return bmpHeader;
}

void createFileHeader(BMPHeader *bmpHeader, FileHeader *fileHeader){
//	FileHeader fileHeader;

	if(bmpHeader->bitCount == 8)fileHeader->dataOffset = 1078;
	else fileHeader->dataOffset = 54;

	fileHeader->fileSize = bmpHeader->ImageDataSize+fileHeader->dataOffset;

//	return fileHeader;
}

