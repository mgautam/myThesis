#ifndef TRIANGLES_H
#define TRIANGLES_H

#include <bmpAccess/bmpEngine.h>

void nonRightTriangle (int *x, int *y, IMAGE *tempImage, double *color_contrast = 0, int *color_brightness = 0);
void rightTriangle (int *x, int *y, IMAGE *tempImage, double *color_contrast = 0, int *color_brightness = 0);
void drawTriangle (int *x, int *y, IMAGE *tempImage, double *color_contrast = 0, int *color_brightness = 0);

#endif