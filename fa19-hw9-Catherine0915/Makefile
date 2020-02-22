CFLAGS=-Wall -Wno-unused-result -march=haswell -std=c99 -fopenmp -O3
DEBFLAGS=-g -Wall -Wno-unused-result -march=haswell -std=c99 -fopenmp -Og

all : benchmark benchmark_baseline

benchmark : benchmark.o mandelbrot.o
	gcc-9 $(CFLAGS) -o benchmark benchmark.o mandelbrot.o -lm

benchmark_baseline : benchmark.o mandelbrot_baseline.o
	gcc-9 $(CFLAGS) -o benchmark_baseline benchmark.o mandelbrot_baseline.o -lm

debug : clean benchmark.c mandelbrot.c parameters.h mandelbrot.h
	gcc-9 $(DEBFLAGS) -c mandelbrot.c
	gcc-9 $(DEBFLAGS) -c benchmark.c
	gcc-9 $(DEBFLAGS) -o benchmark benchmark.o mandelbrot.o -lm

benchmark.o : benchmark.c parameters.h
	gcc-9 $(CFLAGS) -c benchmark.c

mandelbrot.o : mandelbrot.c mandelbrot.h parameters.h
	gcc-9 $(CFLAGS) -c mandelbrot.c

mandelbrot_baseline.o : mandelbrot_baseline.c mandelbrot.h parameters.h
	gcc-9 $(CFLAGS) -c mandelbrot_baseline.c

clean:
	rm -f *.o
	rm -f benchmark
	rm -f benchmark_baseline
	rm -f out/*.ppm