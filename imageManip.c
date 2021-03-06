// Sean Murray
// JHED: smurra42
// Daniel Weber
// JHED: dweber11
//
// imageManip.c


#include "imageManip.h"
#include "ppm_io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#define PI 3.14159265359


/* Function alters the color channels of a single pixel with corresponding EV value.
 * Deals with values exceeding maximum.
 */
void alter_exposure(Image * im, int index, double EV) {
	// Modifying each color channel
	int r = im->data[index].r * pow(2.0, EV);
	int g = im->data[index].g * pow(2.0, EV);
	int b = im->data[index].b * pow(2.0, EV);
	// Checking if value exceeds maximum
	// Red channel
	if (r > 255) {
		r = 255;
	} else if (r < 0) {
		r = 0;
	}
	// Blue channel
	if (b > 255) {
		b = 255;
	} else if (b < 0) {
		b = 0;
	}
	//Green channel
	if (g > 255) {
		g = 255;
	} else if (g < 0) {
		g = 0;
	}	

	// Updating data
	im->data[index].r = r;
	im->data[index].g = g;
	im->data[index].b = b;
}

/* Changes the exposure of all pixels of a given image with corresponding EV value.
 * Returns pointer to new altered image.
 */
Image * change_exposure(Image * im, double EV) {
	// Copying image
	Image * imOut = copy_image(im);
	// Checking for success
	if (imOut == NULL) {
		return NULL;
	}	
	// Changing exposure of pixels
	for (int i = 0; i < imOut->rows * imOut->cols; i++) {
		alter_exposure(imOut, i, EV);
	}
  // Returning image
  return imOut;
}

/*
 * Blends the two images based on a given value of alpha which determines the blur ratio.
 * Return pointer to new image struct.
 */
Image * alpha_blend(Image * im1, Image * im2, double alpha) {
	Image * imOut = (Image *) malloc(sizeof(Image));
	Image * smaller_row_image;
	Image * larger_row_image;
	Image * smaller_col_image;
	Image * larger_col_image;
	//Finding relative image sizes to determine how to blend
	if (im1->rows >= im2->rows) {
		smaller_row_image = im2;
		larger_row_image = im1;
		imOut->rows = im1->rows;
	} else {
		smaller_row_image = im1;
		larger_row_image = im2;
		imOut->rows = im2->rows;
	}
	if (im1->cols >= im2->cols) {
		smaller_col_image = im2;
		larger_col_image = im1;
		imOut->cols = im1->cols;
	} else {
		smaller_col_image = im1;
		larger_col_image = im2;
		imOut->cols = im2->cols;
	}
	//Ensuring that images are differentiated
	assert(larger_col_image != smaller_col_image);
	assert(larger_row_image != smaller_row_image);
	//Creating new image based on dimensions of both input images
	imOut->data = (Pixel *) malloc(sizeof(Pixel) * imOut->rows * imOut->cols);
	for (int i = 0; i < imOut->rows * imOut->cols; i++) {
		imOut->data[i].r = 0;
		imOut->data[i].g = 0;
		imOut->data[i].b = 0;
	}
	for (int curr_y = 0; curr_y < smaller_row_image->rows; curr_y++) {
		for (int curr_x = 0; curr_x < smaller_col_image->cols; curr_x++) {
		//Setting pixel values for areas of overlap between the two images
		imOut->data[index_converter(curr_y, curr_x, imOut->cols)].r = alpha * im1->data[index_converter(curr_y, curr_x, im1->cols)].r + ((1 - alpha) * im2->data[index_converter(curr_y, curr_x, im2->cols)].r);
		imOut->data[index_converter(curr_y, curr_x, imOut->cols)].g = alpha * im1->data[index_converter(curr_y, curr_x, im1->cols)].g + ((1 - alpha) * im2->data[index_converter(curr_y, curr_x, im2->cols)].g);
		imOut->data[index_converter(curr_y, curr_x, imOut->cols)].b = alpha * im1->data[index_converter(curr_y, curr_x, im1->cols)].b + ((1 - alpha) * im2->data[index_converter(curr_y, curr_x, im2->cols)].b);
		}
	}
	if (smaller_row_image->rows != imOut->rows) {
		for (int curr_y = smaller_row_image->rows; curr_y < larger_row_image->rows; curr_y++) {
			for (int curr_x = 0; curr_x < larger_row_image->cols; curr_x++) {
			//Setting pixel values for rows that don't overlap (y dimension leftover)
				imOut->data[index_converter(curr_y, curr_x, imOut->cols)].r = larger_row_image->data[index_converter(curr_y, curr_x, larger_row_image->cols)].r;
				imOut->data[index_converter(curr_y, curr_x, imOut->cols)].g = larger_row_image->data[index_converter(curr_y, curr_x, larger_row_image->cols)].g;
				imOut->data[index_converter(curr_y, curr_x, imOut->cols)].b = larger_row_image->data[index_converter(curr_y, curr_x, larger_row_image->cols)].b;
			}
		}
	}
	if (smaller_col_image->cols != imOut->cols) {
		for (int curr_y = 0; curr_y < larger_col_image->rows; curr_y++) {
			for (int curr_x = smaller_col_image->cols; curr_x < larger_col_image->cols; curr_x++) {
			//Setting pixel values for columns that don't overlap (x dimension leftover)
				imOut->data[index_converter(curr_y, curr_x, imOut->cols)].r = larger_col_image->data[index_converter(curr_y, curr_x, larger_col_image->cols)].r;
				imOut->data[index_converter(curr_y, curr_x, imOut->cols)].g = larger_col_image->data[index_converter(curr_y, curr_x, larger_col_image->cols)].g;
				imOut->data[index_converter(curr_y, curr_x, imOut->cols)].b = larger_col_image->data[index_converter(curr_y, curr_x, larger_col_image->cols)].b;
			}
		}
	}
  return imOut;
}

/* Copies a pixel into surrounding 2x2 square with the same color channgel values.
 */
void expand_pixel(Image * im1, Image * imOut, int index) {
	// Number of pixels per row
	int col = im1->cols;
	// Expanding pixel to 2x2 square
	imOut->data[2 * index + (index / col) * 2 * col] = im1->data[index];
	imOut->data[2 * index + 1 + (index / col) * 2 * col] = im1->data[index];
	imOut->data[2 * index + 2 * col + (index / col) * 2 * col] = im1->data[index];
	imOut->data[2 * index + 2 * col + 1 + (index / col) * 2 * col] = im1->data[index];
}

/* Expands all pixels in an image to a 2x2 square, thus zooming in.
 * Returns pointer to new altered image.
 */
Image * zoom_in(Image * im) {
	// Creating new image
	Image * imOut = create_image((im->cols * 2), (im->rows * 2));
	// Checking for success 
	if (imOut == NULL) {
		return NULL;
	}
	// Expanding pixels
	for (int i = 0; i < im->cols * im->rows; i++) {
		expand_pixel(im, imOut, i);
	}
 	// Returning image
	return imOut;
}

/* Takes average color channel values of 2x2 square corresponding to larger image
 * and places this averaged pixel at index i in smaller image.
 */
void shrink_pixel(Image * im1, Image * imOut, int i) {
	// Number of pixels per row of bigger image
	int COL = im1->cols;
	// Number of pixels per row of smaller image
	int col = imOut->cols;
	// Does im1 have an odd # of cols?
	int odd;
	if (COL % 2 != 0) {
		odd = 1;
	} else {
		odd = 0;
	}
	// Obtaining four pixels
	Pixel p1 = im1->data[2 * i + (i / col) * COL + (i / col) * odd];
	Pixel p2 = im1->data[2 * i + 1 + (i / col) * COL + (i / col) * odd];
	Pixel p3 = im1->data[2 * i + COL + (i / col) * COL + (i / col) * odd];
	Pixel p4 = im1->data[2 * i + COL + 1 + (i / col) * COL + (i / col) * odd];
	
	// Calculating averages
	Pixel shrunk;
	shrunk.r = (p1.r + p2.r + p3.r + p4.r) / 4;
	shrunk.g = (p1.g + p2.g + p3.g + p4.g) / 4;
	shrunk.b = (p1.b + p2.b + p3.b + p4.b) / 4;

	// Placing in shrunken pixel
	imOut->data[i] = shrunk;
}

/* Shrinks 2x2 squares of pixels in original image to averaged 1x1 pixels in zoomed out image.
 * Returns pointer to new altered image.
 */
Image * zoom_out(Image * im) {
	// Creating new image
	Image * imOut = create_image((im->cols / 2), (im->rows / 2));
	// Checking for success
	if (imOut == NULL) {
		return NULL;
	}
	// Shrinking pixels
	for (int i = 0; i < imOut->cols * imOut->rows; i++) {
		// Creating shrunken pixel
		shrink_pixel(im, imOut, i);
	}
	// Returning image
	return imOut;
}

/*
 * Applies pointilism effect on an image on a random selection of 3% of the pixels in the image.
 * Returns pointer to new altered image.
 */
Image * pointilism(Image * im) {
	srand(1);
	int number_of_pixels = im->rows * im->cols;
	int number_pixels_to_change = number_of_pixels * .03;
		int curr_x = 0;
		int curr_y = 0;
		int curr_radius = 0;
	for (int i = 0; i < number_pixels_to_change; i++) {
		curr_x = rand() % im->cols;
		curr_y = rand() % im->rows;
		curr_radius = rand() % 5 + 1;
		//Checks diameter sized square around each central pixel
		for (int curr_col = curr_x - curr_radius; curr_col <= curr_x + curr_radius; curr_col++) {
			//Checks if x coord is out of bounds on either side
			if (curr_col < 0) {
				continue;
			}
			else if (curr_col >= im->cols) {
				break;
			}
			for(int curr_row = curr_y - curr_radius; curr_row <= curr_y + curr_radius; curr_row++) {
				//Checks if y coord is out of bounds on either side
				if (curr_row < 0) {
				continue;
				}
				else if (curr_row >= im->rows) {
					break;
				}
				//Checks if pixels is with circle radius from center pixel 
				else if((pow(curr_col - curr_x, 2) + pow(curr_row - curr_y, 2)) <= pow(curr_radius, 2)) {
					//Changes pixel values to center pixel values
					im->data[index_converter(curr_row, curr_col, im->cols)].r = im->data[index_converter(curr_y, curr_x, im->cols)].r;
					im->data[index_converter(curr_row, curr_col, im->cols)].g = im->data[index_converter(curr_y, curr_x, im->cols)].g;
					im->data[index_converter(curr_row, curr_col, im->cols)].b = im->data[index_converter(curr_y, curr_x, im->cols)].b;
				}
			}
		}
	}
  return im; 
}

/*
 * Converts from 2d coordinates to 1d array location
 */
int index_converter(int row, int column, int num_cols) {
	int row_only_index = row*num_cols + column;
	return row_only_index;
}

/* Converts index into x-coordinate given # columns in image.
 */
int to_x_coord(int i, int num_cols) {
	// Finding x coord
	return i % num_cols;
}

/* Converts index into y-coordinate given # columns in image. 
 */
int to_y_coord(int i, int num_cols) {
	// Finding y coord
	return i / num_cols;
}

/* Converts (x,y) into an index value given # columns in image.
 */
int coord_to_index(int x, int y, int num_cols) {
	// Calculating index
	return (y * num_cols) + x;
}

/* Calculuates alpha value for swirl function.
 */
double calc_alpha(int x, int y, int cX, int cY, int s) {
	// Calculating alpha
	return (double) (sqrt(pow(x - cX, 2.0) + pow(y - cY, 2.0)) / s);

}

/* Performs swirl transformation on a given pixel.
 * If new coordinates are out of bounds, pixel is set to be black.
 */
void swirl_pixel(Image * imOut, Image * im, int cX, int cY, int s, int index) {
	// Finding x and y coord of index value
	int x = to_x_coord(index, im->cols);
	int y = to_y_coord(index, im->cols);
	// Calculating alpha
	double alpha = calc_alpha(x, y, cX, cY, s);

	// Calculating new coordinates
	// x coord
	int new_x = (int) ((x - cX) * cos(alpha) - (y - cY) * sin(alpha) + cX);
	// Checking for valid value
	// Case: pixel index is out of bounds: thus make pixel black
	if (new_x < 0 || new_x > im->cols - 1) {
		imOut->data[index].r = 0;
		imOut->data[index].g = 0;
		imOut->data[index].b = 0;
		return;
	} 
	// y coord
	int new_y = (int) ((x - cX) * sin(alpha) + (y - cY) * cos(alpha) + cY);
	// Checking for valid value
	// Case: pixel index is out of bounds: thus make pixel black
	if (new_y < 0 || new_y > im->rows - 1) {
		imOut->data[index].r = 0;
		imOut->data[index].g = 0;
		imOut->data[index].b = 0;
		return;
	} 
	// Calculating new index
	int new_index = coord_to_index(new_x, new_y, im->cols);

	// Placing swirled pixel
	imOut->data[index] = im->data[new_index];
}

/* Performs swirl transformation of given image with given scale and center.
 * Returns pointer to new altered image.
 */
Image * swirl(Image * im, int col, int row, int scale) {
	// Creating new image
	Image * imOut = create_image(im->cols, im->rows);
	// Checking for success
	if (imOut == NULL) {
		return NULL;
	} 
	// Swirling pixels
	for (int i = 0; i < im->cols * im->rows; i++) {
		swirl_pixel(imOut, im, col, row, scale, i);
	}
	// Returning swirled image
	return imOut;
}

/*
 * Applies a Gaussian blur filter to the image. Strength of the blur is determined by the sigma value entered by the user.
 * Returns pointer to new blurred image.
 */
Image * blur(Image * im, double radius) {
	Image * imOut = copy_image(im);
	int matrix_size = radius / 0.1;
	if (matrix_size % 2 == 0) {
		matrix_size += 1;
	}
	double** general_blur_matrix = generate_gaussian_matrix(radius);
	int matrix_offset = matrix_size / 2;
	//Allocates blur matrix to be altered for each pixel
	double** pixel_matrix = (double**) malloc(sizeof(double *) * matrix_size);
	for (int i = 0; i < matrix_size; i++) {
		pixel_matrix[i] = malloc(sizeof(double) * matrix_size);
	}
	//Will go through every pixel in the image and create specific pixel blur matrix for each
	for (int im_y = 0; im_y < im->rows; im_y++) {
		for (int im_x = 0; im_x < im->cols; im_x++) {
			//For Red Channel
			for (int pixel_y = 0; pixel_y < matrix_size; pixel_y++) {
				for (int pixel_x = 0; pixel_x < matrix_size; pixel_x++) {
					if(im_x - matrix_offset + pixel_x < 0 || im_x - matrix_offset + pixel_x >= im->cols  
					   || im_y - matrix_offset + pixel_y < 0 || im_y - matrix_offset + pixel_y >= im->rows) {
						//If the guassian matrix overlay overhangs the image, the value is set to -1
						pixel_matrix[pixel_y][pixel_x] = -1;
					} else {
						//Sets the pixel matrix to the blur coefficient multiplied by the pixel red value
						//im_x - matrix_offset + pixel_x means that we start from offset (due to placing the center of the guassian matrix on our current pixel)
						//behind our current pixel image and then increment through the columns of our specific pixel blur matrix
						pixel_matrix[pixel_y][pixel_x] = general_blur_matrix[pixel_y][pixel_x] * (im->data[index_converter(im_y - matrix_offset + pixel_y, im_x - matrix_offset + pixel_x, im->cols)].r);
					}
				}
			}
			imOut->data[index_converter(im_y, im_x, imOut->cols)].r = calc_blurry_pixel(pixel_matrix, general_blur_matrix, matrix_size);
			
			//For Green Channel
			for (int pixel_y = 0; pixel_y < matrix_size; pixel_y++) {
				for (int pixel_x = 0; pixel_x < matrix_size; pixel_x++) {
					if(im_x - matrix_offset + pixel_x < 0 || im_x - matrix_offset + pixel_x >= im->cols 
					   || im_y - matrix_offset + pixel_y < 0 || im_y - matrix_offset + pixel_y >= im->rows) {
						pixel_matrix[pixel_y][pixel_x] = -1;
					} else {
					pixel_matrix[pixel_y][pixel_x] = general_blur_matrix[pixel_y][pixel_x] * (im->data[index_converter(im_y - matrix_offset + pixel_y, im_x - matrix_offset + pixel_x, im->cols)].g);
					}
				}
			}
			imOut->data[index_converter(im_y, im_x, imOut->cols)].g = calc_blurry_pixel(pixel_matrix, general_blur_matrix, matrix_size);

			//For Blue Channel
			for (int pixel_y = 0; pixel_y < matrix_size; pixel_y++) {
				for (int pixel_x = 0; pixel_x < matrix_size; pixel_x++) {
					if(im_x - matrix_offset + pixel_x < 0 || im_x - matrix_offset + pixel_x >= im->cols 
					   || im_y - matrix_offset + pixel_y < 0 || im_y - matrix_offset + pixel_y >= im->rows) {
						pixel_matrix[pixel_y][pixel_x] = -1;
					} else {
					pixel_matrix[pixel_y][pixel_x] = general_blur_matrix[pixel_y][pixel_x] * (im->data[index_converter(im_y - matrix_offset + pixel_y, im_x - matrix_offset + pixel_x, im->cols)].b);
					}
				}
			}
			imOut->data[index_converter(im_y, im_x, imOut->cols)].b = calc_blurry_pixel(pixel_matrix, general_blur_matrix, matrix_size);
		}
	}

	for (int i = 0; i < matrix_size; i++) {
		free(general_blur_matrix[i]);
	}
	for (int i = 0; i < matrix_size; i++) {
		free(pixel_matrix[i]);
	}
	free(general_blur_matrix);
	free(pixel_matrix);
  return imOut; 
}

/*
 * Generates a gaussian blur matrix based on the inputted value of sigma.
 * Returns pointer to 2d blur matrix.
 */
double** generate_gaussian_matrix(double sigma) {
	int size = sigma / 0.1;
	if (size % 2 == 0) {
		size += 1;
	}
	double** guassian_matrix = (double**) malloc(sizeof(double *) * size);
	for (int i = 0; i < size; i++) {
		guassian_matrix[i] = malloc(sizeof(double) * size);
	}
	for (int y = 0; y < size; y++) {
		for (int x = 0; x < size; x++) {
			int dx = x - (size / 2);
			int dy = y - (size / 2);
			//Generates values based on given formula
			double g = (1.0 / (2.0 * PI * pow(sigma, 2))) * exp( -(pow(dx, 2) + pow(dy, 2)) / (2 * pow(sigma, 2)));
			guassian_matrix[y][x] = g;
		}
	}
	return guassian_matrix;
}

/*
 * Calculates the value of the specific pixel being blurred for an arbitrary color channel.
 * Returns the value of blurry pixel value for the specific color channel.
 */
int calc_blurry_pixel(double** pixel_blur_matrix, double** general_blur_matrix, int size) {
	double sum = 0;
	double weight = 0;
	for (int y = 0; y < size; y++) {
		for (int x = 0; x < size; x++) {
			if (pixel_blur_matrix[y][x] >= 0) {
				sum += pixel_blur_matrix[y][x];
				weight += general_blur_matrix[y][x];
			}
		}
	}
	int color_value = sum / weight;
	return color_value;
}



