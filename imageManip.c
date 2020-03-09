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

  return NULL; // TODO remove stub
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


