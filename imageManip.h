// Sean Murray
// smurra42
// Daniel Weber
//
//
// imageManip.h


#ifndef IMAGEMANIP_H
#define IMAGEMANIP_H

#include "ppm_io.h"
#include <stdio.h>
#include <stdlib.h>


int process_input(int argc, char* argv[]);
int process_operation(int argc, char* argv[], Image * im1);
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
int calc_blurry_pixel(double** blur_matrix, int size);



#endif
