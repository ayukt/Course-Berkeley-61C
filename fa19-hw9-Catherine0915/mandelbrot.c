// Include SSE intrinsics
#if defined(_MSC_VER)
#include <intrin.h>
#elif defined(__GNUC__) && (defined(__x86_64__) || defined(__i386__))
#include <immintrin.h>
#include <x86intrin.h>
#endif

// Include OpenMP
#include <omp.h>

#include "mandelbrot.h"
#include "parameters.h"



void mandelbrot(struct parameters params, double scale, int32_t *num_pixels_in_set) {
    int32_t num_zero_pixels = 0;
    double center_real = creal(params.center);
    double center_imag = cimag(params.center);
    __m256d _2 = _mm256_set1_pd(2.0);
    __m256d threshold = _mm256_set1_pd(params.threshold * params.threshold);
    #pragma omp parallel
    {
    omp_set_num_threads(8);
    __m256d vec_real;
    __m256d vec_imag;
    __m256d z_real;
    __m256d z_imag;
    __m256d new_z_real;
    int temp = 0;
    int track1;
    int track2;
    int track3;
    int track4;
    int flag;
    double z2_real;
    double z2_imag;
    double new_z2_real;
    #pragma omp for reduction (+: num_zero_pixels)
    for (int i = params.resolution; i >= -params.resolution; i--) {
        vec_imag = _mm256_set1_pd(i * scale / params.resolution + center_imag);
        for (int j = -params.resolution; j <= params.resolution - 3; j += 4) {
            z_real = _mm256_set1_pd(0.0);
            z_imag = _mm256_set1_pd(0.0);
            flag = 1;
            track1 = 1;
            track2 = 1;
            track3 = 1;
            track4 = 1;
            vec_real = _mm256_set_pd((center_real + j * scale / params.resolution),
                    (center_real + (j + 1) * scale / params.resolution),
                    (center_real + (j + 2) * scale / params.resolution),
                    (center_real + (j + 3) * scale / params.resolution));
            for (int k = 1; k <= params.maxiters; k++) {
                new_z_real = _mm256_add_pd(_mm256_sub_pd(_mm256_mul_pd(z_real, z_real), _mm256_mul_pd(z_imag, z_imag)), vec_real);
                z_imag = _mm256_add_pd(_mm256_mul_pd(_2, _mm256_mul_pd(z_real, z_imag)), vec_imag);
                z_real = new_z_real; 
                temp = _mm256_movemask_pd(_mm256_sub_pd(_mm256_add_pd(_mm256_mul_pd(z_real, z_real), _mm256_mul_pd(z_imag, z_imag)), threshold));
                if (!(temp & 1)) {
                    track4 = 0;
                }
                if (!(temp >> 1 & 1)) {
                    track3 = 0;
                }
                if (!(temp >> 2 & 1)) {
                    track2 = 0;
                }
                if (!(temp >> 3 & 1)) {
                    track1 = 0;
                }
                if (!(track1 || track2 ||track3 || track4)) {
                    break;
                }
            }  
            num_zero_pixels += track1 + track2 + track3 + track4;
        }

        for (int j = -params.resolution + (2 * params.resolution) / 4 * 4; j <= params.resolution; j++) {
            z2_real = 0;
            z2_imag = 0;
            flag = 1;
            for (int k = 1; k <= params.maxiters; k++) {
                new_z2_real = z2_real * z2_real - z2_imag * z2_imag + center_real + j * scale / params.resolution;
                z2_imag = 2 * z2_imag * z2_real + center_imag + i * scale / params.resolution;
                z2_real = new_z2_real;

                if (z2_real * z2_real + z2_imag * z2_imag >= params.threshold * params.threshold) {
                    flag = 0;
                    break;
                }
            }
            if (flag) {
                num_zero_pixels++;
            }
        }
    }
    }
    *num_pixels_in_set = num_zero_pixels;
}