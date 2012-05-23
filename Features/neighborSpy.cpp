#include <stdio.h>
#include <stdlib.h>
#include<math.h>

#include <DSP/filter.h>
#include <FEATURES/neighborSpy.h>

#define MAX_FILE_NAME_LENGTH 100

#define MIN_PIXEL_VALUE 0
#define MAX_PIXEL_VALUE 255

#define NUMBER_OF_ORIENTATION_BINS_I 36.0

#define PEAK_THRESHOLD 0.8

#include <iostream>
using namespace std;

static double bilinearInterpolate (double x, double y, GTYPE *Array, int width, int height) {

	double value = 0;
	if (x > 0 && x < width && y > 0 && y < height)
		value += abs ( ((double)((int)(x+0.5)) - x)* ((double)((int)(y+0.5))-y) ) * Array[(int)(x)*width+(int)(y)];
	if (x > 0 && x < width && (y+0.5) > 0 && (y+0.5) < height)
		value += abs ( ((double)((int)(x+0.5)) - x)* ((double)((int)(y))-y) ) * Array[(int)(x)*width+(int)(y)];
	if ((x+0.5) > 0 && (x+0.5) < width && y > 0 && y < height)
		value += abs ( ((double)((int)(x)) - x)* ((double)((int)(y+0.5))-y) ) * Array[(int)(x+0.5)*width+(int)(y+0.5)];
	if ((x+0.5) > 0 && (x+0.5) < width && (y+0.5) > 0 && (y+0.5) < height)
		value += abs ( ((double)((int)(x)) - x)* ((double)((int)(y))-y) ) * Array[(int)(x+0.5)*width+(int)(y+0.5)];

	double Norm = abs ( ((double)((int)(x+0.5)) - x)* ((double)((int)(y+0.5))-y) ) +
		abs ( ((double)((int)(x+0.5)) - x) * (y - (double)((int) y)) ) +
		abs ( (x-(double)((int)x)) * ((double)((int)(y+0.5)) - y) ) +
		abs ( (x-(double)((int)x)) * (y - (double)((int) y)) );

	return value/Norm;
}

int magoriCalc(GIMAGE *GaussPix, IMAGE *ExPix, int octave, int blur, char *Project_Folder) {
	FILE *magFile,*oriFile;
	char filename[MAX_FILE_NAME_LENGTH];
	sprintf(filename,"%s\\04.Gradient_Pyramid\\%d%d.bin",Project_Folder,octave,blur);
	magFile = fopen(filename,"wb");
	sprintf(filename,"%s\\05.Orientation_Pyramid\\%d%d.bin",Project_Folder,octave,blur);
	oriFile = fopen(filename,"wb");
		
	GTYPE *Magnitudes = new GTYPE[GaussPix->width*GaussPix->height];
	GTYPE maxMag = MIN_PIXEL_VALUE, minMag = MAX_PIXEL_VALUE;
	GTYPE *orientations = new GTYPE[GaussPix->width*GaussPix->height];
	
	for(int n1 = 0; n1 < GaussPix->height-1; n1++) {
		for(int n2 = 0; n2 < GaussPix->width-1; n2++) {
			Magnitudes[n1*GaussPix->width+n2] = sqrt(pow(GTYPE(GaussPix->imageData[n1*GaussPix->width+n2]-GaussPix->imageData[(n1+1)*GaussPix->width+n2]),2) 
				+ pow(GTYPE(GaussPix->imageData[n1*GaussPix->width+n2]-GaussPix->imageData[n1*GaussPix->width+n2+1]),2));
			if(Magnitudes[n1*GaussPix->width+n2] > maxMag) maxMag = Magnitudes[n1*GaussPix->width+n2];
			if(Magnitudes[n1*GaussPix->width+n2] < minMag) minMag = Magnitudes[n1*GaussPix->width+n2];			

			orientations[n1*GaussPix->width+n2] = atan2(GTYPE(GaussPix->imageData[n1*GaussPix->width+n2]-GaussPix->imageData[(n1+1)*GaussPix->width+n2]), 
				GTYPE(GaussPix->imageData[n1*GaussPix->width+n2]-GaussPix->imageData[n1*GaussPix->width+n2+1]));			
		}
		Magnitudes[n1*GaussPix->width+GaussPix->width-1] = 0;
	}
	for(int n2 = 0; n2 < GaussPix->width; n2++)
		Magnitudes[(GaussPix->height-1)*GaussPix->width+GaussPix->width-1] = 0;
			
	fwrite(&maxMag,sizeof(GTYPE),1,magFile);
	fwrite(&minMag,sizeof(GTYPE),1,magFile);
	fwrite(Magnitudes,sizeof(GTYPE),GaussPix->width*GaussPix->height,magFile);
	fwrite(orientations,sizeof(GTYPE),GaussPix->width*GaussPix->height,oriFile);
	//cout << index << ". MaxMag = " << maxMag << " , MinMag = " << minMag << endl;
	fclose(magFile);
	fclose(oriFile);
	
	delete orientations;
	
	int numKeys = 0;
	GTYPE threshold = maxMag/10.0;	
	for(int n1 = 0; n1 < ExPix->height-1; n1++)
		for(int n2 = 0; n2 < ExPix->width-1; n2++)
			if(ExPix->imageData[n1*ExPix->width+n2] == MAX_PIXEL_VALUE && Magnitudes[n1*ExPix->width+n2] > threshold )
				numKeys++;

	delete Magnitudes;

	cout << "\t\tStable Keypoints: " << numKeys << endl;
	return numKeys;	
}

void angleKeyCalc(IMAGE *ExPix, int numKeys, int octave, int blur, char* Project_Folder) {

	int number_of_sectors = FEATURE_LENGTH;

	int window_radius = 15;	
	
	// Sector Mask (Window) Initialization
	int ***sectorWindow = new int**[number_of_sectors];
	for (int sectorIndex = 0; sectorIndex < number_of_sectors; sectorIndex++) {
		sectorWindow[sectorIndex] = new int*[2*window_radius];
		for(int k1 = -window_radius; k1 < window_radius; k1++) {
			sectorWindow[sectorIndex][k1+window_radius] = new int[2*window_radius];
			for(int k2 = -window_radius; k2 < window_radius; k2++)
				sectorWindow[sectorIndex][k1+window_radius][k2+window_radius] = 0;
		}
	} 

	// Window Definition
		for(int k1 = -window_radius; k1 < 0; k1++)
			for(int k2 = -window_radius; k2 < 0; k2++)
				if ( sqrt ((double)(k1*k1 + k2*k2)) < window_radius )
					sectorWindow[ number_of_sectors/2+(int)((atan ((double)k1/(double)k2)+pi/2.0) * (double)number_of_sectors/(2*pi))][k1+window_radius][k2+window_radius] = 1;
		for(int k1 = -window_radius; k1 < window_radius; k1++)
			for(int k2 = 0; k2 < window_radius; k2++)
				if ( sqrt ((double)(k1*k1 + k2*k2)) < window_radius )
					sectorWindow[ (int)((atan ((double)k1/(double)k2)+pi/2.0) * (double)number_of_sectors/(2*pi))][k1+window_radius][k2+window_radius] = 1;
		for(int k1 = 0; k1 < window_radius; k1++)
			for(int k2 = 0; k2 < window_radius; k2++)
				if ( sqrt ((double)(k1*k1 + k2*k2)) < window_radius )
					sectorWindow[(int)((atan ((double)k1/(double)k2)+pi/2.0) * (double)number_of_sectors/(2*pi))][k1+window_radius][k2+window_radius] = 1;
		for(int k1 = 0; k1 < window_radius; k1++)
			for(int k2 = -window_radius; k2 < 0; k2++)
				if ( sqrt ((double)(k1*k1 + k2*k2)) < window_radius )
					sectorWindow[ number_of_sectors/2+(int)((atan ((double)k1/(double)k2)+pi/2.0) * (double)number_of_sectors/(2*pi))][k1+window_radius][k2+window_radius] = 1;
		
/*
	for (int sectorIndex = 0; sectorIndex < number_of_sectors; sectorIndex++) {
		for(int k1 = -window_radius; k1 < window_radius; k1++) {
			for(int k2 = -window_radius; k2 < window_radius; k2++)
				cout << sectorWindow[sectorIndex][k1+window_radius][k2+window_radius] << " ";
			cout << endl;
		}
		cout << endl;
	}
*/	

	char filename[MAX_FILE_NAME_LENGTH];	

	FILE *magFile,*oriFile;
	GTYPE *Magnitudes = new GTYPE[ExPix->width*ExPix->height];
	GTYPE maxMag, minMag;
	GTYPE *orientations = new GTYPE[ExPix->width*ExPix->height];

	sprintf(filename,"%s\\04.Gradient_Pyramid\\%d%d.bin",Project_Folder,octave,blur);
	magFile = fopen(filename,"rb");
	fread(&maxMag,sizeof(GTYPE),1,magFile);
	fread(&minMag,sizeof(GTYPE),1,magFile);
	fread(Magnitudes,sizeof(GTYPE),ExPix->width*ExPix->height,magFile);
	fclose(magFile);

	sprintf(filename,"%s\\05.Orientation_Pyramid\\%d%d.bin",Project_Folder,octave,blur);
	oriFile = fopen(filename,"rb");
	fread(orientations,sizeof(GTYPE),ExPix->width*ExPix->height,oriFile);
	fclose(oriFile);
	

	GTYPE gaussFilter[81];
	GaussianFilter2 (0, 1.5*1.5*blur, -4, 9, gaussFilter);
	GTYPE oriBins[(int)NUMBER_OF_ORIENTATION_BINS_I];

	angleKey *AngleDescriptor = new angleKey[(int)(NUMBER_OF_ORIENTATION_BINS_I*numKeys)];
	int keyIndex = 0;
	GTYPE Magnif = 1.0 / pow(2.0,octave);
	GTYPE stdev = 1.5 * Magnif;
	int sectorOffset = 0;
	int totalNeighbors = 0;
	for(int n1 = 0; n1 < (ExPix->height)-1; n1++) {
		for(int n2 = 0; n2 < (ExPix->width)-1; n2++) {
			
			if(ExPix->imageData[n1*ExPix->width+n2] == MAX_PIXEL_VALUE ) {

				for(int i = 0; i < NUMBER_OF_ORIENTATION_BINS_I; i++) 
						oriBins[i] = 0;
					
				// aren't the orientations given by atan only distributed between -pi/2 to pi/2
				// How can we say that they occupy the whole 2*pi space
					for(float k1=-3.5; k1 < 4; k1++)
						for(float k2=-3.5; k2 < 4; k2++)
							if((n1+k1*Magnif) > 0 && (n1+k1*Magnif) < ExPix->height && (n2+k2*Magnif) > 0 && (n2+k2*Magnif) < ExPix->width)
								oriBins[(int) ((bilinearInterpolate ((n1+k1*Magnif),(n2+k2*Magnif),orientations,ExPix->width,ExPix->height)/(2.0*pi) + 0.5) * NUMBER_OF_ORIENTATION_BINS_I)] 
										+= bilinearInterpolate ((n1+k1*Magnif),(n2+k2*Magnif),Magnitudes,ExPix->width,ExPix->height)
											* exp(-pow((GTYPE)k1*Magnif,2)/(2.0*pow(stdev,2)))/(stdev*sqrt(2.0*pi))
												* exp(-pow((GTYPE)k2*Magnif,2)/(2.0*pow(stdev,2)))/(stdev*sqrt(2.0*pi));
										

					double maxBin = 0;
					for(int i=0; i < NUMBER_OF_ORIENTATION_BINS_I; i++)
						if(maxBin < oriBins[i])
							maxBin = oriBins[i];
					
					int numMax = 0;
					AngleDescriptor[keyIndex].KeyOrientation = 0;
					for(int i=0; i < NUMBER_OF_ORIENTATION_BINS_I; i++)
						if(oriBins[i] > PEAK_THRESHOLD * maxBin) {
							AngleDescriptor[keyIndex+numMax].KeyOrientation = (((double) i) / NUMBER_OF_ORIENTATION_BINS_I - 0.5)*(2*pi);
							numMax++;						
						}

				int subIndex = 0;
				for ( ; subIndex < numMax; subIndex++ ) {

					AngleDescriptor[keyIndex].x = n1;
					AngleDescriptor[keyIndex].y = n2;
					AngleDescriptor[keyIndex].scale = Magnif;
					
					// Check whether I have to change the offset to KeyOrientation as -pi/2 < atanX < pi/2
					sectorOffset = (AngleDescriptor[keyIndex].KeyOrientation + pi)* number_of_sectors / (2*pi);

					int totalNeighbors = 0;
					for (int sectorIndex = 0 ; sectorIndex < number_of_sectors; sectorIndex++) {
						AngleDescriptor[keyIndex].sectorCount[sectorIndex] = 0;
						for (int k1 = -window_radius; k1 < window_radius; k1++)
							for (int k2 = -window_radius; k2 < window_radius; k2++)
							// I should be able to add single side edges
								if ( n1 + k1 < (ExPix->height - 1) && n1 + k1 >= 0)
									if ( n2 + k2 < (ExPix->width - 1) && n2 + k2 >= 0) {
										AngleDescriptor[keyIndex].sectorCount[sectorIndex] 
											+= ExPix->imageData[(n1+k1)*ExPix->width+(n2+k2)] 
												* sectorWindow[(sectorIndex + sectorOffset) % number_of_sectors][k1+window_radius][k2+window_radius];
									}
						AngleDescriptor[keyIndex].sectorCount[sectorIndex] /= MAX_PIXEL_VALUE;
						totalNeighbors += AngleDescriptor[keyIndex].sectorCount[sectorIndex];
					}				
					if (totalNeighbors > 10) keyIndex++;
				}				
			}
		}
	}
	
	cout << "\t\tKeys with Orientation = " << keyIndex << endl;
	FILE *keyFile;
	sprintf(filename,"%s\\07.Angle_Keys\\%d%d.bin",Project_Folder,octave,blur);
	keyFile = fopen(filename,"wb");
	fwrite(&keyIndex,sizeof(int),1,keyFile);
	fwrite(AngleDescriptor,sizeof(angleKey),keyIndex,keyFile);
	fclose(keyFile);


	// Garbage Collection
	for (int sectorIndex = 0; sectorIndex < number_of_sectors; sectorIndex++) {
		for(int k1 = -window_radius; k1 < window_radius; k1++)
			delete sectorWindow[sectorIndex][k1+window_radius];
		delete sectorWindow[sectorIndex];
	}
	delete sectorWindow;

	delete Magnitudes;
	delete orientations;
	delete AngleDescriptor;	
}


void writeAllSift (GTYPE sigma, int numOctaves, int numBlurs, char *PROJECT_FOLDER, int frameIndex) {
	FILE *keyFile;
	int numKeys,totalKeys = 0;
	angleKey *AngleDescriptor;
	GTYPE scale;
	char filename[MAX_FILE_NAME_LENGTH];

	if (frameIndex == -1) sprintf (filename,"%s\\07.Angle_Keys\\train.bin",PROJECT_FOLDER);
	else sprintf (filename,"%s\\07.Angle_Keys\\testFeature(%d).bin",PROJECT_FOLDER,frameIndex);
	FILE *AllKeyFile = fopen (filename,"wb");

	int featurelength = FEATURE_LENGTH;
	fwrite(&featurelength,sizeof(int),1,AllKeyFile);
	fseek(AllKeyFile,sizeof(int),SEEK_CUR); // Allocated space to write Total Number of features
	
	for (int i = 0; i < numOctaves; i++)
		for (int j = 1; j < numBlurs-2; j++) {
			sprintf(filename,"%s\\07.Angle_Keys\\%d%d.bin",PROJECT_FOLDER,i,j);
			keyFile = fopen(filename,"rb");
			fread(&numKeys,sizeof(int),1,keyFile);
			AngleDescriptor = new angleKey[numKeys];
			fread(AngleDescriptor,sizeof(angleKey),numKeys,keyFile);
			fclose(keyFile);

			fwrite(AngleDescriptor,sizeof(angleKey),numKeys,AllKeyFile);
			totalKeys += numKeys;
			delete AngleDescriptor;
		}
	fseek(AllKeyFile,sizeof (int),SEEK_SET); // Skip over previous written Feature Length =128
	fwrite(&totalKeys,sizeof(int),1,AllKeyFile);
	fclose(AllKeyFile);	
}
