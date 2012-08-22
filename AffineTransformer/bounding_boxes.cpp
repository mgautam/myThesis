#include <bmpAccess/bmpEngine.h>
#include <AffineTransformer/bounding_boxes.h>
#include <graphics/polygons.h>

//#include <iostream>
//using namespace std;

extern int TrainObject_x;
extern int TrainObject_y;

IMAGE* render_selection (MATRIX *selection_box, MATRIX *AffineModel, IMAGE *inImage, double *color_contrast, int *color_brightness)
{
	int num_vertices = selection_box->getWidth();

	MATRIX Transformed(num_vertices,AffineModel->getHeight());
	Transformed = (*AffineModel) * (*selection_box);

	//cout << Transformed << endl;

	MATRIX_CONTENT_TYPE **TransformedData = Transformed.getDataPtr();
	
	int *x = new int[num_vertices], *y = new int[num_vertices];
	for (int col = 0; col < num_vertices; col++)
	{
		x[col] = TransformedData[col][0] + TrainObject_x;
		y[col] = TransformedData[col][1] + TrainObject_y;
	}
	
	IMAGE* outImage = drawPolygon (num_vertices,x,y,inImage, color_contrast, color_brightness);

	return outImage;
}

IMAGE* extract_selection (int num_vertices, int *x, int *y, IMAGE *inImage)
{
	return extractPolygonRegion (num_vertices, x, y, inImage);
}