#include <DSP/Pyramids.h>
#include <CriticalPoints/Extreme.h>

#include <iostream>
using namespace std;

// A pixel's Neighbouring pixels in same scale
#define NUMBER_OF_NEIGHBORS 8
// A pixel's Neighbouring pixels in different scale
#define DIFF_SCALE_NEIGHBORS 9

 int findExtrema (GIMAGE *inImage, GIMAGE *lowImage, GIMAGE *highImage, IMAGE *extremeImage) {
	int numExtreme = 0;

	for (int n1 = 0; n1 < extremeImage->height; n1++)
		for (int n2 = 0; n2 < extremeImage->width; n2++)
			extremeImage->imageData[n1*inImage->width + n2] = 0;

	for (int n1 = 1; n1 < inImage->height - 1 ; n1++)
		for (int n2 = 1; n2 < inImage->width - 1; n2++) {

			int numHighNeigbors = 0;
			for (int x = n1-1; x <= n1+1; x++)
				for (int y = n2-1; y <= n2+1; y++) {
					if (x == n1 && y == n2) continue; // the pixel doesn't have to compare with itself
					if (inImage->imageData[n1*inImage->width+n2] > inImage->imageData[x*inImage->width+y])	numHighNeigbors++;
					else if (inImage->imageData[n1*inImage->width+n2] < inImage->imageData[x*inImage->width+y]) numHighNeigbors--;
				}
			
			if (!(numHighNeigbors % NUMBER_OF_NEIGHBORS)) {
			
				int numHighNeighbors_lowScale = 0;
				for (int x = n1-1; x <= n1+1; x++)
					for (int y = n2-1; y <= n2+1; y++) {						
							if (inImage->imageData[n1*inImage->width+n2] > lowImage->imageData[x*lowImage->width+y]) numHighNeighbors_lowScale++;
							else if (inImage->imageData[n1*inImage->width+n2] < lowImage->imageData[x*lowImage->width+y]) numHighNeighbors_lowScale--;						
					}

				if (!(numHighNeighbors_lowScale % DIFF_SCALE_NEIGHBORS)) {
					
					int numHighNeighbors_highScale = 0;
					for (int x = n1-1; x <= n1+1; x++)
						for (int y = n2-1; y <= n2+1; y++) {							
								if (inImage->imageData[n1*inImage->width+n2] > highImage->imageData[x*highImage->width+y])	numHighNeighbors_highScale++;
								else if (inImage->imageData[n1*inImage->width+n2] < highImage->imageData[x*highImage->width+y]) numHighNeighbors_highScale--;
						}
			
					if (numHighNeigbors == NUMBER_OF_NEIGHBORS && numHighNeighbors_lowScale == DIFF_SCALE_NEIGHBORS && numHighNeighbors_highScale == DIFF_SCALE_NEIGHBORS){
							extremeImage->imageData[n1*extremeImage->width + n2] = 255;
							numExtreme++;
					}
					else if (numHighNeigbors == -NUMBER_OF_NEIGHBORS && numHighNeighbors_lowScale == -DIFF_SCALE_NEIGHBORS && numHighNeighbors_highScale == -DIFF_SCALE_NEIGHBORS){
							extremeImage->imageData[n1*extremeImage->width + n2] = 255;
							numExtreme++;
					}
					
				}
			}
		}


		/* Edge Response Removal */


		cout << "\t\tNumber of Extrema Points:" << numExtreme << endl;
		return numExtreme;
}