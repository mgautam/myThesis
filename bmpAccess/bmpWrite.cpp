#include <stdio.h>
#include <string.h>

#include <bmpAccess/bmpWrite.h>

#define BYTE_POWER 256
#define TWO_BYTES 2
#define FOUR_BYTES 4
#define MULTIPLY_BYTE_POSITION (2<<((8*byteIndex)))/2 /* why /2 is required? */

void writeFileHeader(FILE *fhandle, FileHeader fileHeader, FILE *logFile) {

	char *bmtype = new char[3];
	strcpy(bmtype,"BM");
	fwrite(bmtype,sizeof(char),TWO_BYTES,fhandle);	
	delete bmtype;

	if(logFile) fprintf (logFile, "\tFile Size : \t\t %d\n", fileHeader.fileSize);
	unsigned char *cfsize = new unsigned char[FOUR_BYTES];
	for(int i = 0; i < FOUR_BYTES; i++)
	{
		*(cfsize+i) = fileHeader.fileSize % 256;
		fileHeader.fileSize /= 256;		
	}
	fwrite(cfsize,sizeof(unsigned char),FOUR_BYTES,fhandle);
	delete cfsize;

	//wnullDump = new char[5];
	//sprintf((char *)wnullDump,"0000");
	int *wnullDump;
	wnullDump = new int(0);
	fwrite(wnullDump,sizeof(char),FOUR_BYTES,fhandle);
	delete wnullDump;
	
	if(logFile) fprintf (logFile, "\tData Offset : \t\t %d\n", fileHeader.dataOffset);
	unsigned char *cdataOffset = new unsigned char[FOUR_BYTES];	
	for(int i = 0; i < FOUR_BYTES; i++)
	{
		*(cdataOffset+i) = fileHeader.dataOffset % 256;
		fileHeader.dataOffset /= 256;
	}
	fwrite(cdataOffset,sizeof(unsigned char),FOUR_BYTES,fhandle);
	delete cdataOffset;
	
	return;
}

void writeDataHeader(FILE *fhandle, BMPHeader bmpHeader, FILE *logFile)
{
	
	if(logFile) fprintf (logFile, "\tData Header Size : \t %d\n", bmpHeader.headerSize);
	unsigned char *chsize = new unsigned char[FOUR_BYTES];	
	for(int i = 0; i < FOUR_BYTES; i++)
	{
		*(chsize+i) = bmpHeader.headerSize % 256;
		bmpHeader.headerSize /= 256;
	}
	fwrite(chsize,sizeof(unsigned char),FOUR_BYTES,fhandle);
	delete chsize;
	
	if(logFile) fprintf (logFile, "\tImage Width : \t\t %d\n", bmpHeader.BMPwidth);
	unsigned char *cBMPwidth = new unsigned char[FOUR_BYTES];	
	for(int i = 0; i < FOUR_BYTES; i++)
	{
		*(cBMPwidth+i) = bmpHeader.BMPwidth % 256;
		bmpHeader.BMPwidth /= 256;
	}
	fwrite(cBMPwidth,sizeof(unsigned char),FOUR_BYTES,fhandle);
	delete cBMPwidth;

	if(logFile) fprintf (logFile, "\tImage Height : \t\t %d\n" ,bmpHeader.BMPheight);
	unsigned char *cBMPheight = new unsigned char[FOUR_BYTES];
	for(int i = 0; i < FOUR_BYTES; i++)
	{
		*(cBMPheight+i) = bmpHeader.BMPheight % 256;
		bmpHeader.BMPheight /= 256;
	}
	fwrite(cBMPheight,sizeof(unsigned char),FOUR_BYTES,fhandle);
	delete cBMPheight;

	if(logFile) fprintf (logFile, "\tNumber of Color Planes : %d\n", bmpHeader.CPlanes);
	unsigned char *cCPlanes = new unsigned char[TWO_BYTES];	
	for(int i = 0; i < TWO_BYTES; i++)
	{
		*(cCPlanes+i) = bmpHeader.CPlanes % 256;
		bmpHeader.CPlanes /= 256;
	}
	fwrite(cCPlanes,sizeof(unsigned char),TWO_BYTES,fhandle);
	delete cCPlanes;

	if(logFile) fprintf (logFile, "\tBits per Pixel : \t %d\n", bmpHeader.bitCount);
	unsigned char *cbitCount = new unsigned char[TWO_BYTES];	
	for(int i = 0; i < TWO_BYTES; i++)
	{
		*(cbitCount+i) = bmpHeader.bitCount % 256;
		bmpHeader.bitCount /= 256;
	}
	fwrite(cbitCount,sizeof(unsigned char),TWO_BYTES,fhandle);
	delete cbitCount;

	if(logFile) fprintf (logFile, "\tCompression : \t\t %d\n", bmpHeader.Compression);
	unsigned char *cCompression = new unsigned char[FOUR_BYTES];	
	for(int i = 0; i < FOUR_BYTES; i++)
	{
		*(cCompression+i) = bmpHeader.Compression % 256;
		bmpHeader.Compression /= 256;
	}
	fwrite(cCompression,sizeof(unsigned char),FOUR_BYTES,fhandle);
	delete cCompression;

	if(logFile) fprintf (logFile, "\tRaster Size : \t\t %d\n", bmpHeader.ImageDataSize);
	unsigned char *cImageDataSize = new unsigned char[FOUR_BYTES];	
	for(int i = 0; i < FOUR_BYTES; i++)
	{
		*(cImageDataSize+i) = bmpHeader.ImageDataSize % 256;
		bmpHeader.ImageDataSize /= 256;
	}
	fwrite(cImageDataSize,sizeof(unsigned char),FOUR_BYTES,fhandle);
	delete cImageDataSize;

	if(logFile) fprintf (logFile, "\tWidth Resolution : \t %d\n", bmpHeader.WidthRes);
	unsigned char *cWidthRes = new unsigned char[FOUR_BYTES];	
	for(int i = 0; i < FOUR_BYTES; i++)
	{
		*(cWidthRes+i) = bmpHeader.WidthRes % 256;
		bmpHeader.WidthRes /= 256;
	}
	fwrite(cWidthRes,sizeof(unsigned char),FOUR_BYTES,fhandle);
	delete cWidthRes;

	if(logFile) fprintf (logFile, "\tHeight Resolution : \t %d\n", bmpHeader.HeightRes);
	unsigned char *cHeigthRes = new unsigned char[FOUR_BYTES];	
	for(int i = 0; i < FOUR_BYTES; i++)
	{
		*(cHeigthRes+i) = bmpHeader.HeightRes % 256;
		bmpHeader.HeightRes /= 256;
	}
	fwrite(cHeigthRes,sizeof(unsigned char),FOUR_BYTES,fhandle);
	delete cHeigthRes;

	if(logFile) fprintf (logFile, "\tNumber of Colors Used : %d\n", bmpHeader.NumColors);
	unsigned char *cNumColors = new unsigned char[FOUR_BYTES];	
	for(int i = 0; i < FOUR_BYTES; i++)
	{
		*(cNumColors+i) = bmpHeader.NumColors % 256;
		bmpHeader.NumColors /= 256;
	}
	fwrite(cNumColors,sizeof(unsigned char),FOUR_BYTES,fhandle);
	delete cNumColors;

	if(logFile) fprintf (logFile, "\tImportant Colors : \t %d\n", bmpHeader.ImpColors);
	unsigned char *cImpColors = new unsigned char[FOUR_BYTES];	
	for(int i = 0; i < FOUR_BYTES; i++)
	{
		*(cImpColors+i) = bmpHeader.ImpColors % 256;
		bmpHeader.ImpColors /= 256;
	}
	fwrite(cImpColors,sizeof(unsigned char),FOUR_BYTES,fhandle);
	delete cImpColors;

	return;
}

void writeGreyPalette(FILE *fhandle) {
	for(int i=0; i<256; i++)
		fprintf(fhandle,"%c%c%c%c",i,i,i,0);		
}

void writeRaster(FILE *fhandle, IMAGE *image, FILE *logFile)
{
	//fprintf (logFile, "%d %d %d\n", image->height, image->numColors, image->width);
	char *wnullDump;
	int numPixels = (int)(image->width*image->height);
	int zeroPads = (image->width*image->numColors) % FOUR_BYTES;
	if(zeroPads == 0) fwrite(image->imageData,image->numColors,numPixels,fhandle);
	else
	{
		wnullDump = new char[FOUR_BYTES-zeroPads];
		for(int i=0;i < FOUR_BYTES-zeroPads; i++)
			*wnullDump = 0;
		if(logFile) fprintf(logFile, "\tZeros = %d\n",FOUR_BYTES-zeroPads);
		for(int n=0;n<image->height;n++)
		{			
			fwrite(image->imageData+n*image->width*image->numColors,image->numColors,image->width,fhandle);			
			fwrite(wnullDump,sizeof(unsigned char),FOUR_BYTES-zeroPads,fhandle);
		}
		delete wnullDump;
	}
	return;
}
