#include <graphics/triangles.h>
#include <string.h>

void sort_vertices (int num_vertices, int *sorted_x, int *sorted_y)
{
	int temp_x,temp_y;
	int min_x,min_xy;
	int min_vertex;
	for (int vertex = 1; vertex < num_vertices; vertex++)
	{
		min_x = sorted_x[vertex-1], min_xy = sorted_y[vertex-1];
		min_vertex = vertex-1;
		
		for (int sub_vertex = vertex; sub_vertex < num_vertices; sub_vertex++)
		{
			if (sorted_x[sub_vertex] < min_x)
			{
				min_x = sorted_x[sub_vertex];
				min_xy = sorted_y[sub_vertex];
				min_vertex = sub_vertex;
			}
			else if ( sorted_x[sub_vertex] == min_x)
			{
				if (sorted_y[sub_vertex] < min_xy)
				{
					min_x = sorted_x[sub_vertex];
					min_xy = sorted_y[sub_vertex];
					min_vertex = sub_vertex;
				}				
			}
		}

		if (vertex-1 != min_vertex)
		{
			temp_x = sorted_x[vertex -1], temp_y = sorted_y[vertex -1];
			sorted_x[vertex -1] = sorted_x[min_vertex], sorted_y[vertex-1] = sorted_y[min_vertex];
			sorted_x[min_vertex] = temp_x, sorted_y[min_vertex] = temp_y;
		}

		//cout << sorted_x[vertex-1] << " , " << sorted_y[vertex-1] << endl;
	}
	//cout << sorted_x[num_vertices-1] << " , " << sorted_y[num_vertices-1] << endl;

}

IMAGE* drawPolygon (int num_vertices, int *x, int *y, IMAGE *inImage, double *color_contrast, int *color_brightness)
{
	int *sorted_x = new int[num_vertices], *sorted_y = new int[num_vertices];
	memcpy (sorted_x, x, sizeof(int)*num_vertices);
	memcpy (sorted_y, y, sizeof(int)*num_vertices);

	sort_vertices (num_vertices, sorted_x, sorted_y);

	IMAGE* outImage = cloneImage (inImage);

	int temp_x[3];
	int temp_y[3];
		
	for (int triangleIndex = 0; triangleIndex < num_vertices-2; triangleIndex++)
	{
		/* This draws only regular polygons */
		temp_x[0] = sorted_x[triangleIndex + 0], temp_y[0] = sorted_y[triangleIndex + 0];
		temp_x[1] = sorted_x[triangleIndex + 1], temp_y[1] = sorted_y[triangleIndex + 1];
		temp_x[2] = sorted_x[triangleIndex + 2], temp_y[2] = sorted_y[triangleIndex + 2];

		drawTriangle (temp_x, temp_y, outImage, color_contrast, color_brightness);
	}

	return outImage;
}

IMAGE* extractPolygonRegion (int num_vertices, int *x, int *y, IMAGE *inImage)
{
	double color_contrast[3] = {0};
	int color_brightness[3] = {0};

	IMAGE* tempImage = drawPolygon (num_vertices, x, y, inImage, color_contrast, color_brightness);

	for (int row = 0; row < inImage->height; row++)
	{
		for (int col = 0; col < inImage->width; col++)
		{
			for (int c = 0; c < inImage->numColors; c++)
			{
				tempImage->imageData [ (row * tempImage->width + col)*tempImage->numColors + c ] 
				= inImage->imageData [ (row * tempImage->width + col)*inImage->numColors + c ] 
				- tempImage->imageData [ (row * tempImage->width + col)*tempImage->numColors + c ] ;
			}
		}
	}

	return tempImage;
}