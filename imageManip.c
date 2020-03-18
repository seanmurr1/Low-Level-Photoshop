// Sean Murray
// smurra42
// Daniel Weber
//
//
// imageManip.c
//

#include "imageManip.h"
#include "ppm_io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>



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


void expand_pixel(Image * im1, Image * imOut, int index) {
	// Number of pixels per row
	int col = im1->cols;
	// Expanding pixel to 2x2 square
	imOut->data[2 * index + (index / col) * 2 * col] = im1->data[index];
	imOut->data[2 * index + 1 + (index / col) * 2 * col] = im1->data[index];
	imOut->data[2 * index + 2 * col + (index / col) * 2 * col] = im1->data[index];
	imOut->data[2 * index + 2 * col + 1 + (index / col) * 2 * col] = im1->data[index];
}


/**
 *
 **/
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


/**
 *
 **/
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

/**
 *
 **/
Image * pointilism(Image * im) {

  return NULL; // TODO remove stub
}

int to_x_coord(int i, int num_cols) {
	// Finding x coord
	return i % num_cols;
}

int to_y_coord(int i, int num_cols) {
	// Finding y coord
	return i / num_cols;
}

int coord_to_index(int x, int y, int num_cols) {
	// Calculating index
	return (y * num_cols) + x;
}

double calc_alpha(int x, int y, int cX, int cY, int s) {
	// Calculating alpha
	return (double) (sqrt(pow(x - cX, 2.0) + pow(y - cY, 2.0)) / s);

}

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

/**
 *
 *
 **/

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
	Image * imOut = zoom_in(im1);
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
  // Case: zoom_out function 
  else if (strcmp(argv[3], "zoom_out") == 0) {
	// Checkign for proper # of args
	if (argc != 4) {
		printf("Error: incorrect number of arguments for specified operation.\n");
		return 5;
	}
	// TODO calling zoom_out function
	Image * imOut = zoom_out(im1);
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
  // Case: pointilism function
  else if (strcmp(argv[3], "pointilism") == 0) {
	// Checking for proper # of args
	if (argc != 4) {
		printf("Error: incorrect number of arguments for specified operation.\n");
		return 5;
	}
	// TODO calling pointilism function
	//*imOut = pointilism(im1);

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
	Image * imOut = swirl(im1, col, row, scale);
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


