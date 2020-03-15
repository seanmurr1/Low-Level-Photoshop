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

  return NULL; // TODO remove stub
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
	int number_of_changed_pixels = number_of_pixels * .03;
	int * center_pixel_indices = generate_rand_num_arr(number_of_pixels, number_of_changed_pixels);
	for (int curr_pixel = 0; curr_pixel < number_of_changed_pixels; curr_pixel++) {
		int radius = rand() % 5 + 1;
		for (int j = 1; j <= radius; j++) {
			if((center_pixel_indices[curr_pixel] + j > number_of_pixels) || (center_pixel_indices[curr_pixel] - j < 0)) {
				return im_out;
			}
			(im_out->data[center_pixel_indices[curr_pixel] + j]) = im_out->data[center_pixel_indices[curr_pixel]];
			im_out->data[center_pixel_indices[curr_pixel] - j] = im_out->data[center_pixel_indices[curr_pixel]];
		}
	}
  return im_out; // TODO remove stub
}
int * generate_rand_num_arr(int max_value, int number_to_generate) {
	int * randum_num_arr = (int *) malloc(number_to_generate * sizeof(int));
	char * is_used = (char *) calloc(max_value, sizeof(char)); /* flags */
	int in, im;
	im = 0;

	for (in = max_value - number_to_generate; in < max_value && im < number_to_generate; ++in) {
	int r = rand() % (in + 1); /* generate a random number 'r' */

	if (is_used[r])
		/* we already have 'r' */
		r = in; /* use 'in' instead of the generated number */
	randum_num_arr[im++] = r + 1; /* +1 since your range begins from 1 */
	is_used[r] = 1;
	}
	assert(im = number_to_generate);
	free(is_used);
	return randum_num_arr;
}
int row_col_ind_to_row_ind(int row, int column, int num_cols) {
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

  return NULL; // TODO remove stub
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
	//*imOut = alpha_blend(im1, im2, alpha);

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
	//*imOut = blur(im1, radius);
 
  }
  // Case: invalid operation name
  else {
	printf("Error: no operation name specified or operation name specified was invalid.\n");
	return 4;
  }

return 0;
}


