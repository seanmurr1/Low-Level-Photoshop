CC=gcc
CFLAGS=-std=c99 -pedantic -Wall -Wextra -g

project: project.o ppm_io.o imageManip.o
	$(CC) project.o ppm_io.o imageManip.o -o project -lm

demo: demo_ppm.o ppm_io.o
	$(CC) demo_ppm.o ppm_io.o -o demo

project.o: project.c ppm_io.h imageManip.h
	$(CC) $(CFLAGS) -c project.c

demo_ppm.o: demo_ppm.c ppm_io.h
	$(CC) $(CFLAGS) -c demo_ppm.c

imageManip.o: imageManip.c imageManip.h ppm_io.h
	$(CC) $(CFLAGS) -c imageManip.c

ppm_io.o: ppm_io.c ppm_io.h
	$(CC) $(CFLAGS) -c ppm_io.c


clean:
	rm -f *.o demo project
