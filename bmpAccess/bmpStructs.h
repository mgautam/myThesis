#ifndef BMPSTRUCTS_H
#define BMPSTRUCTS_H

#define pi 3.1416

typedef struct{
	int fileSize, dataOffset;
} FileHeader;

typedef struct{
	int headerSize,BMPwidth,BMPheight,Compression,ImageDataSize,WidthRes,HeightRes,NumColors,ImpColors;
	unsigned short int CPlanes,bitCount;
} BMPHeader;

typedef struct{
	int width;
	int height;
	int numColors;
	unsigned char *imageData;
} IMAGE;

typedef double GTYPE;

typedef struct{
	int width;
	int height;
	int numColors;
	GTYPE *imageData;
} GIMAGE;

#endif