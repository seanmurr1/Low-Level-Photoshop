// Sean Murray
// JHED: smurra42
// Daniel Weber
// JHED: dweber11
//
// imageManip.c
//

#include "imageManip.h"
#include "ppm_io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#define PI 3.14



void alter_exposure(Image * im, int index, double EV) {
	// Modifying each color channel
	unsigned char r = im->data[index].r * pow(2.0, EV);
	unsigned char g = im->data[index].g * pow(2.0, EV);
	unsigned char b = im->data[index].b * pow(2.0, EV);
	// Checking if value exceeds maximum
	if (EV > 0 ) {
		if (r < im->data[index].r) {
			r = 255;
		}
		if (g < im->data[index].g) {
			g = 255;
		}
		if (b < im->data[index].b) {
			b = 255;
		}
	}
	// Updating data
	im->data[index].r = r;
	im->data[index].g = g;
	im->data[index].b = b;
}
/**
 *
 **/
Image * change_exposure(Image * im, double EV) {
	// Copying image
	Image * imOut = copy_image(im);
	// Checking for success
	if (imOut == NULL) {
		return NULL;
	}	
	for (int i = 0; i < imOut->rows * imOut->cols; i++) {
		alter_exposure(imOut, i, EV);
	}
  return imOut;
}

/**
 *
 **/
Image * alpha_blend(Image * im1, Image * im2, double alpha) {
	Image * imOut = (Image *) malloc(sizeof(Image));
	Image * smaller_row_image;
	Image * larger_row_image;
	Image * smaller_col_image;
	Image * larger_col_image;
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
	assert(larger_col_image != smaller_col_image);
	assert(larger_row_image != smaller_row_image);
	imOut->data = (Pixel *) malloc(sizeof(Pixel) * imOut->rows * imOut->cols);
	for (int i = 0; i < imOut->rows * imOut->cols; i++) {
		imOut->data[i].r = 0;
		imOut->data[i].g = 0;
		imOut->data[i].b = 0;
	}
	for (int curr_y = 0; curr_y < smaller_row_image->rows; curr_y++) {
		for (int curr_x = 0; curr_x < smaller_col_image->cols; curr_x++) {
		imOut->data[index_converter(curr_y, curr_x, imOut->cols)].r = alpha * im1->data[index_converter(curr_y, curr_x, im1->cols)].r + ((1 - alpha) * im2->data[index_converter(curr_y, curr_x, im2->cols)].r);
		imOut->data[index_converter(curr_y, curr_x, imOut->cols)].g = alpha * im1->data[index_converter(curr_y, curr_x, im1->cols)].g + ((1 - alpha) * im2->data[index_converter(curr_y, curr_x, im2->cols)].g);
		imOut->data[index_converter(curr_y, curr_x, imOut->cols)].b = alpha * im1->data[index_converter(curr_y, curr_x, im1->cols)].b + ((1 - alpha) * im2->data[index_converter(curr_y, curr_x, im2->cols)].b);
		}
	}
	if (smaller_row_image->rows != imOut->rows) {
		for (int curr_y = smaller_row_image->rows; curr_y < larger_row_image->rows; curr_y++) {
			for (int curr_x = 0; curr_x < larger_row_image->cols; curr_x++) {
				imOut->data[index_converter(curr_y, curr_x, imOut->cols)].r = larger_row_image->data[index_converter(curr_y, curr_x, larger_row_image->cols)].r;
				imOut->data[index_converter(curr_y, curr_x, imOut->cols)].g = larger_row_image->data[index_converter(curr_y, curr_x, larger_row_image->cols)].g;
				imOut->data[index_converter(curr_y, curr_x, imOut->cols)].b = larger_row_image->data[index_converter(curr_y, curr_x, larger_row_image->cols)].b;
			}
		}
	}
	if (smaller_col_image->cols != imOut->cols) {
		for (int curr_y = 0; curr_y < larger_col_image->rows; curr_y++) {
			for (int curr_x = smaller_col_image->cols; curr_x < larger_col_image->cols; curr_x++) {
				imOut->data[index_converter(curr_y, curr_x, imOut->cols)].r = larger_col_image->data[index_converter(curr_y, curr_x, larger_col_image->cols)].r;
				imOut->data[index_converter(curr_y, curr_x, imOut->cols)].g = larger_col_image->data[index_converter(curr_y, curr_x, larger_col_image->cols)].g;
				imOut->data[index_converter(curr_y, curr_x, imOut->cols)].b = larger_col_image->data[index_converter(curr_y, curr_x, larger_col_image->cols)].b;
			}
		}
	}
  return imOut; // TODO remove stub
}

/**
 *
 **/
Image * zoom_in(Image * im) {

  return NULL; // TODO remove stub
}

/**
 *
 **/
Image * zoom_out(Image * im) {

  return NULL; // TODO remove stub
}

/**
 *
 **/
Image * pointilism(Image * im) {
	Image * im_out = copy_image(im);
	int number_of_pixels = im_out->rows * im_out->cols;
	int number_pixels_to_change = number_of_pixels * .03;
	for (int i = 0; i < number_pixels_to_change; i++) {
		int curr_x = rand() % im_out->cols;
		int curr_y = rand() % im_out->rows;
		int curr_radius = rand() % 5 + 1;
		for (int curr_col = curr_x - curr_radius; curr_col <= curr_x + curr_radius; curr_col++) {
			if (curr_col < 0) {
				continue;
			}
			else if (curr_col > im_out->cols) {
				break;
			}
			for(int curr_row = curr_y - curr_radius; curr_row <= curr_y + curr_radius; curr_row++) {
				if (curr_row < 0) {
				continue;
				}
				else if (curr_row > im_out->rows) {
					break;
				}
				else if((pow(curr_col - curr_x, 2) + pow(curr_row - curr_y, 2)) <= pow(curr_radius, 2)) {
					im_out->data[index_converter(curr_row, curr_col, im_out->cols)] = im_out->data[index_converter(curr_y, curr_x, im_out->cols)];
				}
			}
		}
	}
  return im_out; 
}

int index_converter(int row, int column, int num_cols) {
	int row_only_index = row*num_cols + column;
	return row_only_index;
}

/**
 *
 *
 **/

Image * swirl(Image * im, int col, int row, int scale) {


  return NULL; // TODO remove stub
}

/**
 *
 *
 **/
Image * blur(Image * im, double radius) {
	Image * imOut = copy_image(im);
	double** general_blur_matrix = generate_gaussian_matrix(radius);
	int matrix_size = radius / 0.1;
	int matrix_offset = matrix_size / 2;
	double** pixel_matrix = (double**) malloc(sizeof(double *) * matrix_size);
	for (int i = 0; i < matrix_size; i++) {
		pixel_matrix[i] = malloc(sizeof(double) * matrix_size);
	}
	for (int im_y = 0; im_y < im->rows; im_y++) {
		for (int im_x = 0; im_x < im->cols; im_x++) {
			//For Red Channel
			for (int pixel_y = 0; pixel_y < matrix_size; pixel_y++) {
				for (int pixel_x = 0; pixel_x < matrix_size; pixel_x++) {
					if(im_x - matrix_offset + pixel_x < 0 || im_x - matrix_offset + pixel_x >= im->cols || im_y - matrix_offset + pixel_y < 0 || im_y - matrix_offset + pixel_y >= im->rows) {
						pixel_matrix[pixel_y][pixel_x] = -1;
					} else {
					pixel_matrix[pixel_y][pixel_x] = general_blur_matrix[pixel_y][pixel_x] * (im->data[index_converter(im_y - matrix_offset + pixel_y, im_x - matrix_offset + pixel_x, im->cols)].r);
					}
				}
			}
			imOut->data[index_converter(im_y, im_x, imOut->cols)].r = calc_blurry_pixel(pixel_matrix, general_blur_matrix, matrix_size);
			/*for (int pixel_y = 0; pixel_y < matrix_size; pixel_y++) {
				for (int pixel_x = 0; pixel_x < matrix_size; pixel_x++) {
					printf("%f ", pixel_matrix[pixel_y][pixel_x]);
				}
				printf("\n\n");
			}
			printf("\n"); 
			printf("\n %d \n", calc_blurry_pixel(pixel_matrix, matrix_size)); */
			//For Green Channel
			for (int pixel_y = 0; pixel_y < matrix_size; pixel_y++) {
				for (int pixel_x = 0; pixel_x < matrix_size; pixel_x++) {
					if(im_x - matrix_offset + pixel_x < 0 || im_x - matrix_offset + pixel_x >= im->cols || im_y - matrix_offset + pixel_y < 0 || im_y - matrix_offset + pixel_y >= im->rows) {
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
					if(im_x - matrix_offset + pixel_x < 0 || im_x - matrix_offset + pixel_x >= im->cols || im_y - matrix_offset + pixel_y < 0 || im_y - matrix_offset + pixel_y >= im->rows) {
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


double** generate_gaussian_matrix(double sigma) {
	int size = sigma / 0.1;
	double** guassian_matrix = (double**) malloc(sizeof(double *) * size);
	for (int i = 0; i < size; i++) {
		guassian_matrix[i] = malloc(sizeof(double) * size);
	}
	for (int y = 0; y < size; y++) {
		for (int x = 0; x < size; x++) {
			int dx = x - (size / 2);
			int dy = y - (size / 2);
			double g = (1.0 / (2.0 * PI * pow(sigma, 2))) * exp( -(pow(dx, 2) + pow(dy, 2)) / (2 * pow(sigma, 2)));
			guassian_matrix[y][x] = g;
		}
	}
	return guassian_matrix;
}
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
	//printf("sum: %f \n", sum);
	//printf("counter: %f \n", counter);
	int color_value = sum / weight;
	return color_value;
}
/**
 * Processes command-line arguments and handles errors. 
 *
 **/
int process_input(int argc, char* argv[]) {

  // Checking if input and output filenames exist
  if (argc < 3) {
	printf("Failed to supply input filename or output filename, or both.\n");
	return 1;
  }

  // Attempting to open input file
  // TODO check if "b" is needed here
  FILE* input = fopen(argv[1], "rb");
  if (input == NULL) {
	printf("Error: input file could not be opened.\n");
	return 2;
  }

  // Attempting to read PPM file
  Image * im1 = read_ppm(input);
  if (im1 == NULL) {
	printf("Error: input file is not a properly formatted PPM file, or reading input has failed.\n");
	return 3;
  }
  // Closing input file
  fclose(input);

  // Checking operation
  int check = process_operation(argc, argv, im1);
  // Case: error detected
  if (check != 0) {
	// Returning proper error value
	// TODO free imOut and im1 and others if needed
	return check;
  } 
 
  // TODO free images here

return 0;
}



/**
 * Processes given operation and errors, if any.
 *
 **/
int process_operation(int argc, char* argv[], Image * im1) {

  // Case: exposure function
  if (strcmp(argv[3], "exposure") == 0) {
	// Checking for proper # of args
	if (argc != 5) {
		printf("Error: incorrect number of arguments for specified operation.\n");
		destroy(im1);
		return 5;
	}
	// Obtaining EV value and checking for validity
	double EV;
	if (sscanf(argv[4], "%lf", &EV) != 1) {
		printf("Error: incorrect kind of arguments for specified opertaion.\n");
		destroy(im1);
		return 5;
	}
	// Checking for proper range of EV value
	// TODO do we need epsilon value to compare doubles?
	if (EV > 3 || EV < -3) {
		printf("Error: invalid arguments for operation.\n ");
		destroy(im1);
		return 6;
	}
	
	// TODO call exposure function  
	Image * imOut = change_exposure(im1, EV);
	if (imOut == NULL) {
		destroy(im1);
		return 8;
	}
	int check = output_image(imOut, argv);
	destroy(im1);
	destroy(imOut);
	if (check != 0) {
		return check; 
	}
	return 0;
  }
  // Case: blend function 
  else if (strcmp(argv[3], "blend") == 0) {
	// Checking for proper # of args
	if (argc != 6) {
		printf("Error: incorrect number of arguments for specified operation.\n");
		return 5;
	}
	// Attempting to open second input file
	FILE* input2 = fopen(argv[4], "rb");
	if (input2 == NULL) {
		printf("Error: could not open second input file.\n");
		return 2;
	}	
	// Attempting to read PPM image
	Image * im2 = read_ppm(input2);
	if (im2 == NULL) {
		printf("Error: second input file is not properly formatted or reading has failed.\n");
		return 3;
	}
	// Obtaining alpha value 
	double alpha;
	if (sscanf(argv[5], "%lf", &alpha) != 1) {
		printf("Error: incorrect kind of arguments for operation.\n");
		return 5;
	}
	// Checking for proper range of alpha value 
	if (alpha < 0 || alpha > 1) {
		printf("Error: invalid arguments for operation.\n");
		return 6;
	}

	// TODO calling blend function
	Image* imOut = alpha_blend(im1, im2, alpha);
	if (imOut == NULL) {
		destroy(im1);
		destroy(im2);
		return 8;
	}
	int check = output_image(imOut, argv);
	destroy(im1);
	destroy(im2);
	destroy(imOut);
	if (check != 0) {
		return check; 
	}
	return 0;

  }
  // Case: zoom_in function 
  else if (strcmp(argv[3], "zoom_in") == 0) {
	// Checking for proper # of args
	if (argc != 4) {
		printf("Error: incorrect number of arguments for specified operation.\n");
		return 5;
	}

	// TODO calling zoom_in function
	//*imOut = zoom_in(im1);

  }
  // Case: zoom_out function 
  else if (strcmp(argv[3], "zoom_out") == 0) {
	// Checkign for proper # of args
	if (argc != 4) {
		printf("Error: incorrect number of arguments for specified operation.\n");
		return 5;
	}
	// TODO calling zoom_out function
	//*imOut = zoom_out(im1);

  } 
  // Case: pointilism function
  else if (strcmp(argv[3], "pointilism") == 0) {
	// Checking for proper # of args
	if (argc != 4) {
		printf("Error: incorrect number of arguments for specified operation.\n");
		return 5;
	}
	// TODO calling pointilism function
	Image* imOut = pointilism(im1);
	if (imOut == NULL) {
		destroy(im1);
		return 8;
	}
	int check = output_image(imOut, argv);
	destroy(im1);
	destroy(imOut);
	if (check != 0) {
		return check; 
	}
	return 0;

  } 
  // Case: swirl function
  else if (strcmp(argv[3], "swirl") == 0) {
	// Checking for proper # of args
	if (argc != 7) {
		printf("Error: incorrect number of arguments for specified operation.\n");
		return 5;
	}
	// Obtaining coordinates
	int col, row;
	if (sscanf(argv[4], "%d", &col) != 1) {
		printf("Error: incorrect kind of arguments for operation.\n");
		return 5;
	}
	if (sscanf(argv[5], "%d", &row) != 1) {
		printf("Error: incorrect kind of arguments for operation.\n");
		return 5;
	}
	// Checking for valid coordinates
	if (col < 0 || col > im1->cols || row < 0 || row > im1->rows) {
		printf("Error: invalid arguments for operation.\n");
		return 6;
	}
	// Obtaining scale
	int scale; 
	if (sscanf(argv[6], "%d", &scale) != 1) {
		printf("Error: incorrect kind of arguments for operation.\n");
		return 5;
	}
	// Checking for valid scale
	if (scale <= 0) {
		printf("Error: invalid arguments for operation.\n");
		return 6;
	} 
	// TODO calling swirl function
	//*imOut = swirl(im1, col, row, scale);

  } 
  // Case: blur function
  else if (strcmp(argv[3], "blur") == 0) {
	// Checking for proper # of args
	if (argc != 5) {
		printf("Error: incorrect number of arguments for specified operation.\n");
		return 5;
	}
	// Obtaining radius
	double radius;
	if (sscanf(argv[4], "%lf", &radius) != 1) {
		printf("Error: incorrect kind of arguments for operation.\n");
		return 5;
	}
	// Checking for valid radius
	// TODO check for valid radius/sigma
	
	// TODO calling blur function
	Image* imOut = blur(im1, radius);
	if (imOut == NULL) {
		destroy(im1);
		return 8;
	}
	int check = output_image(imOut, argv);
	destroy(im1);
	destroy(imOut);
	if (check != 0) {
		return check; 
	}
	return 0;
 
  }
  // Case: invalid operation name
  else {
	printf("Error: no operation name specified or operation name specified was invalid.\n");
	return 4;
  }

return 0;
}


