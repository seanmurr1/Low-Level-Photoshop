// Sean Murray
// JHED: smurra42
// Daniel Weber
// JHED: dweber11

// ppm_io.h
// 601.220, Spring 2019
// Starter code for midterm project - feel free to edit/add to this file


#ifndef PPM_IO_H
#define PPM_IO_H

#include <stdio.h>

/* A struct to store a single RGB pixel, one byte per color channel.
 */
typedef struct _pixel {
  unsigned char r;  //red   channel value
  unsigned char g;  //green channel value
  unsigned char b;  //blue  channel value
} Pixel;


/* A struct to bundle together a pixel array with the other
 * image data we'll frequently want to pass around with it.
 * (This saves us from having to pass the same three 
 * variables to every function.) Note that no Pixels are
 * stored within this struct; the data field is a pointer.
 */
typedef struct _image {
  Pixel *data;  //pointer to array of Pixels
  int rows;     //number of rows of Pixels
  int cols;     //number of columns of Pixels
} Image;


/* Read a PPM-formatted image from a file (assumes fp != NULL).
 * Returns the address of the heap-allocated Image struct it
 * creates and populates with the Image data.
 */
Image * read_ppm(FILE *fp);


/* Write a PPM-formatted image to a file (assumes fp != NULL),
 * and return the number of pixels successfully written.
 */
int write_ppm(FILE *fp, const Image *im);

/* Copies a given image. New image is located on heap with exact values of old image.
 * Returns pointer to copied image.
 */
Image * copy_image(Image * im);

/* Outputs given image to ppm file with corresponding output file name from argv.
 * Returns corresponding error value (0 if no errors).
 */
int output_image(Image * im, char* argv[]);

/* Frees a given image (first freeing its data).
 */
void destroy(Image * im);

/* Creates an image located on the heap with inputted dimensions
 * Returns pointer to created image.
 */
Image * create_image(int cols, int rows);

/* Processes command-line arguments and handles errors.
 * Opens and reads input image and then processes given operation.
 * Returns error value. 
 */
int process_input(int argc, char* argv[]);

/* Processes given operation and errors, if any.
 * Returns error value.
 */
int process_operation(int argc, char* argv[], Image * im1);

#endif
