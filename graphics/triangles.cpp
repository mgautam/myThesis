#include <graphics/triangles.h>

#include <iostream>
using namespace std;

// Change these to get color and Texture embedding
#define DEFAULT_CONTRAST 0.5
#define DEFAULT_BRIGHTNESS 0

void nonRightTriangle (int *x, int *y, IMAGE *tempImage, double *color_contrast, int *color_brightness)
{
	double sorted_x[3], sorted_y[3];
	if ( x[0] <= x[1] && x[1] <= x[2])
	{
		sorted_x[0] = x[0], sorted_y[0] = y[0];
		sorted_x[1] = x[1], sorted_y[1] = y[1];
		sorted_x[2] = x[2], sorted_y[2] = y[2];
	}
	else if ( x[0] <= x[2] && x[2] <= x[1])
	{
		sorted_x[0] = x[0], sorted_y[0] = y[0];
		sorted_x[1] = x[2], sorted_y[1] = y[2];
		sorted_x[2] = x[1], sorted_y[2] = y[1];
	}
	else if ( x[1] <= x[0] && x[0] <= x[2])
	{
		sorted_x[0] = x[1], sorted_y[0] = y[1];
		sorted_x[1] = x[0], sorted_y[1] = y[0];
		sorted_x[2] = x[2], sorted_y[2] = y[2];
	}
	else if ( x[1] <= x[2] && x[2] <= x[0])
	{
		sorted_x[0] = x[1], sorted_y[0] = y[1];
		sorted_x[1] = x[2], sorted_y[1] = y[2];
		sorted_x[2] = x[0], sorted_y[2] = y[0];
	}
	else if ( x[2] <= x[1] && x[1] <= x[0])
	{
		sorted_x[0] = x[2], sorted_y[0] = y[2];
		sorted_x[1] = x[1], sorted_y[1] = y[1];
		sorted_x[2] = x[0], sorted_y[2] = y[0];
	}
	else //if ( x[2] <= x[0] && x[0] <= x[1])
	{
		sorted_x[0] = x[2], sorted_y[0] = y[2];
		sorted_x[1] = x[0], sorted_y[1] = y[0];
		sorted_x[2] = x[1], sorted_y[2] = y[1];
	}

	double *created_contrast_handle = 0;
	if (color_contrast == 0)
	{
		color_contrast = new double[tempImage->numColors];
		for (int colorIndex = 0; colorIndex < tempImage->numColors; colorIndex++)
			color_contrast[colorIndex] = DEFAULT_CONTRAST;
		created_contrast_handle = color_contrast;
	}

	int *created_brightness_handle = 0;
	if (color_brightness == 0)
	{
		color_brightness = new int[tempImage->numColors];
		for (int colorIndex = 0; colorIndex < tempImage->numColors; colorIndex++)
			color_brightness[colorIndex] = DEFAULT_BRIGHTNESS;
		created_brightness_handle = color_brightness;
	}

	int temp_pixel;

	/* Left Part of Triangle */
	if ( ((int)sorted_x[0] != (int)sorted_x[1]) && ((int)sorted_x[2] != (int)sorted_x[0]) )
	{
		double slope_1 = (sorted_y[1] - sorted_y[0]) / (sorted_x[1] - sorted_x[0]);
		double slope_2 = (sorted_y[2] - sorted_y[0]) / (sorted_x[2] - sorted_x[0]);

		//cout << slope_1 << " , " << slope_2 << endl;

		int row_start = (sorted_x[0] < 0) ? 0 : sorted_x[0];
		int row_end = (sorted_x[1] >= tempImage->height) ? tempImage->height - 1 : sorted_x[1];


		for (int row = row_start; row <= row_end; row++ )
		{
			int inter_col_1 = sorted_y[0] + slope_1 * (row - sorted_x[0]);
			int inter_col_2 = sorted_y[0] + slope_2 * (row - sorted_x[0]);
			
			if (inter_col_1 > inter_col_2)
			{
				int temp = inter_col_1;
				inter_col_1 = inter_col_2;
				inter_col_2 = temp;
			}
			
			inter_col_1 = (inter_col_1 < 0) ? 0 : inter_col_1;
			inter_col_2 = (inter_col_2 < 0) ? 0 : inter_col_2;
			inter_col_1 = (inter_col_1 >= tempImage->width) ? tempImage->width - 1 : inter_col_1;
			inter_col_2 = (inter_col_2 >= tempImage->width) ? tempImage->width - 1 : inter_col_2;
			
			for (int col = inter_col_1; col <= inter_col_2; col++)
			{
				for (int colorIndex = 0; colorIndex < tempImage->numColors; colorIndex++)
				{
					temp_pixel = color_contrast[colorIndex]*(double)tempImage->imageData [ (col * tempImage->width + row)*tempImage->numColors + colorIndex ] 
					+ color_brightness[colorIndex];

					tempImage->imageData [ (col * tempImage->width + row)*tempImage->numColors + colorIndex ] = (temp_pixel > 255)?255:(temp_pixel < 0)?0:temp_pixel;
				}
			}
			
		}
	}

	/* Right Part of Triangle */
	if ( ((int)sorted_x[1] != (int)sorted_x[2]) && ((int)sorted_x[0] != (int)sorted_x[2]) )
	{
		double slope_1 = (sorted_y[1] - sorted_y[2]) / (sorted_x[1] - sorted_x[2]);
		double slope_2 = (sorted_y[0] - sorted_y[2]) / (sorted_x[0] - sorted_x[2]);

		//cout << slope_1 << " , " << slope_2 << endl;

		int row_start = (sorted_x[2] >= tempImage->height) ? tempImage->height - 1 : sorted_x[2];
		int row_end = (sorted_x[1] < 0) ? 0 : sorted_x[1];

		for (int row = row_start; row >= row_end; row-- )
		{
			int inter_col_1 = sorted_y[2] + slope_1 * (row - sorted_x[2]);
			int inter_col_2 = sorted_y[2] + slope_2 * (row - sorted_x[2]);
			
			if (inter_col_1 > inter_col_2)
			{
				int temp = inter_col_1;
				inter_col_1 = inter_col_2;
				inter_col_2 = temp;
			}
			
			inter_col_1 = (inter_col_1 < 0) ? 0 : inter_col_1;
			inter_col_2 = (inter_col_2 < 0) ? 0 : inter_col_2;
			inter_col_1 = (inter_col_1 >= tempImage->width) ? tempImage->width - 1 : inter_col_1;
			inter_col_2 = (inter_col_2 >= tempImage->width) ? tempImage->width - 1 : inter_col_2;
			
			for (int col = inter_col_1; col <= inter_col_2; col++)
			{
				for (int colorIndex = 0; colorIndex < tempImage->numColors; colorIndex++)
				{
					temp_pixel = color_contrast[colorIndex]*(double)tempImage->imageData [ (col * tempImage->width + row)*tempImage->numColors + colorIndex ] 
					+ color_brightness[colorIndex];

					tempImage->imageData [ (col * tempImage->width + row)*tempImage->numColors + colorIndex ] = (temp_pixel > 255)?255:(temp_pixel < 0)?0:temp_pixel;
				}
			}
			
		}
	}

	/* Separating Line */	

	if (created_contrast_handle)
		delete created_contrast_handle;

	if (created_brightness_handle)
		delete created_brightness_handle;
}

void rightTriangle (int *x, int *y, IMAGE *tempImage, double *color_contrast, int *color_brightness)
{
	bool hypotenuse_vertex[3] = {false};

	double init_x, init_y;	// Initial vertex of hypotenuse
	if ( y[0] == y[1] )
	{
		init_x = x[2], init_y = y[2];
		hypotenuse_vertex[2] = true;
	}
	else if ( y[1] == y[2] )
	{
		init_x = x[0], init_y = y[0];
		hypotenuse_vertex[0] = true;
	}
	else //if ( y[2] == y[0] )
	{
		init_x = x[1], init_y = y[1];
		hypotenuse_vertex[1] = true;
	}

	double final_x, final_y;// Final vertex of hypotenuse
	if ( x[0] == x[1] )
	{
		final_x = x[2], final_y = y[2];
		hypotenuse_vertex[2] = true;
	}
	else if ( x[1] == x[2] )
	{
		final_x = x[0], final_y = y[0];
		hypotenuse_vertex[0] = true;
	}
	else //if ( x[3] == x[0] )
	{
		final_x = x[1],	final_y = y[1];
		hypotenuse_vertex[1] = true;
	}

	double other_x, other_y;
	if ( !hypotenuse_vertex[0] )
	{
		other_x = x[0], other_y = y[0];
	}
	else if ( !hypotenuse_vertex[1] )
	{
		other_x = x[1], other_y = y[1];
	}
	else //if ( !hypotenuse_vertex[2] )
	{
		other_x = x[2],	other_y = y[2];
	}

	double temp_x, temp_y;
	if (init_x > final_x)
	{
		temp_x = final_x, temp_y = final_y;
		final_x = init_x, final_y = init_y;
		init_x = temp_x, init_y = temp_y;
	}

	double slope = (final_y - init_y) / (final_x - init_x);

	double *created_contrast_handle = 0;
	if (color_contrast == 0)
	{
		color_contrast = new double[tempImage->numColors];
		for (int colorIndex = 0; colorIndex < tempImage->numColors; colorIndex++)
			color_contrast[colorIndex] = DEFAULT_CONTRAST;
		created_contrast_handle = color_contrast;
	}

	int *created_brightness_handle = 0;
	if (color_brightness == 0)
	{
		color_brightness = new int[tempImage->numColors];
		for (int colorIndex = 0; colorIndex < tempImage->numColors; colorIndex++)
			color_brightness[colorIndex] = DEFAULT_BRIGHTNESS;
		created_brightness_handle = color_brightness;
	}

	int temp_pixel;
	

	int row_start = (init_x < 0) ? 0 : init_x;
	int row_end = (final_x >= tempImage->height) ? tempImage->height - 1 : final_x;

	for (int row = row_start; row <= row_end; row++ )
	{
		int inter_col_1 = other_y;
		int inter_col_2 = init_y + slope * (row - init_x);		

		if (inter_col_1 > inter_col_2)
		{
			int temp = inter_col_2;
			inter_col_2 = inter_col_1;
			inter_col_1	= temp;
		}

		inter_col_1 = (inter_col_1 < 0) ? 0 : inter_col_1;
		inter_col_2 = (inter_col_2 < 0) ? 0 : inter_col_2;
		inter_col_1 = (inter_col_1 >= tempImage->width) ? tempImage->width - 1 : inter_col_1;
		inter_col_2 = (inter_col_2 >= tempImage->width) ? tempImage->width - 1 : inter_col_2;

		for (int col = inter_col_1; col <= inter_col_2; col++)
		{
			for (int colorIndex = 0; colorIndex < tempImage->numColors; colorIndex++)
			{
				temp_pixel = color_contrast[colorIndex]*(double)tempImage->imageData [ (col * tempImage->width + row)*tempImage->numColors + colorIndex ] 
				+ color_brightness[colorIndex];

				tempImage->imageData [ (col * tempImage->width + row)*tempImage->numColors + colorIndex ] = (temp_pixel > 255)?255:(temp_pixel < 0)?0:temp_pixel;
			}
		}
		
	}

	if (created_contrast_handle)
		delete created_contrast_handle;

	if (created_brightness_handle)
		delete created_brightness_handle;

}

void drawTriangle (int *x, int *y, IMAGE *tempImage, double *color_contrast, int *color_brightness)
{
	if (
			x[0] == x[1] && ( y[1] == y[2] || y[0] == y[2] )
		||	x[1] == x[2] && ( y[2] == y[0] || y[1] == y[0] )
		||	x[2] == x[0] && ( y[0] == y[1] || y[2] == y[1] )
		) {
			rightTriangle (x, y, tempImage, color_contrast, color_brightness);
			//cout << "Right Triangle" << endl;
		}
	else
	{
		nonRightTriangle (x, y, tempImage, color_contrast, color_brightness);
		//cout << "Non-Right Triangle" << endl;
	}
}