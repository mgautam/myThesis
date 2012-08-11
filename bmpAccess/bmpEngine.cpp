#include <iostream>
using namespace std;
#include <stdio.h>

#include <bmpAccess/bmpRead.h>
#include <bmpAccess/bmpCreate.h>
#include <bmpAccess/bmpWrite.h>
#include <bmpAccess/bmpEngine.h>

 IMAGE* readRGB(char *fileName, bool printInfo){
	if(printInfo) printf("Reading File...\n");
	FileHeader fileHeader;
	BMPHeader bmpHeader;
	
	FILE *fhandle = fopen(fileName,"rb");	
	readFileHeader(fhandle, &fileHeader, printInfo);
	readDataHeader(fhandle, &bmpHeader, printInfo);
	IMAGE *image = readRaster(fhandle, &fileHeader, &bmpHeader, printInfo);
	fclose(fhandle);

	return image;
}

 IMAGE* readGrey(char *fileName, bool printInfo){
	if(printInfo) printf("Reading File...\n");
	FileHeader fileHeader;
	BMPHeader bmpHeader;

	FILE *fhandle = fopen(fileName,"rb");	
	readFileHeader(fhandle, &fileHeader, printInfo);	
	readDataHeader(fhandle, &bmpHeader, printInfo);
	
	IMAGE *image;
	if(bmpHeader.bitCount == 8) image = readRaster(fhandle, &fileHeader, &bmpHeader, printInfo);
	else image = readLuminance(fhandle, &fileHeader, &bmpHeader, printInfo);
	fclose(fhandle);

	return image;
}

 void writeImage(char *fileName, IMAGE *image, bool printInfo) {
	if(printInfo) printf("Writing File...\n");

	BMPHeader bmpHeader;
	FileHeader fileHeader;

	createDataHeader(image->width, image->height, image->numColors, &bmpHeader);
	createFileHeader(&bmpHeader,&fileHeader);
	
	FILE *fhandle = fopen(fileName,"wb");
	writeFileHeader(fhandle, fileHeader, printInfo);
	writeDataHeader(fhandle, bmpHeader, printInfo);
	if(image->numColors == 1) writeGreyPalette(fhandle);
	writeRaster(fhandle, image, printInfo);
	fclose(fhandle);
}
 void writeImage(char *fileName, GIMAGE *image, bool printInfo){
	IMAGE *outImage =  uChar(image);
	writeImage(fileName, outImage, printInfo);
	releaseImage (outImage);
}
 void writeImageNorm(char *fileName, GIMAGE *image, bool printInfo){
	IMAGE *outImage =  uCharNorm(image);
	writeImage(fileName, outImage, printInfo);
	releaseImage (outImage);
}



 IMAGE* createimage(int width, int height, int numColors) {
	IMAGE *image = new IMAGE;
	image->width = width;
	image->height = height;
	image->numColors = numColors;
	image->imageData = new unsigned char[width*height*numColors];
	return image;
}

 GIMAGE* createImage(int width, int height, int numColors) {
	GIMAGE *image = new GIMAGE;
	image->width = width;
	image->height = height;
	image->numColors = numColors;
	image->imageData = new GTYPE[width*height*numColors];
	return image;
}

 IMAGE* cloneImage(IMAGE* inImage) {
	IMAGE *image = new IMAGE;
	image->width = inImage->width;
	image->height = inImage->height;
	image->numColors = inImage->numColors;
	image->imageData = new unsigned char[image->width*image->height*image->numColors];
	for(int n1=0; n1 < image->height; n1++)
		for(int n2=0; n2 < image->width; n2++)
			for(int c=0; c < image->numColors; c++)
				image->imageData[(n1*image->width+n2)*image->numColors+c] = inImage->imageData[(n1*image->width+n2)*image->numColors+c];
	return image;
}

 GIMAGE* cloneImage(GIMAGE* inImage){
	GIMAGE *image = new GIMAGE;
	image->width = inImage->width;
	image->height = inImage->height;
	image->numColors = inImage->numColors;
	image->imageData = new GTYPE[image->width*image->height*image->numColors];
	for(int n1=0; n1 < image->height; n1++)
		for(int n2=0; n2 < image->width; n2++)
			for(int c=0; c < image->numColors; c++)
				image->imageData[(n1*image->width+n2)*image->numColors+c] = inImage->imageData[(n1*image->width+n2)*image->numColors+c];
	return image;
}



 IMAGE* Greylize(IMAGE *inImage){
	if(inImage->numColors == 1)
		return inImage;

	IMAGE *image = new IMAGE;
	image->width = inImage->width;
	image->height = inImage->height;
	image->numColors = 1;
	image->imageData = new unsigned char[inImage->width*inImage->height];

	
	for(int n1=0; n1 < image->height; n1++)
		for(int n2=0; n2 < image->width; n2++)
		{
			*(image->imageData+n1*image->width+n2) = (unsigned char)(0.3*(GTYPE)*(inImage->imageData+(n1*inImage->width+n2)*inImage->numColors)
				+ 0.59*(GTYPE)*(inImage->imageData+(n1*inImage->width+n2)*inImage->numColors+1)
			+ 0.11*(GTYPE)*(inImage->imageData+(n1*inImage->width+n2)*inImage->numColors+2));//Y = 0.3*B+0.59*G+0.11*R
			//cout << (int)*(image->imageData+n1*image->width+n2) <<" "<<(int)tempBGR[0]<<" "<<(int)tempBGR[1]<<" "<<(int)tempBGR[2]<< endl;
		}
	
	return image;
}

 GIMAGE* Gtype(IMAGE *inImage){
	GIMAGE *image = new GIMAGE;
	image->width = inImage->width;
	image->height = inImage->height;
	image->numColors = inImage->numColors;
	image->imageData = new GTYPE[inImage->width*inImage->height*image->numColors];

	for(int n1=0; n1 < image->height; n1++)
		for(int n2=0; n2 < image->width; n2++)
			for(int c=0; c < image->numColors; c++)
				*(image->imageData+(n1*image->width+n2)*image->numColors+c) = (GTYPE)(((GTYPE)*(inImage->imageData+(n1*inImage->width+n2)*inImage->numColors+c))-128.0)/255.0;
			
	
	return image;
}

 IMAGE* uChar(GIMAGE *inImage){
	IMAGE *image = new IMAGE;
	image->width = inImage->width;
	image->height = inImage->height;
	image->numColors = inImage->numColors;
	image->imageData = new unsigned char[inImage->width*inImage->height*image->numColors];

	for(int n1=0; n1 < image->height; n1++)
		for(int n2=0; n2 < image->width; n2++)
			for(int c=0; c < image->numColors; c++)
				*(image->imageData+(n1*image->width+n2)*image->numColors+c) = (unsigned char)(0.5+((*(inImage->imageData+(n1*inImage->width+n2)*inImage->numColors+c)*255.0)+128.0));
			
	
	return image;
}

 IMAGE* uCharNorm(GIMAGE *inImage){
	IMAGE *image = new IMAGE;
	image->width = inImage->width;
	image->height = inImage->height;
	image->numColors = inImage->numColors;
	image->imageData = new unsigned char[inImage->width*inImage->height*image->numColors];

	GTYPE *max = new GTYPE[inImage->numColors];
	GTYPE *min = new GTYPE[inImage->numColors];
	for(int c=0; c < image->numColors; c++)
	{
		max[c] = -1024;
		min[c] = 1024;
		for(int n1=0; n1 < image->height; n1++)
			for(int n2=0; n2 < image->width; n2++)
			{
				if(max[c] < *(inImage->imageData+(n1*inImage->width+n2)*inImage->numColors+c))
					max[c] = *(inImage->imageData+(n1*inImage->width+n2)*inImage->numColors+c);
				else if(min[c] > *(inImage->imageData+(n1*inImage->width+n2)*inImage->numColors+c))
					min[c] = *(inImage->imageData+(n1*inImage->width+n2)*inImage->numColors+c);
			}
	}
	
	for(int n1=0; n1 < image->height; n1++)
		for(int n2=0; n2 < image->width; n2++)
			for(int c=0; c < image->numColors; c++)	
				*(image->imageData+(n1*image->width+n2)*image->numColors+c) = (unsigned char)(((*(inImage->imageData+(n1*inImage->width+n2)*inImage->numColors+c)-min[c])/(max[c]-min[c]))*255.0);

	return image;
}



 void releaseImage(IMAGE *image){
	delete image->imageData;
	delete image;
}

 void releaseImage(GIMAGE *image){
	delete image->imageData;
	delete image;
}
