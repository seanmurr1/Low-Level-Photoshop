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

  // Output image
  Image * imOut;

  // Checking operation
  int check = process_operation(argc, argv, im1, imOut);
  // Case: error detected
  if (check != 0) {
	// Returning proper error value
	// TODO free imOut and im1 and others if needed
	return check;
  } 

  // Outputting image
  // Attemping to open output file
  // TODO check if "b" is needed here
  FILE* output = fopen(argv[2], "wb");
  if (output == NULL) {
	printf("Error: output file could not be opened for writing.\n");
	// TODO free images
	return 7;
  }

  // Writing output image 
  int num_pixels_written = write_ppm(output, imOut);
  // Checking for success
  if (num_pixels_written != (imOut)->cols * (imOut)->rows) {
	printf("Error: writing output failed.\n");
	// TODO free images
	return 7;
  } 
  // TODO do we need more file checks, i.e. ferror?

  // Closing output file
  fclose(output);
  
  // TODO free images here

return 0;
}



/**
 * Processes given operation and errors, if any.
 *
 **/
int process_operation(int argc, char* argv[], Image * im1, Image * imOut) {

  // Case: exposure function
  if (strcmp(argv[3], "exposure") == 0) {
	// Checking for proper # of args
	if (argc != 5) {
		printf("Error: incorrect number of arguments for specified operation.\n");
		return 5;
	}
	// Obtaining EV value and checking for validity
	double EV;
	if (sscanf(argv[4], "%lf", &EV) != 1) {
		printf("Error: incorrect kind of arguments for specified opertaion.\n");
		return 5;
	}
	// Checking for proper range of EV value
	// TODO do we need epsilon value to compare doubles?
	if (EV > 3 || EV < -3) {
		printf("Error: invalid arguments for operation.\n ");
		return 6;
	}
	
	// TODO call exposure function  
	imOut = change_exposure(im1, EV);
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
	imOut = alpha_blend(im1, im2, alpha);

  }
  // Case: zoom_in function 
  else if (strcmp(argv[3], "zoom_in") == 0) {
	// Checking for proper # of args
	if (argc != 4) {
		printf("Error: incorrect number of arguments for specified operation.\n");
		return 5;
	}

	// TODO calling zoom_in function
	imOut = zoom_in(im1);

  }
  // Case: zoom_out function 
  else if (strcmp(argv[3], "zoom_out") == 0) {
	// Checkign for proper # of args
	if (argc != 4) {
		printf("Error: incorrect number of arguments for specified operation.\n");
		return 5;
	}
	// TODO calling zoom_out function
	imOut = zoom_out(im1);

  } 
  // Case: pointilism function
  else if (strcmp(argv[3], "pointilism") == 0) {
	// Checking for proper # of args
	if (argc != 4) {
		printf("Error: incorrect number of arguments for specified operation.\n");
		return 5;
	}
	// TODO calling pointilism function
	imOut = pointilism(im1);

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
	imOut = swirl(im1, col, row, scale);

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
	imOut = blur(im1, radius);
 
  }
  // Case: invalid operation name
  else {
	printf("Error: no operation name specified or operation name specified was invalid.\n");
	return 4;
  }

return 0;
}

/**
 *
 **/
Image * change_exposure(Image * im, double EV) {

return NULL; // TODO remove stub
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



