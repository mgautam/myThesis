#include <stdio.h>
#include <stdlib.h>
#include<math.h>

#include <DSP/filter.h>
#include <Features/neighborSpy.h>

#define MAX_FILE_NAME_LENGTH 100

#define MIN_PIXEL_VALUE 0
#define MAX_PIXEL_VALUE 255

#define NUMBER_OF_ORIENTATION_BINS_I 36.0

#define PEAK_THRESHOLD 0.8

#include <iostream>
using namespace std;

static double bilinearInterpolate (double x, double y, double *Array, int width, int height) {

	double value = 0;
	if ((int)x >= 0 && x < width && (int)y >= 0 && y < height)
		value += abs ( ((double)((int)(x+1)) - x)* ((double)((int)(y+1))-y) ) * Array[(int)(y)*width+(int)(x)];
	if ((int)x >= 0 && x < width && (int)(y+1) >= 0 && (y+1) < height)
		value += abs ( ((double)((int)(x+1)) - x)* ((double)((int)(y))-y) ) * Array[(int)(y+1)*width+(int)(x)];
	if ((int)(x+1) >= 0 && (x+1) < width && (int)y >= 0 && y < height)
		value += abs ( ((double)((int)(x)) - x)* ((double)((int)(y+1))-y) ) * Array[(int)(y)*width+(int)(x+1)];
	if ((int)(x+1) >= 0 && (x+1) < width && (int)(y+1) >= 0 && (y+1) < height)
		value += abs ( ((double)((int)(x)) - x)* ((double)((int)(y))-y) ) * Array[(int)(y+1)*width+(int)(x+1)];

	double Norm = abs ( ((double)((int)(x+1)) - x)* ((double)((int)(y+1))-y) ) +
		abs ( ((double)((int)(x+1)) - x) * (y - (double)((int) y)) ) +
		abs ( (x-(double)((int)x)) * ((double)((int)(y+1)) - y) ) +
		abs ( (x-(double)((int)x)) * (y - (double)((int) y)) );

	fprintf (stdout, "X:%lf %lf\n", ((double)((int)(x+1)) - x), (x-(double)((int)x)) );

	if (Norm < 0.001 && Norm > -0.001)
		Norm = 1;

	return value/Norm;
}

static int* cyclicConv (int *Sequence, double spread) {
	
	GTYPE *filter = new GTYPE[2*(int)spread + 1];
	GaussianFilter (0, spread/3, (int) -spread, 2*(int)spread + 1, filter);

	double *convolved = new double[FEATURE_LENGTH];
	for (int n = 0; n < FEATURE_LENGTH; n++) {
		convolved[n] = 0;
		for (int k = -(int)spread; k < spread; k++)
			if ( (n - k) >= 0 )
				convolved[n] += filter[k+(int)spread] * Sequence[(n-k)%FEATURE_LENGTH];
			else
				convolved[n] += filter[k+(int)spread] * Sequence[FEATURE_LENGTH - ((k-n)%FEATURE_LENGTH)];
	}
	delete filter;

	int *iConvolved = new int [FEATURE_LENGTH];
	for (int n = 0; n < FEATURE_LENGTH; n++)
		iConvolved[n] = (int) (0.5+convolved[n]);//rounding off
	delete convolved;

	return iConvolved;
}

int featureCalc(GIMAGE *GaussPix, IMAGE *ExPix, int octave, int blur, char *Project_Folder) 
{
	GTYPE *Magnitudes = new GTYPE[GaussPix->width*GaussPix->height];
	GTYPE maxMag = MIN_PIXEL_VALUE, minMag = MAX_PIXEL_VALUE;
		
	GTYPE tempOri = 0;
	GTYPE *orientations = new GTYPE[GaussPix->width*GaussPix->height];

	for(int n1 = 0; n1 < GaussPix->height-1; n1++) 
	{
		for(int n2 = 0; n2 < GaussPix->width-1; n2++) 
		{
			Magnitudes[n1*GaussPix->width+n2] = 
				sqrt( pow( GTYPE( GaussPix->imageData[n1*GaussPix->width+n2] - GaussPix->imageData[(n1+1)*GaussPix->width+n2] ) , 2 ) 
				+ pow(GTYPE(GaussPix->imageData[n1*GaussPix->width+n2]-GaussPix->imageData[n1*GaussPix->width+n2+1]),2));
			
			if(Magnitudes[n1*GaussPix->width+n2] > maxMag) 
				maxMag = Magnitudes[n1*GaussPix->width+n2];
			
			if(Magnitudes[n1*GaussPix->width+n2] < minMag) 
				minMag = Magnitudes[n1*GaussPix->width+n2];			

			tempOri = atan2(GTYPE(GaussPix->imageData[n1*GaussPix->width+n2]-GaussPix->imageData[(n1+1)*GaussPix->width+n2]), 
				GTYPE(GaussPix->imageData[n1*GaussPix->width+n2]-GaussPix->imageData[n1*GaussPix->width+n2+1]));			

			// aren't the orientations given by atan only distributed between -pi/2 to pi/2
			// How can we say that they occupy the whole 2*pi space
			if (tempOri < 0)
				orientations[n1*GaussPix->width+n2] = tempOri + 2*pi;
			else
				orientations[n1*GaussPix->width+n2] = tempOri;
		}
		Magnitudes[n1*GaussPix->width+GaussPix->width-1] = 0;
	}
	for(int n2 = 0; n2 < GaussPix->width; n2++)
		Magnitudes[(GaussPix->height-1)*GaussPix->width+n2] = 0;
			
	



	int number_of_sectors = FEATURE_LENGTH;

	int window_radius = 15;	
	
	// Sector Mask (Window) Initialization
	int ***sectorWindow = new int**[number_of_sectors];
	for (int sectorIndex = 0; sectorIndex < number_of_sectors; sectorIndex++) 
	{
		sectorWindow[sectorIndex] = new int*[2*window_radius];
		for(int k1 = -window_radius; k1 < window_radius; k1++) 
		{
			sectorWindow[sectorIndex][k1+window_radius] = new int[2*window_radius];
			for(int k2 = -window_radius; k2 < window_radius; k2++)
			{
				sectorWindow[sectorIndex][k1+window_radius][k2+window_radius] = 0;
			}
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

	
	GTYPE gaussFilter[81];
	GaussianFilter2 (0, 1.5*1.5*blur, -4, 9, gaussFilter);

	int numKeys = 0;
	GTYPE threshold = maxMag/10.0;	
	for(int n1 = 0; n1 < ExPix->height-1; n1++)
		for(int n2 = 0; n2 < ExPix->width-1; n2++)
			if(
				ExPix->imageData[n1*ExPix->width+n2] == MAX_PIXEL_VALUE 
				&& Magnitudes[n1*ExPix->width+n2] > threshold 
				)
				numKeys++;

	
	angleKey *AngleDescriptor = new angleKey[(int)(NUMBER_OF_ORIENTATION_BINS_I*numKeys)];
	int keyIndex = 0;
	GTYPE oriBins[(int)NUMBER_OF_ORIENTATION_BINS_I];

	GTYPE Magnif = 1.0;//1.0 / pow(2.0,octave);
	GTYPE stdev = 1.5 * Magnif;
	int sectorOffset = 0;
	int totalNeighbors = 0;
	int *tempPtr = 0;
	for(int n1 = 0; n1 < (ExPix->height)-1; n1++) 
	{
		for(int n2 = 0; n2 < (ExPix->width)-1; n2++) 
		{
			
			if(ExPix->imageData[n1*ExPix->width+n2] == MAX_PIXEL_VALUE  
				&& Magnitudes[n1*ExPix->width+n2] > threshold 
				) 
			{
				for(int i = 0; i < NUMBER_OF_ORIENTATION_BINS_I; i++) 
						oriBins[i] = 0;
					
				
					for(int k1=-3; k1 < 4; k1++)
						for(int k2=-3; k2 < 4; k2++)
							if((n1+k1*Magnif) > 0 && (n1+k1*Magnif) < ExPix->height && (n2+k2*Magnif) > 0 && (n2+k2*Magnif) < ExPix->width) {

								oriBins[(int) (orientations[(n1+k1)*ExPix->width +(n2+k2)]/(2.0*pi) * NUMBER_OF_ORIENTATION_BINS_I)] 
										+= Magnitudes[(n1+k1)*ExPix->width +(n2+k2)]
											* exp(-pow((GTYPE)k1*Magnif,2)/(2.0*pow(stdev,2)))/(stdev*sqrt(2.0*pi))
												* exp(-pow((GTYPE)k2*Magnif,2)/(2.0*pow(stdev,2)))/(stdev*sqrt(2.0*pi));
								
								
								/*
								oriBins[(int) ((bilinearInterpolate ((n1+k1*Magnif),(n2+k2*Magnif),orientations,ExPix->width,ExPix->height)/(2.0*pi) + 0.5) * NUMBER_OF_ORIENTATION_BINS_I)] 
										+= bilinearInterpolate ((n1+k1*Magnif),(n2+k2*Magnif),Magnitudes,ExPix->width,ExPix->height)
											* exp(-pow((GTYPE)k1*Magnif,2)/(2.0*pow(stdev,2)))/(stdev*sqrt(2.0*pi))
												* exp(-pow((GTYPE)k2*Magnif,2)/(2.0*pow(stdev,2)))/(stdev*sqrt(2.0*pi));
								*/
							}

					double maxBin = 0;
					for(int i=0; i < NUMBER_OF_ORIENTATION_BINS_I; i++)
						if(maxBin < oriBins[i])
							maxBin = oriBins[i];
					
					int numMax = 0;
					AngleDescriptor[keyIndex].KeyOrientation = 0;
					for(int i=0; i < NUMBER_OF_ORIENTATION_BINS_I; i++) 
						if(oriBins[i] > PEAK_THRESHOLD * maxBin)
						{
							AngleDescriptor[keyIndex+numMax].KeyOrientation = (((double) i) / NUMBER_OF_ORIENTATION_BINS_I)*(2*pi);
							numMax++;						
						}

				
				for ( int subIndex = 0; subIndex < numMax; subIndex++ ) 
				{
					AngleDescriptor[keyIndex].y = (double)n1 * pow(2.0,octave-1);//before without pow
					AngleDescriptor[keyIndex].x = (double)n2 * pow(2.0,octave-1);//before without pow
					AngleDescriptor[keyIndex].scale = 1.0 / pow(2.0,octave);//Magnif;//0.121212;//
					
					// Check whether I have to change the offset to KeyOrientation as -pi/2 < atanX < pi/2
					sectorOffset = (int) ((AngleDescriptor[keyIndex].KeyOrientation)* number_of_sectors / (2.0*pi));
					//sectorOffset = (int) ((AngleDescriptor[keyIndex].KeyOrientation + pi)* number_of_sectors / (2.0*pi));

					int totalNeighbors = 0;
					for (int sectorIndex = 0 ; sectorIndex < number_of_sectors; sectorIndex++)
					{
						AngleDescriptor[keyIndex].sectorCount[sectorIndex] = 0;
						for (int k1 = -window_radius; k1 < window_radius; k1++)
							for (int k2 = -window_radius; k2 < window_radius; k2++)
							// I should be able to add single side edges
								if ( n1 + k1 < (ExPix->height - 1) && n1 + k1 >= 0)
									if ( n2 + k2 < (ExPix->width - 1) && n2 + k2 >= 0) 
									{
										// Number of stable points in each sector. Sector is taken relative to the key orientation
										AngleDescriptor[keyIndex].sectorCount[sectorIndex] 
											+= ExPix->imageData[(n1+k1)*ExPix->width+(n2+k2)] 
												* sectorWindow[(sectorIndex + sectorOffset) % number_of_sectors][k1+window_radius][k2+window_radius];
									}
						//AngleDescriptor[keyIndex].sectorCount[sectorIndex] /= MAX_PIXEL_VALUE;						
						totalNeighbors += AngleDescriptor[keyIndex].sectorCount[sectorIndex];

						// Polynomial Separation
						//AngleDescriptor[keyIndex].sectorCount[sectorIndex] *= AngleDescriptor[keyIndex].sectorCount[sectorIndex] + 1;
/*						
						// Noise Reduction
						tempPtr = cyclicConv (AngleDescriptor[keyIndex].sectorCount,3.0);
						for (int i = 0; i < FEATURE_LENGTH; i++)
							AngleDescriptor[keyIndex].sectorCount[i] = tempPtr[i];
						delete tempPtr;
*/					}				
					if (totalNeighbors > 10*MAX_PIXEL_VALUE) 
					{
						//cout << totalNeighbors << "\t";
						keyIndex++;
					}
				}				
			}
		}
	}
	//cout << "\t\tStable Keypoints: " << numKeys << endl;
	//cout << "\t\tKeys with Orientation = " << keyIndex << endl;

	// Garbage Collection
	for (int sectorIndex = 0; sectorIndex < number_of_sectors; sectorIndex++) {
		for(int k1 = -window_radius; k1 < window_radius; k1++)
			delete sectorWindow[sectorIndex][k1+window_radius];
		delete sectorWindow[sectorIndex];
	}
	delete sectorWindow;


	char filename[MAX_FILE_NAME_LENGTH];	
/*
	sprintf(filename,"%s\\04.Gradient_Pyramid\\%d%d.bin",Project_Folder,octave,blur);
	FILE *magFile = fopen(filename,"wb");
	fwrite(&maxMag,sizeof(GTYPE),1,magFile);
	fwrite(&minMag,sizeof(GTYPE),1,magFile);
	fwrite(Magnitudes,sizeof(GTYPE),GaussPix->width*GaussPix->height,magFile);
	//cout << ". MaxMag = " << maxMag << " , MinMag = " << minMag << endl;
	fclose(magFile);		
*/	delete Magnitudes;

/*
	sprintf(filename,"%s\\05.Orientation_Pyramid\\%d%d.bin",Project_Folder,octave,blur);
	FILE *oriFile = fopen(filename,"wb");
	fwrite(orientations,sizeof(GTYPE),GaussPix->width*GaussPix->height,oriFile);
	fclose(oriFile);	
*/	delete orientations;


	sprintf(filename,"%s\\07.Feature_Keys\\%d%d.bin",Project_Folder,octave,blur);
	FILE *keyFile = fopen(filename,"wb");
	fwrite(&keyIndex,sizeof(int),1,keyFile);
	fwrite(AngleDescriptor,sizeof(angleKey),keyIndex,keyFile);
	fclose(keyFile);
	delete AngleDescriptor;

	return numKeys;
}


void writeAllFeatures (GTYPE sigma, int numOctaves, int numBlurs, char *PROJECT_FOLDER, int frameIndex) 
{
	char filename[MAX_FILE_NAME_LENGTH];

	if (frameIndex == -1) sprintf (filename,"%s/07.Feature_Keys/train.bin",PROJECT_FOLDER);
	else sprintf (filename,"%s/07.Feature_Keys/testFeature(%d).bin",PROJECT_FOLDER,frameIndex);
	FILE *AllKeyFile = fopen (filename,"wb");

	int featurelength = FEATURE_LENGTH;
	fwrite(&featurelength,sizeof(int),1,AllKeyFile);
	fseek(AllKeyFile,sizeof(int),SEEK_CUR); // Allocated space to write Total Number of features

	int numKeys,totalKeys = 0;
	for (int i = 0; i < numOctaves; i++)
		for (int j = 1; j < numBlurs-2; j++) {
			sprintf(filename,"%s/07.Feature_Keys/%d%d.bin",PROJECT_FOLDER,i,j);
			FILE *keyFile = fopen(filename,"rb");
			fread(&numKeys,sizeof(int),1,keyFile);
			angleKey *AngleDescriptor = new angleKey[numKeys];
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
