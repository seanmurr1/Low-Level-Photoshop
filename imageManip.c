// Sean Murray
// smurra42
// Daniel Weber
//
//
// imageManip.c
//

#include "imageManip.h"
#include "ppm.io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
  // TODO probably make this into a function of its own "processOperation"
  if (strcmp(argv[3], "exposure") == 0) {
	// Checking for proper # of args
	if (argc != 5) {
		printf("Error: incorrect number of arguments or kind of arguments for specified operation\n");
		return 5;
	}
	// Obtaining EV value and checking for validity
	double EV;
	if (sscanf(argv[4], "%lf", &EV) != 1) {
		printf("Error: incorrect number of arguments or kind of arguments for specified opertaion.\n");
		return 5;
	}
	// Checking for proper range of EV value
	// TODO do we need epsilon value to compare doubles?
	if (EV > 3 || EV < -3) {
		printf("Error: invalid arguments for operation.\n ");
		return 6;
	}
	//
	// TODO call exposure function  
  }
  // Case: blend function 
  else if (strcmp(argv[3], "blend") == 0) {
	// Checking for proper # of args
	if (argc != 6) {
		printf("Error: incorrect number of arguments or kind of arguments for specified operation.\n");
		return 5;
	}
	// TODO rest of checks and calls
	


  }
  // Case: zoom_in function 
  else if (strcmp(argv[3], "zoom_in") == 0) {




  }
  // Case: zoom_out function 
  else if (strcmp(argv[3], "zoom_out") == 0) {




  } 
  // Case: pointilism function
  else if (strcmp(argv[3], "pointilism") == 0) {




  } 
  // Case: swirl function
  else if (strcmp(argv[3], "swirl") == 0) {




  } 
  // Case: blur function
  else if (strcmp(argv[3], "blur") == 0) {



 
  }
  // Case: invalid operation name
  else {
	printf("Error: no operation name specified or operation name specified was invalid.\n");
	return 4;
  }

  // TODO process outputting (probably with a function of its own


return 0;
}
