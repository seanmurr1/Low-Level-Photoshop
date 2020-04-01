// Sean Murray
// JHED: smurra42
// Daniel Weber
// JHED: dweber11
//
// imageManip.h


#ifndef IMAGEMANIP_H
#define IMAGEMANIP_H

#include "ppm_io.h"
#include <stdio.h>
#include <stdlib.h>


/* Changes the exposure of all pixels of a given image with corresponding EV value.
 * Returns pointer to new altered image.
 */
Image * change_exposure(Image * im, double EV);

/*
 *
 */
Image * alpha_blend(Image * im1, Image * im2, double alpha);

/* Expands all pixels in an image to a 2x2 square, thus zooming in.
 * Returns pointer to new altered image.
 */
Image * zoom_in(Image * im);

/* Shrinks 2x2 squares of pixels in original image to averaged 1x1 pixels in zoomed out image.
 * Returns pointer to new altered image.
 */
Image * zoom_out(Image * im);

/*
 *
 */
Image * pointilism(Image * im);

/* Performs swirl transformation of given image with given scale and center.
 * Returns pointer to new altered image.
 */
Image * swirl(Image * im, int col, int row, int scale);

/*
 *
 */
Image * blur(Image * im, double radius);

/* Function alters the color channels of a single pixel with corresponding EV value.
 * Deals with values exceeding maximum.
 */
void alter_exposure(Image * im, int index, double EV);

/*
 *
 */
int index_converter(int row, int column, int num_cols);

/*
 *
 */
double** generate_gaussian_matrix(double sigma);

/*
 *
 */
int calc_blurry_pixel(double** blur_matrix, double** general_blur_matrix, int size);

/* Copies a pixel into surrounding 2x2 square with the same color channgel values.
 */
void expand_pixel(Image * im1, Image * imOut, int index);

/* Takes average color channel values of 2x2 square corresponding to larger image
 * and places this averaged pixel at index i in smaller image.
 */
void shrink_pixel(Image * im1, Image * imOut, int i);

/* Converts index into x-coordinate given # columns in image.
 */
int to_x_coord(int i, int num_cols);

/* Converts index into y-coordinate given # columns in image. 
 */
int to_y_coord(int i, int num_cols);

/* Converts (x,y) into an index value given # columns in image.
 */
int coord_to_index(int x, int y, int num_cols);

/* Calculuates alpha value for swirl function.
 */
double calc_alpha(int x, int y, int cX, int cY, int s);

/* Performs swirl transformation on a given pixel.
 * If new coordinates are out of bounds, pixel is set to be black.
 */
void swirl_pixel(Image * imOut, Image * im, int cX, int cY, int s, int index);


#endif
