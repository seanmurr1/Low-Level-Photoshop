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
void expand_pixel(Image * im1, Image * imOut, int index);
void shrink_pixel(Image * im1, Image * imOut, int i);
int to_x_coord(int i, int num_cols);
int to_y_coord(int i, int num_cols);
int coord_to_index(int x, int y, int num_cols);
double calc_alpha(int x, int y, int cX, int cY, int s);
void swirl_pixel(Image * imOut, Image * im, int cX, int cY, int s, int index);




#endif
