// Sean Murray
// JHED: smurra42
// Daniel Weber
// JHED: dweber11

// project.c

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "ppm_io.h"
#include "imageManip.h"

int main(int argc, char* argv[]) {
  // Processing inputs
  int check = process_input(argc, argv);
  // Checking for errors
  if (check != 0) {
	return check;
  }

  // Case: no errors
  return 0;
}
