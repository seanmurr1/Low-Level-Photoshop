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


Image * change_exposure(Image * im, double EV);
Image * alpha_blend(Image * im1, Image * im2, double alpha);
Image * zoom_in(Image * im);
Image * zoom_out(Image * im);
Image * pointilism(Image * im);
Image * swirl(Image * im, int col, int row, int scale);
Image * blur(Image * im, double radius);
void alter_exposure(Image * im, int index, double EV);
int index_converter(int row, int column, int num_cols);
double** generate_gaussian_matrix(double sigma);
int calc_blurry_pixel(double** blur_matrix, double** general_blur_matrix, int size);
void expand_pixel(Image * im1, Image * imOut, int index);
void shrink_pixel(Image * im1, Image * imOut, int i);
int to_x_coord(int i, int num_cols);
int to_y_coord(int i, int num_cols);
int coord_to_index(int x, int y, int num_cols);
double calc_alpha(int x, int y, int cX, int cY, int s);
void swirl_pixel(Image * imOut, Image * im, int cX, int cY, int s, int index);



#endif
