#include <stdio.h>
#include <stdlib.h>

extern "C" {
#include <vl/generic.h>
#include <vl/sift.h>
}

#include <bmpAccess/bmpEngine.h>
#include <Features/featureStruct.h>

#define numOctaves 4
#define numBlurs 5


static FEATURES* aggregator (int *nkeys_in_octave, const VlSiftKeypoint **keypoints, int **numMaxOrientations, vl_sift_pix ****descriptor);
static void showStats (FEATURES *featureHeap);

FEATURES BuildFeature (char *imFileName) {
	
	GIMAGE *inImage;
	inImage = Gtype (readGrey(imFileName));
		
	VlSiftFilt* siftFilter = vl_sift_new (inImage->width, inImage->height, numOctaves, numBlurs, 0);

	const VlSiftKeypoint *keypoints[numOctaves];
	double ***angles = new double **[numOctaves];	int **numMaxOrientations = new int*[numOctaves];;


	vl_sift_pix ****descriptor;//should add one more ptr reference for number of points
	
	descriptor = new vl_sift_pix***[numOctaves];
	int nkeys_in_octave[numOctaves];
	if (vl_sift_process_first_octave ( siftFilter, (const vl_sift_pix *) inImage->imageData ) !=  VL_ERR_EOF ) {
	
		vl_sift_detect (siftFilter); // How do you get number of critical points?
		keypoints[0] =  vl_sift_get_keypoints ( siftFilter );
		nkeys_in_octave[0] = siftFilter->nkeys;
		descriptor[0] = new vl_sift_pix**[siftFilter->nkeys];
		angles[0] = new double*[siftFilter->nkeys];
		numMaxOrientations[0] = new int[siftFilter->nkeys];
		for (int keyIndex = 0; keyIndex < siftFilter->nkeys; keyIndex++) {
			angles[0][keyIndex] = new double[4];
			numMaxOrientations[0][keyIndex] = vl_sift_calc_keypoint_orientations ( siftFilter, angles[0][keyIndex], &keypoints[0][keyIndex] );
			descriptor[0][keyIndex] = new vl_sift_pix*[numMaxOrientations[0][keyIndex]];
			for (int angleIndex = 0; angleIndex < numMaxOrientations[0][keyIndex]; angleIndex++) {
				descriptor[0][keyIndex][angleIndex] = new vl_sift_pix[128];
				vl_sift_calc_keypoint_descriptor ( siftFilter, descriptor[0][keyIndex][angleIndex], &keypoints[0][keyIndex], angles[0][keyIndex][angleIndex] ); // How to set custom number of orientations?
			}
		}

		//int octave = 0;
		//while (vl_sift_process_next_octave ( siftFilter ) !=  VL_ERR_EOF ) { octave++;
		for ( int octave = 1; octave < numOctaves; octave++) {
			vl_sift_detect (siftFilter); // How do you get number of critical points?
			keypoints[octave] =  vl_sift_get_keypoints ( siftFilter );
			nkeys_in_octave[octave] = siftFilter->nkeys;
			descriptor[octave] = new vl_sift_pix**[siftFilter->nkeys];
			angles[octave] = new double*[siftFilter->nkeys];
			numMaxOrientations[octave] = new int[siftFilter->nkeys];
			for (int keyIndex = 0; keyIndex < siftFilter->nkeys; keyIndex++) {
				angles[octave][keyIndex] = new double[4];
				numMaxOrientations[octave][keyIndex] = vl_sift_calc_keypoint_orientations ( siftFilter, angles[octave][keyIndex], &keypoints[octave][keyIndex] );
				descriptor[octave][keyIndex] = new vl_sift_pix*[numMaxOrientations[octave][keyIndex]];
				for (int angleIndex = 0; angleIndex < numMaxOrientations[octave][keyIndex]; angleIndex++) {
					descriptor[octave][keyIndex][angleIndex] = new vl_sift_pix[128];
					vl_sift_calc_keypoint_descriptor ( siftFilter, descriptor[octave][keyIndex][angleIndex], &keypoints[octave][keyIndex], angles[octave][keyIndex][angleIndex] ); // How to set custom number of orientations?
				}
			}
		}


	}

	FEATURES* FeatureHeap = aggregator (nkeys_in_octave, keypoints, numMaxOrientations, descriptor);
	
	vl_sift_delete (siftFilter);
	releaseImage (inImage);


	//showStats ( FeatureHeap );

	return *FeatureHeap;
}

FEATURES* aggregator (int *nkeys_in_octave, const VlSiftKeypoint **keypoints, int **numMaxOrientations, vl_sift_pix ****descriptor) {
	int totalkeys = 0;
	for ( int octave = 0; octave < numOctaves; octave++ )
		for (int keyIndex = 0; keyIndex < nkeys_in_octave[octave]; keyIndex++)
			for (int angleIndex = 0; angleIndex < numMaxOrientations[0][keyIndex]; angleIndex++)
				totalkeys ++;
	printf ("TotalKeys = %d\n",totalkeys);
			
	
	FEATURES *featureheap = new FEATURES;
	
	featureheap->FeatureVectorLength = 128;
	featureheap->Number_of_Features = totalkeys;
	featureheap->features = new FEATURE[featureheap->Number_of_Features];
			
	int featureIndex = 0;
	for ( int octave = 0; octave < numOctaves; octave++ ) {	
		for (int keyIndex = 0; keyIndex < nkeys_in_octave[octave]; keyIndex++) {
			for (int angleIndex = 0; angleIndex < numMaxOrientations[0][keyIndex]; angleIndex++) {
				featureheap->features[featureIndex].x = keypoints[octave][keyIndex].ix;//.x;
				featureheap->features[featureIndex].y = keypoints[octave][keyIndex].iy;//.y;
				featureheap->features[featureIndex].FeatureVector = new float [featureheap->FeatureVectorLength];
				for (int i = 0; i < featureheap->FeatureVectorLength; i++)
					featureheap->features[featureIndex].FeatureVector[i] = descriptor[octave][keyIndex][angleIndex][i];
				featureIndex++;
			}
		}
	}

	return featureheap;
}

void showStats (FEATURES *featureHeap) {
	for (int featureIndex = 0; featureIndex < featureHeap->Number_of_Features; featureIndex++) {
		printf ("%lf->%lf\n",featureHeap->features[featureIndex].x,featureHeap->features[featureIndex].y);
	}	
}

// Build a destructor for the frames