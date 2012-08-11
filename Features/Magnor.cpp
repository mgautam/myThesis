#include <stdio.h>
#include <stdlib.h>
#include<math.h>

#include <string.h>
#include <bmpAccess/bmpEngine.h>

#include <DSP/filter.h>
#include <Features/Magnor.h>

#define MAX_FILE_NAME_LENGTH 100

#define MIN_PIXEL_VALUE 0
#define MAX_PIXEL_VALUE 255

#define NUMBER_OF_ORIENTATION_BINS_I 36.0
#define NUMBER_OF_ORIENTATION_BINS_II 8.0

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

	//cout << "X:" << ((double)((int)(x+1)) - x) << " " << (x-(double)((int)x)) << endl;

	if (Norm < 0.001 && Norm > -0.001)
		Norm = 1;

	return value/Norm;
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
				sqrt(pow(GTYPE(GaussPix->imageData[n1*GaussPix->width+n2]-GaussPix->imageData[(n1+1)*GaussPix->width+n2]),2) 
					+pow(GTYPE(GaussPix->imageData[n1*GaussPix->width+n2]-GaussPix->imageData[n1*GaussPix->width+n2+1]),2));
			if(Magnitudes[n1*GaussPix->width+n2] > maxMag) maxMag = Magnitudes[n1*GaussPix->width+n2];
			if(Magnitudes[n1*GaussPix->width+n2] < minMag) minMag = Magnitudes[n1*GaussPix->width+n2];			

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
	




	GTYPE gaussFilter[81];
	GaussianFilter2 (0, 1.5*1.5*blur, -4, 9, gaussFilter);
	
	int numKeys = 0;
	GTYPE threshold = maxMag/10.0;	
	for(int n1 = 0; n1 < ExPix->height-1; n1++)
		for(int n2 = 0; n2 < ExPix->width-1; n2++)
			if(ExPix->imageData[n1*ExPix->width+n2] == MAX_PIXEL_VALUE && Magnitudes[n1*ExPix->width+n2] > threshold )
				numKeys++;

	siftKey *keyDescriptors = new siftKey[(int)(NUMBER_OF_ORIENTATION_BINS_I*numKeys)];
	int keyIndex = 0;
	GTYPE oriBins[(int)NUMBER_OF_ORIENTATION_BINS_I];
	
	GTYPE Norm;
	GTYPE Magnif = 1.0;//1.0 / pow(2.0,octave);
	GTYPE stdev = 1.5 * Magnif;

	for(int n1 = 0; n1 < (ExPix->height)-1; n1++) 
	{
		for(int n2 = 0; n2 < (ExPix->width)-1; n2++) 
		{			
			if(ExPix->imageData[n1*ExPix->width+n2] == MAX_PIXEL_VALUE && Magnitudes[n1*ExPix->width+n2] > threshold ) 
			{

				for(int i=0; i < NUMBER_OF_ORIENTATION_BINS_I; i++) 
					oriBins[i] = 0;
				
				for(int k1=-3.5; k1 < 4; k1++) //float
					for(int k2=-3.5; k2 < 4; k2++) //float
					{
						if((n1+k1*Magnif) > 0 && (n1+k1*Magnif) < ExPix->height && (n2+k2*Magnif) > 0 && (n2+k2*Magnif) < ExPix->width) 
						{
							
							oriBins[(int) (orientations[(n1+k1)*ExPix->width +(n2+k2)]/(2.0*pi) * NUMBER_OF_ORIENTATION_BINS_I)] 
									+= Magnitudes[(n1+k1)*ExPix->width +(n2+k2)]
										* exp(-pow((GTYPE)k1,2)/(2.0*pow(stdev,2)))/(stdev*sqrt(2.0*pi))
											* exp(-pow((GTYPE)k2,2)/(2.0*pow(stdev,2)))/(stdev*sqrt(2.0*pi));
								
								
							/*
							oriBins[(int) ((bilinearInterpolate ((n1+k1*Magnif),(n2+k2*Magnif),orientations,ExPix->width,ExPix->height)/(2.0*pi) + 0.5) * NUMBER_OF_ORIENTATION_BINS_I)] 
									+= bilinearInterpolate ((n1+k1*Magnif),(n2+k2*Magnif),Magnitudes,ExPix->width,ExPix->height)
										* exp(-pow((GTYPE)k1*Magnif,2)/(2.0*pow(stdev,2)))/(stdev*sqrt(2.0*pi))
											* exp(-pow((GTYPE)k2*Magnif,2)/(2.0*pow(stdev,2)))/(stdev*sqrt(2.0*pi));
							*/
						}						
					}
				
				

				double maxBin = 0;
				for(int i=0; i < NUMBER_OF_ORIENTATION_BINS_I; i++)
					if(maxBin < oriBins[i])
						maxBin = oriBins[i];
				
				int numMax = 0;
				keyDescriptors[keyIndex].KeyOrientation = 0;
				for(int i=0; i < NUMBER_OF_ORIENTATION_BINS_I; i++)
					if(oriBins[i] > PEAK_THRESHOLD * maxBin)
					{
						keyDescriptors[keyIndex+numMax].KeyOrientation = (((double) i) / NUMBER_OF_ORIENTATION_BINS_I)*(2*pi);
						numMax++;						
					}
			


				for ( int subIndex = 0; subIndex < numMax; subIndex++ ) 
				{
					keyDescriptors[keyIndex].y = (double)n1 * pow(2.0,octave-1);//before without pow
					keyDescriptors[keyIndex].x = (double)n2 * pow(2.0,octave-1);//before without pow
					keyDescriptors[keyIndex].scale = Magnif;

					// SubIndex based on number of orientations above 80% of Maximum
					double xi,yi;
					double cos0 = cos (keyDescriptors[keyIndex].KeyOrientation);
					double sin0 = sin (keyDescriptors[keyIndex].KeyOrientation);
					for(float m1=-1.5; m1 < 2; m1++)
						for(float m2=-1.5; m2 < 2; m2++) 
						{
							// Enumerating Neighbouring Blocks around Keypoint
							
							for(int i=0; i < NUMBER_OF_ORIENTATION_BINS_II; i++) 
								keyDescriptors[keyIndex].Descriptor[(int)(((m1+1.5)*4+(m2+1.5))*NUMBER_OF_ORIENTATION_BINS_II+i)] = 0;
													
							for(float k1=-1.5; k1 < 2; k1++)
								for(float k2=-1.5; k2 < 2; k2++) 
								{
									// Enumerating pixels in each block

									// Rotation or Key Orientation about the keypoint
									xi = ((m1*4+k1) *  cos0 + (m2*4+k2) * sin0)*Magnif;
									yi = ((m1*4+k1) * -sin0 + (m2*4+k2) * cos0)*Magnif;


									if((int)((float)n1+xi) >= 0 && ((float)n1+xi) < ExPix->height && (int)((float)n2+yi) >= 0 && ((float)n2+yi) < ExPix->width)
										keyDescriptors[keyIndex].Descriptor[(int)(((m1+1.5)*4+(m2+1.5))*NUMBER_OF_ORIENTATION_BINS_II+
										(int)((bilinearInterpolate (((float)n1+xi),((float)n2+yi),orientations,ExPix->width,ExPix->height))*NUMBER_OF_ORIENTATION_BINS_II/(2*pi)))] 
											+= bilinearInterpolate (((float)n1+xi),((float)n2+yi),Magnitudes,ExPix->width,ExPix->height);//(maxMag-minMag)//*gaussFilter[(k1+3)*7+(k2+3)];
											// Use temporary variables to accomodate good double to int conversion i.e. only in final stage
								}
						}

					Norm = 0;
					for(int i = 0; i < FEATURE_LENGTH; i++)
						Norm += pow(keyDescriptors[keyIndex].Descriptor[i],2);
					Norm = sqrt(Norm);
					if( Norm != 0 ) {
						for(int i=0; i < FEATURE_LENGTH; i++)
							keyDescriptors[keyIndex].Descriptor[i] /= Norm;					
					}
					/*
					//Clip to 0.2					
					for(int i = 0; i < FEATURE_LENGTH; i++)
						if(keyDescriptors[keyIndex].Descriptor[i] > 0.2) 
							keyDescriptors[keyIndex].Descriptor[i] = 0.2;

					// Renormalize
					Norm = 0;
					for(int i = 0; i < FEATURE_LENGTH; i++)
						Norm += pow(keyDescriptors[keyIndex].Descriptor[i],2);
					Norm = sqrt(Norm);
					for(int i=0; i < FEATURE_LENGTH; i++) {
						keyDescriptors[keyIndex].Descriptor[i] /= Norm;
						if(keyDescriptors[keyIndex].Descriptor[i] > 1 || keyDescriptors[keyIndex].Descriptor[i]  < 0)
							keyDescriptors[keyIndex].Descriptor[i] = 0;
					}
					
					*/	

					keyDescriptors[keyIndex].y = (double)n1 * pow(2.0,octave-1);//before without pow
					keyDescriptors[keyIndex].x = (double)n2 * pow(2.0,octave-1);//before without pow
					keyDescriptors[keyIndex].scale = 0.121212;

					keyIndex++;
				}
			}
		}		
	}
	//cout << "\t\tStable Keypoints: " << numKeys << endl;
	//cout << "\t\tKeys with Orientation = " << keyIndex << endl;

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
	fwrite(keyDescriptors,sizeof(siftKey),keyIndex,keyFile);
	fclose(keyFile);
	delete keyDescriptors;

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
			siftKey *siftDescriptor = new siftKey[numKeys];
			fread(siftDescriptor,sizeof(siftKey),numKeys,keyFile);
			fclose(keyFile);

			fwrite(siftDescriptor,sizeof(siftKey),numKeys,AllKeyFile);
			totalKeys += numKeys;
			//cout << numKeys << endl;
			delete siftDescriptor;
		}

	fseek(AllKeyFile,sizeof (int),SEEK_SET); // Skip over previous written Feature Length =128
	fwrite(&totalKeys,sizeof(int),1,AllKeyFile);
	fclose(AllKeyFile);	
}