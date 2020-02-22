/*********************
**  Mandelbrot fractal
** clang -Xpreprocessor -fopenmp -lomp -o Mandelbrot Mandelbrot.c
** by Dan Garcia <ddgarcia@cs.berkeley.edu>
** Modified for this class by Justin Yokota and Chenyu Shi
**********************/

#include <stdio.h>
#include <stdlib.h>
#include "ComplexNumber.h"
#include "Mandelbrot.h"
#include <sys/types.h>

/*
This function returns the number of iterations before the initial point >= the threshold.
If the threshold is not exceeded after maxiters, the function returns 0.
*/
u_int64_t MandelbrotIterations(u_int64_t maxiters, ComplexNumber * point, double threshold)
{
    u_int64_t count = 0;
    ComplexNumber* z = newComplexNumber(0, 0);
    while (count < maxiters) {
        ComplexNumber* zold = z;
        if (ComplexAbs(z) >= threshold) {
            freeComplexNumber(z);
            return count;
        }
        count += 1;
        z = ComplexSum(ComplexProduct(zold, zold), point);
        freeComplexNumber(zold);
    }
    freeComplexNumber(z);
    return 0;
}

/*
This function calculates the Mandelbrot plot and stores the result in output.
The number of pixels in the image is resolution * 2 + 1 in one row/column. It's a square image.
Scale is the the distance between center and the top pixel in one dimension.
*/
void Mandelbrot(double threshold, u_int64_t max_iterations, ComplexNumber* center, double scale, u_int64_t resolution, u_int64_t * output){
    u_int64_t* p = output;
    u_int64_t size = 2 * resolution + 1;
    u_int64_t col; u_int64_t row;
    double frac = (double) scale / resolution;
    ComplexNumber* point;
    for(int i = 0; i < size * size; i++) {
        col = i % size;
        row = i / size;
        point = newComplexNumber((int)(col - resolution) * frac + Re(center) , (int)(resolution - row) * frac + Im(center));
        *p = MandelbrotIterations(max_iterations, point, threshold);
        p++;
    }
    freeComplexNumber(point);
}


