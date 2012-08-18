#ifndef POLYGONS_H
#define POLYGONS_H

#include <bmpAccess/bmpEngine.h>

IMAGE* drawPolygon (int num_vertices, int *x, int *y, IMAGE *inImage,	double *color_contrast = 0, int *color_brightness = 0);
IMAGE* extractPolygonRegion (int num_vertices, int *x, int *y, IMAGE *inImage);

#endif