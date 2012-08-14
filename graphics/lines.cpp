#include <graphics/lines.h>

#define CONTRAST 0.5
#define BRIGHTNESS 0

void drawLine (double x, double *y, IMAGE *inImage)
{
	int row = x;
	int init_y, final_y;
	if (y[0] < y[1])
	{		
		final_y = y[1];
		init_y = y[0];
	}
	else
	{
		final_y = y[0];
		init_y = y[1];
	}
	for (int col = init_y; col <= final_y; col++)
	{
		inImage->imageData [ col * inImage->width + row ] = CONTRAST*(double)inImage->imageData [ col * inImage->width + row ] + BRIGHTNESS;
	}

}