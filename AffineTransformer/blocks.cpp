#include <bmpAccess/bmpEngine.h>
#include <AffineTransformer/blocks.h>
#include <graphics/polygons.h>

#include <iostream>
using namespace std;

IMAGE* render_selection (int num_vertices, MATRIX *selection_box, MATRIX *AffineModel, IMAGE *inImage, double *color_contrast, int *color_brightness)
{

	MATRIX Transformed(num_vertices,3);
	Transformed = (*AffineModel) * (*selection_box);
	MATRIX_CONTENT_TYPE **TransformedData = Transformed.getDataPtr();
	//cout << Transformed << endl;

	int *x = new int[num_vertices], *y = new int[num_vertices];
	for (int col = 0; col < num_vertices; col++)
	{
		x[col] = TransformedData[col][0] + inImage->width / 2;
		y[col] = TransformedData[col][1] + inImage->height / 2;			
	}
	
	IMAGE* outImage = drawPolygon (num_vertices,x,y,inImage, color_contrast, color_brightness);

	return outImage;
}

IMAGE* extract_selection (int num_vertices, int *x, int *y, IMAGE *inImage)
{
	return extractPolygonRegion (num_vertices, x, y, inImage);
}