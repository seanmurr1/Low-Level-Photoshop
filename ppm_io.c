
// Sean Murray
// smurra42
// Daniel Weber
//
//
// ppm_io.c
// 601.220, Spring 2019
//

#include <assert.h>
#include "ppm_io.h"
#include <stdlib.h>



/* Read a PPM-formatted image from a file (assumes fp != NULL).
 * Returns the address of the heap-allocated Image struct it
 * creates and populates with the Image data.
 */
Image * read_ppm(FILE *fp) {

  // check that fp is not NULL
  assert(fp); 
  // Tag
  char tag1;
  char tag2;
  // Taking in Tag
  fscanf(fp, "%c%c", &tag1, &tag2);
  // Checking for proper PPM tag
  if (!(tag1 == 'P' && tag2 == '6')) {
	fprintf(stderr, "1Invalid PPM file.\n");
	return NULL;
  }
 
  int row;
  int col;
  int colors;
  char input;

  // Taking in newline char and #col
  // If this fails, then there is a comment
  if (fscanf(fp, "%c%d", &input, &col) != 2) {
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
	fprintf(stderr, "2Invalid PPM file.\n");
	return NULL;
  }
  // Taking in last whitespace char before binary data
  fscanf(fp, "%c", &input);
  
  // At this point, actual image data starts
  // Allocating space for image with correct dimensions
  Image * im = (Image *) malloc(sizeof(Image));
  im->rows = row;
  im->cols = col;
  im->data = (Pixel *) malloc(sizeof(Pixel) * row * col);
  
  // Reading image data
  int numRead = fread(im->data, sizeof(Pixel), row * col, fp);
  // Checking for proper reading 
  if (numRead != row * col) {
	fprintf(stderr, "3Failed to read image data.\n");
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

