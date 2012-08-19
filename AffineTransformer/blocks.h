#ifndef BLOCKS_H
#define BLOCKS_H

#include <Matrix/matrix.h>

IMAGE* render_selection ( MATRIX *selection_box, MATRIX *AffineModel, IMAGE *inImage, double *color_contrast = 0, int *color_brightness = 0);
IMAGE* extract_selection (int num_vertices, int *x, int *y, IMAGE *inImage);

#endif