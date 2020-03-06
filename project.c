#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "ppm_io.h"

int main(int argc, char* argv[]) {

  if (argc != 2) {
	printf("Invalid CL args\n");
	return 1;
  }
  FILE* input = fopen(argv[1], "r");
  if (input == NULL) {
	printf("Unable to open input file\n");
	return 1;
  }
  Image *im = read_ppm(input);
  if (im == NULL) {
	printf("Unable to process image\n");
	return 1;
  }
  FILE* output = fopen("output.ppm", "w");
  if (output == NULL) {
	printf("Unable to create output");
	return 1;
  }
  write_ppm(output, im);
  fclose(output);
  fclose(input);	
	






return 0;
}
