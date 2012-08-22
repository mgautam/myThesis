#include <bmpAccess/bmpEngine.h>

#include <graphics/lines.h>
#include <graphics/triangles.h>
#include <graphics/polygons.h>

#include <Matrix/matrix.h>

#include <AffineTransformer/bounding_boxes.h>

#include <stdlib.h>
#include <math.h>
#include <iostream>
using namespace std;

#define NUM_VERTICES 4

void testBlocks (void) {
	IMAGE *inImage = readRGB ("TestRepo/lena.bmp");

	int x[NUM_VERTICES] = {126,126,386,386};//{226,236,276,286,286,276,236,226};//{126,146,386,366};//
	int y[NUM_VERTICES] = {126,386,126,386};//{276,286,286,276,236,226,226,236};//{366,386,146,126};//
	
	IMAGE *outImage = extract_selection (NUM_VERTICES, x, y, inImage);
	
	char filename[100];
	sprintf (filename,"TestRepo/01.Train/subject.bmp");
	cout << filename << endl;
	writeImage (filename,outImage);
	releaseImage (outImage);

	MATRIX Polygon(NUM_VERTICES,3); // [x;y;1];	
	MATRIX_CONTENT_TYPE **PolygonData = Polygon.getDataPtr();
	for (int col = 0; col < NUM_VERTICES; col++)
	{
		PolygonData[col][0] = x[col] - inImage->width / 2;
		PolygonData[col][1] = y[col] - inImage->height / 2;
		PolygonData[col][2] = 1;
	}
	//cout << Polygon << endl << endl;


	MATRIX AffineModel(3,2);
	MATRIX_CONTENT_TYPE **AffineData = AffineModel.getDataPtr();
		
	double color_contrast[3] = {0.75,0.75,0.75};
	int color_brightness[3] = {-50,-50,-50};

	double theta;
	for (int i = 0; i < 100; i++)
	{
		theta = (double) i * 2 * 3.14 / 100.0;
		AffineData[0][0] = cos (theta) , AffineData[1][0] = sin (theta) , AffineData[2][0] = 0;//- 50 + i;
		AffineData[0][1] = - sin (theta) , AffineData[1][1] = cos (theta) , AffineData[2][1] = 0;//- 50 + i;
		//cout << AffineModel << endl << endl;

		outImage = render_selection ( &Polygon, &AffineModel, inImage, color_contrast, color_brightness);
		
		sprintf (filename,"TestRepo/blocked/%d.bmp",i);
		cout << filename << endl;
		writeImage (filename,outImage);

		releaseImage (outImage);
	}
	releaseImage (inImage);

}