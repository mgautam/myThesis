#include <iostream>
using namespace std;
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <bmpAccess/bmpRead.h>

#define TWO_BYTES 2
#define FOUR_BYTES 4
#define MULTIPLY_BYTE_POSITION (2<<((8*byteIndex)))/2 /* why /2 is required? */

using namespace std;

void readFileHeader(FILE *fhandle, FileHeader *fileHeader, FILE *logFile) {
	unsigned char *nullDump;//I don't have to care about the data structure hence I use void *
	
	char *bmtype = new char[3];
	fread(bmtype,sizeof(char),TWO_BYTES,fhandle);
	bmtype[2] = '\0';
	if (strcmp("BM",bmtype)) {
		if(logFile) fprintf (logFile,"Error: Invalid BMP Header");
		exit(-1);
	}
	delete bmtype;

	unsigned char *cfsize = new unsigned char[FOUR_BYTES];
	fread(cfsize,sizeof(unsigned char),FOUR_BYTES,fhandle);
	fileHeader->fileSize = 0;
	for(int byteIndex = 0; byteIndex < FOUR_BYTES; byteIndex++)
		fileHeader->fileSize += (int)(*(cfsize+byteIndex)*MULTIPLY_BYTE_POSITION);//Remove pow and add shift << operators here// so you can remove math.h
	if(logFile) fprintf (logFile, "\tFile Size : \t\t%d\n", fileHeader->fileSize);
	delete cfsize;

	nullDump = new unsigned char[FOUR_BYTES];
	fread(nullDump,sizeof(unsigned char),FOUR_BYTES,fhandle);
	delete nullDump;

	unsigned char *cdataOffset = new unsigned char[FOUR_BYTES];	
	fread(cdataOffset,sizeof(unsigned char),FOUR_BYTES,fhandle);
	fileHeader->dataOffset = 0;
	for(int byteIndex = 0; byteIndex < FOUR_BYTES; byteIndex++)
		fileHeader->dataOffset += (int)(*(cdataOffset+byteIndex)*MULTIPLY_BYTE_POSITION);
	if(logFile) fprintf (logFile, "\tData Offset : \t\t%d\n", fileHeader->dataOffset);
	delete cdataOffset;
}

void readDataHeader(FILE *fhandle, BMPHeader *bmpHeader, FILE *logFile) {
	//BMPHeader bmpHeader;

	unsigned char *chsize = new unsigned char[FOUR_BYTES];
	fread(chsize,sizeof(unsigned char),FOUR_BYTES,fhandle);
	bmpHeader->headerSize = 0;
	for(int byteIndex = 0; byteIndex < FOUR_BYTES; byteIndex++)
		bmpHeader->headerSize += (int)(*(chsize+byteIndex)*MULTIPLY_BYTE_POSITION);
	if(logFile) fprintf (logFile, "\tData Header Size : \t%d\n", bmpHeader->headerSize);
	delete chsize;

	unsigned char *cBMPwidth = new unsigned char[FOUR_BYTES];
	fread(cBMPwidth,sizeof(unsigned char),FOUR_BYTES,fhandle);
	bmpHeader->BMPwidth = 0;
	for(int byteIndex = 0; byteIndex < FOUR_BYTES; byteIndex++)
		bmpHeader->BMPwidth += (int)(*(cBMPwidth+byteIndex)*MULTIPLY_BYTE_POSITION);
	if(logFile) fprintf (logFile, "\tImage Width : \t\t%d\n", bmpHeader->BMPwidth);
	delete cBMPwidth;

	unsigned char *cBMPheight = new unsigned char[FOUR_BYTES];
	fread(cBMPheight,sizeof(unsigned char),FOUR_BYTES,fhandle);
	bmpHeader->BMPheight = 0;
	for(int byteIndex = 0; byteIndex < FOUR_BYTES; byteIndex++)
		bmpHeader->BMPheight += (int)(*(cBMPheight+byteIndex)*MULTIPLY_BYTE_POSITION);
	if(logFile) fprintf (logFile, "\tImage Height : \t\t%d\n", bmpHeader->BMPheight);
	delete cBMPheight;

	unsigned char *cCPlanes = new unsigned char[TWO_BYTES];
	fread(cCPlanes,sizeof(unsigned char),TWO_BYTES,fhandle);
	bmpHeader->CPlanes = *cCPlanes+*(cCPlanes+1)*256;
	if(logFile) fprintf (logFile, "\tNumber of Color Planes : %d\n", bmpHeader->CPlanes);
	delete cCPlanes;

	unsigned char *cbitCount = new unsigned char[TWO_BYTES];
	fread(cbitCount,sizeof(unsigned char),TWO_BYTES,fhandle);
	bmpHeader->bitCount = *cbitCount+*(cbitCount+1)*256;
	if(logFile) fprintf (logFile, "\tBits per Pixel : \t%d\n", bmpHeader->bitCount);
	delete cbitCount;

	unsigned char *cCompression = new unsigned char[FOUR_BYTES];
	fread(cCompression,sizeof(unsigned char),FOUR_BYTES,fhandle);
	bmpHeader->Compression = 0;
	for(int byteIndex = 0; byteIndex < FOUR_BYTES; byteIndex++)
		bmpHeader->Compression += (int)(*(cCompression+byteIndex)*MULTIPLY_BYTE_POSITION);
	if(logFile) fprintf (logFile, "\tCompression : \t\t%d\n", bmpHeader->Compression);
	delete cCompression;

	unsigned char *cImageDataSize = new unsigned char[FOUR_BYTES];
	fread(cImageDataSize,sizeof(unsigned char),FOUR_BYTES,fhandle);
	bmpHeader->ImageDataSize = 0;
	for(int byteIndex = 0; byteIndex < FOUR_BYTES; byteIndex++)
		bmpHeader->ImageDataSize += (int)(*(cImageDataSize+byteIndex)*MULTIPLY_BYTE_POSITION);
	if(logFile) fprintf (logFile, "\tRaster Size : \t\t%d\n", bmpHeader->ImageDataSize);
	delete cImageDataSize;

	unsigned char *cWidthRes = new unsigned char[FOUR_BYTES];
	fread(cWidthRes,sizeof(unsigned char),FOUR_BYTES,fhandle);
	bmpHeader->WidthRes = 0;
	for(int byteIndex = 0; byteIndex < FOUR_BYTES; byteIndex++)
		bmpHeader->WidthRes += (int)(*(cWidthRes+byteIndex)*MULTIPLY_BYTE_POSITION);
	if(logFile) fprintf (logFile, "\tWidth Resolution : \t%d\n", bmpHeader->WidthRes);
	delete cWidthRes;

	unsigned char *cHeigthRes = new unsigned char[FOUR_BYTES];
	fread(cHeigthRes,sizeof(unsigned char),FOUR_BYTES,fhandle);
	bmpHeader->HeightRes = 0;
	for(int byteIndex = 0; byteIndex < FOUR_BYTES; byteIndex++)
		bmpHeader->HeightRes += (int)(*(cHeigthRes+byteIndex)*MULTIPLY_BYTE_POSITION);
	if(logFile) fprintf (logFile, "\tHeight Resolution : \t%d\n", bmpHeader->HeightRes);
	delete cHeigthRes;

	unsigned char *cNumColors = new unsigned char[FOUR_BYTES];
	fread(cNumColors,sizeof(unsigned char),FOUR_BYTES,fhandle);
	bmpHeader->NumColors = 0;
	for(int byteIndex = 0; byteIndex < FOUR_BYTES; byteIndex++)
		bmpHeader->NumColors += (int)(*(cNumColors+byteIndex)*MULTIPLY_BYTE_POSITION);
	if(logFile) fprintf (logFile, "\tNumber of Colors Used : %d\n", bmpHeader->NumColors);
	delete cNumColors;

	unsigned char *cImpColors = new unsigned char[FOUR_BYTES];
	fread(cImpColors,sizeof(unsigned char),FOUR_BYTES,fhandle);
	bmpHeader->ImpColors = 0;
	for(int byteIndex = 0; byteIndex < FOUR_BYTES; byteIndex++)
		bmpHeader->ImpColors += (int)(*(cImpColors+byteIndex)*MULTIPLY_BYTE_POSITION);
	if(logFile) fprintf (logFile, "\tImportant Colors : \t%d\n", bmpHeader->ImpColors);
	delete cImpColors;
}

IMAGE* readRaster(FILE *fhandle,FileHeader *fileHeader, BMPHeader *bmpHeader, FILE *logFile) {
	unsigned char *nullDump;//I don't have to care about the data structure hence I use void *
	int numPixels = (int)(bmpHeader->BMPwidth*bmpHeader->BMPheight);

	IMAGE *image = new IMAGE;
	image->width = bmpHeader->BMPwidth;
	image->height = bmpHeader->BMPheight;
	image->numColors = bmpHeader->bitCount/8;
	image->imageData = new unsigned char[numPixels*image->numColors];
		
	fseek(fhandle,fileHeader->dataOffset,0);
	int zeroPads = (image->width*image->numColors) % FOUR_BYTES;
	if(zeroPads == 0) fread(image->imageData,1,image->numColors*numPixels,fhandle);
	else {
		nullDump = new unsigned char[FOUR_BYTES-zeroPads];
		if(logFile) fprintf (logFile,"\tZeros = %d\n",FOUR_BYTES-zeroPads);
		for(int row = 0; row < image->height; row++) {
			fread(image->imageData+row*image->width*image->numColors,image->numColors,image->width,fhandle);//fread(imageData+n*BMPwidth,sizeof(unsigned char),BMPwidth*sizeof(BGR),fhandle);//THis can also be done
			fread(nullDump,sizeof(unsigned char),FOUR_BYTES-zeroPads,fhandle);
		}
		delete nullDump;
	}

	return image;
}


IMAGE* readLuminance(FILE *fhandle, FileHeader *fileHeader, BMPHeader *bmpHeader, FILE *logFile)
{
	unsigned char *nullDump;//I don't have to care about the data structure hence I use void *
	int numPixels = (int)(bmpHeader->BMPwidth*bmpHeader->BMPheight);
	
	int input_numColors = bmpHeader->bitCount/8;

	IMAGE *image = new IMAGE;
	image->width = bmpHeader->BMPwidth;
	image->height = bmpHeader->BMPheight;
	image->numColors = 1;
	image->imageData = new unsigned char[numPixels];
	
	fseek(fhandle,fileHeader->dataOffset,0);
	int zeroPads = (image->width*input_numColors) % FOUR_BYTES;
	unsigned char tempBGR[3];
	if(zeroPads == 0) 
		for(int row = 0; row < image->height; row++)
			for(int col = 0; col < image->width; col++)
			{
				fread(tempBGR,input_numColors,1,fhandle);
				*(image->imageData+row*image->width+col) = (unsigned char)(0.3*(GTYPE)tempBGR[0] + 0.59*(GTYPE)tempBGR[1] + 0.11*(GTYPE)tempBGR[2]);//Y = 0.3*B+0.59*G+0.11*R
			}
	else
	{
		nullDump = new unsigned char[FOUR_BYTES-zeroPads];
		if(logFile) fprintf (logFile,"\tZeros = %d\n",FOUR_BYTES-zeroPads);
		for(int row = 0; row < image->height; row++) {
			for(int col = 0; col < image->width; col++) {
				fread(tempBGR,input_numColors,1,fhandle);
				*(image->imageData+row*image->width+col) = (unsigned char)(0.3*(GTYPE)tempBGR[0] + 0.59*(GTYPE)tempBGR[1] + 0.11*(GTYPE)tempBGR[2]);//Y = 0.3*B+0.59*G+0.11*R
			}
			fread(nullDump,sizeof(unsigned char),FOUR_BYTES-zeroPads,fhandle);
		}
		delete nullDump;
	}
	return image;
}
