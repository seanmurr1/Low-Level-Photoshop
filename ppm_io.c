// Sean Murray
// JHED: smurra42
// Daniel Weber
// JHED: dweber11
//
// ppm_io.c
// 601.220, Spring 2019


#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ppm_io.h"
#include "imageManip.h"



/* Read a PPM-formatted image from a file (assumes fp != NULL).
 * Returns the address of the heap-allocated Image struct it
 * creates and populates with the Image data.
 */
Image * read_ppm(FILE *fp) {

  // check that fp is not NULL
  assert(fp); 
  // Tag
  char* tag = (char*) malloc(sizeof(char));
  size_t size = sizeof(char);
  // Taking in Tag
  getline(&tag, &size, fp);
  // Checking for proper PPM tag
  if (strcmp("P6\n", tag) != 0) {
	fprintf(stderr, "Error: invalid PPM file.\n");
	free(tag);
	return NULL;
  }
  free(tag);
 
  int row;
  int col;
  int colors;
  char input;

  // Taking in #col
  // If this fails, then there is a comment
  if (fscanf(fp, "%d", &col) != 1) {
	// Iterating through comment
	do {
		fscanf(fp, "%c", &input);
	} while (input != '\n'); 
	// Taking in correct integer for #col
	fscanf(fp, "%d", &col);
  }

  // Obtaining #row and #colors
  fscanf(fp, "%d%d", &row, &colors);

  // Checking for proper format
  if (row <= 0 || col <= 0 || colors != 255) {
	fprintf(stderr, "Error: invalid PPM file.\n");
	return NULL;
  }
  // Taking in last whitespace char before binary data
  fscanf(fp, "%c", &input);
  
  // At this point, actual image data starts
  // Allocating space for image with correct dimensions
  Image * im = (Image *) malloc(sizeof(Image));
  // Checking for success
  if (im == NULL) {
	fprintf(stderr, "Error: could not allocate data for image.\n");
	return NULL;
  }
  // Inputting data
  im->rows = row;
  im->cols = col;
  // Allocating space for Pixel array
  im->data = (Pixel *) malloc(sizeof(Pixel) * row * col);
  // Checking for success
  if (im->data == NULL) {
	fprintf(stderr, "Error: could not allocate data for image.\n");
	free(im);
	return NULL;
  }
  // Reading image data
  int numRead = fread(im->data, sizeof(Pixel), row * col, fp);
  // Checking for proper reading 
  if (numRead != row * col) {
	fprintf(stderr, "Error: failed to read image data.\n");
	destroy(im);
	return NULL;
  }

  // Returning populated Image
  return im;
  
}


/* Write a PPM-formatted image to a file (assumes fp != NULL),
 * and return the number of pixels successfully written.
 */
int write_ppm(FILE *fp, const Image *im) {

  // check that fp is not NULL
  assert(fp); 

  // write PPM file header, in the following format
  // P6
  // cols rows
  // 255
  fprintf(fp, "P6\n%d %d\n255\n", im->cols, im->rows);

  // now write the pixel array
  int num_pixels_written = fwrite(im->data, sizeof(Pixel), im->cols * im->rows, fp);

  if (num_pixels_written != im->cols * im->rows) {
    fprintf(stderr, "Uh oh. Pixel data failed to write properly!\n");
  }

  return num_pixels_written;
}

/* Copies a given image. New image is located on heap with exact values of old image.
 * Returns pointer to copied image.
 */
Image * copy_image(Image * im) {
	// Allocating space for new Image
	Image * imOut = (Image *) malloc(sizeof(Image));
	// Checking for success
	if (imOut == NULL) {
		fprintf(stderr, "Error: could not allocate data for image.\n");
		return NULL;
	}
	// Copying row and col data
	imOut->rows = im->rows;
	imOut->cols = im->cols;
	// Allocating space for array of Pixels
	imOut->data = (Pixel *) malloc(sizeof(Pixel) * imOut->rows * imOut->cols);
	// Checking for success
	if (imOut->data == NULL) {
		fprintf(stderr, "Error: could not allocate data for image.\n");
		free(imOut);
		return NULL;
	}
	// Copying pixel data
	for (int i = 0; i < im->rows * im->cols; i++) {
		imOut->data[i] = im->data[i];
	}
	// Returning copied image
	return imOut;
}

/* Outputs given image to ppm file with corresponding output file name from argv.
 * Returns corresponding error value (0 if no errors).
 */
int output_image(Image * im, char* argv[]) {
	// Attempting to open output file
	FILE* output = fopen(argv[2], "wb");
	if (output == NULL) {
		fprintf(stderr, "Error: output file could nto be opened for writing.\n");
		return 7;
	}
	// Writing output image
	int num_pixels_written = write_ppm(output, im);
	// Checking for errors
	if (num_pixels_written != im->cols * im->rows) {
		fprintf(stderr, "Error: writing output failed.\n");
		fclose(output);
		return 7;
	}
	
	// Closing file
	fclose(output);
	
	return 0;
}

/* Frees a given image (first freeing its data).
 */
void destroy(Image * im) {
	free(im->data);
	free(im);
}

/* Creates an image located on the heap with inputted dimensions
 * Returns pointer to created image.
 */
Image * create_image(int cols, int rows) {
	// Creating space for image
	Image * im = (Image *) malloc(sizeof(Image));
	// Checking for success
	if (im == NULL) {
		fprintf(stderr, "Error: could not allocate memory for image.\n");
		return NULL;
	}
	// Updating data
	im->cols = cols;
	im->rows = rows;
	im->data = (Pixel *) malloc(sizeof(Pixel) * cols * rows);
	// Checking for success
	if (im->data == NULL) {
		fprintf(stderr, "Error: could not allocate memory for image.\n");
		free(im);
		return NULL;
	}
	// Returning image
	return im;
}

/* Processes command-line arguments and handles errors.
 * Opens and reads input image and then processes given operation.
 * Returns error value. 
 */
int process_input(int argc, char* argv[]) {

  // Checking if input and output filenames exist
  if (argc < 3) {
	printf("Failed to supply input filename or output filename, or both.\n");
	return 1;
  }
  else if (argc < 4) {
	  printf("No operation name was specified, or operation name specified was invalid.\n");
	  return 4;
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
	return check;
  }

return 0;
}

/* Processes given operation and errors, if any.
 * Returns error value.
 */
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
	if (EV > 3 || EV < -3) {
		printf("Error: invalid arguments for operation.\n ");
		destroy(im1);
		return 6;
	}
	
	// Changing exposure  
	Image * imOut = change_exposure(im1, EV);
	// Checking for errors
	if (imOut == NULL) {
		destroy(im1);
		return 8;
	}
	// Outputting image
	int check = output_image(imOut, argv);
	// Freeing memory
	destroy(im1);
	destroy(imOut);
	// Checking for errors
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

	// Blending images
	Image* imOut = alpha_blend(im1, im2, alpha);
	// Checking for errors
	if (imOut == NULL) {
		destroy(im1);
		destroy(im2);
		return 8;
	}
	// Outputting image
	int check = output_image(imOut, argv);
	// Freeing memory
	destroy(im1);
	destroy(im2);
	destroy(imOut);
	// Checking for errors
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
	// Performing function
	Image * imOut = zoom_in(im1);
	// Checking for errors
	if (imOut == NULL) {
		destroy(im1);
		return 8;
	}
	// Outputting image
	int check = output_image(imOut, argv);
	// Freeing memory
	destroy(im1);
	destroy(imOut);
	// Checking for errors
	if (check != 0) {
		return check;
	}
	return 0;

  }
  // Case: zoom_out function 
  else if (strcmp(argv[3], "zoom_out") == 0) {
	// Checking for proper # of args
	if (argc != 4) {
		printf("Error: incorrect number of arguments for specified operation.\n");
		return 5;
	}
	// Performing function
	Image * imOut = zoom_out(im1);
	// Checking for errors
	if (imOut == NULL) {
		destroy(im1);
		return 8;
	}
	// Outputting image
	int check = output_image(imOut, argv);
	// Freeing memory
	destroy(im1);
	destroy(imOut);
	// Checking for errors
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
	// Calling pointilism function
	Image* imOut = pointilism(im1);
	// Checking for errors
	if (imOut == NULL) {
		destroy(im1);
		return 8;
	}
	// Outputting image
	int check = output_image(imOut, argv);
	// Freeing memory
	destroy(im1);
	// Only need to destroy im1 as im1 and imOut point to same image	
	// Checking for errors
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
	if (col < 0 || col > im1->cols - 1 || row < 0 || row > im1->rows - 1) {
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
	// Performing swirl function
	Image * imOut = swirl(im1, col, row, scale);
	// Checking for errors
	if (imOut == NULL) {
		destroy(im1);
		return 8;
	}
	// Outputting image
	int check = output_image(imOut, argv);
	// Freeing memory
	destroy(im1);
	destroy(imOut);
	// Checking for errors
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
		return 6;
	}
	// Checking for valid radius 
	if (radius > 0) {
		// Blurring
		Image* imOut = blur(im1, radius);
		// Checking for errors
		if (imOut == NULL) {
			destroy(im1);
			return 8;
		}
		// Outputting image
		int check = output_image(imOut, argv);
		// Freeing memory
		destroy(im1);
		destroy(imOut);
		// Checking for errors
		if (check != 0) {
			return check; 
		}
		return 0;
	} else {
		printf("Error: Invalid arguments for operation.\n");
		return 6;
	}
	
 
  }
  // Case: invalid operation name
  else {
	printf("Error: no operation name specified or operation name specified was invalid.\n");
	destroy(im1);
	return 4;
  }

return 0;
}
