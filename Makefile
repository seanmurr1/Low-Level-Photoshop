CC=gcc
CFLAGS=-std=c99 -pedantic -Wall -Wextra -g


demo: demo_ppm.o ppm_io.o
	$(CC) demo_ppm.o ppm_io.o -o demo

demo_ppm.o: demo_ppm.c ppm_io.h
	$(CC) $(CFLAGS) -c demo_ppm.c

ppm_io.o: ppm_io.c ppm_io.h
	$(CC) $(CFLAGS) -c ppm_io.c

clean:
	rm -f *.o demo
