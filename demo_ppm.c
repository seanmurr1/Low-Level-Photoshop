//demo_ppm.c

#include <stdio.h>
#include <stdlib.h>
#include "ppm_io.h"

int main() {

  // allocate space for an Image
  Image * im = malloc(sizeof(Image));
  if (!im) {
    fprintf(stderr, "Uh oh. Image allocation failed!\n");
    return 1;
  }

  // specify dimensions for our Image
  im->rows = 100;
  im->cols = 200;
  
  
  // allocate space for array of Pixels
  Pixel *pix = malloc(sizeof(Pixel) * im->rows * im->cols);
  if (!pix) {
    fprintf(stderr, "Uh oh. Pixel array allocation failed!\n");
    free(im);
    return 1;
  }

  // let data field of Image point to the new array
  im->data = pix;
  
  // fill Pixel array by setting all Pixels to same color
  Pixel my_color = {0, 127, 255}; //the color for most pixels
  Pixel white = {255, 255, 255}; //the color for a few pixels
  for (int r = 0; r < im->rows; r++) {
    for (int c = 0; c < im->cols; c++) {
      if ((r + c) % 8 != 0) { 
	im->data[(r * im->cols) + c] = my_color;
      } else {
	im->data[(r * im->cols) + c] = white;
      }
    }
  }

  // write image to disk
  FILE *fp = fopen("demo.ppm", "wb");
  if (!fp) {
    fprintf(stderr, "Uh oh. Output file failed to open!\n");
    free(im->data);
    free(im);
    return 1;
  }
  int num_pixels_written = write_ppm(fp, im);
  fclose(fp);
  printf("Image created with %d pixels.\n", num_pixels_written);
  
  // clean up!
  free(im->data);  // releases the pixel array
  free(im);        // releases the image itself

  return 0;

}
