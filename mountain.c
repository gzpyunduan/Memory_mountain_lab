/* mountain.c - Generate the memory mountain. */
/* $begin mountainmain */
#include <stdlib.h>
#include <stdio.h>
#include "fcyc2.h" /* measurement routines */
#include "clock.h" /* routines to access the cycle counter */

#define MINBYTES (1 << 10)  /* Working set size ranges from 1 KB */
#define MAXBYTES (1 << 26)  /* ... up to 256 MB */
#define MAXSTRIDE 30       /* Strides range from 1 to 64 elems */
#define MAXELEMS MAXBYTES/sizeof(double)

/* $begin mountainfuns */
double data[MAXELEMS];      /* The global array we'll be traversing */

/* $end mountainfuns */
/* $end mountainmain */
void init_data(double *data, int n);
void test(int elems, int stride);
double run(int size, int stride, double Mhz);

/* $begin mountainmain */
int main()
{
    int size;        /* Working set size (in bytes) */
    int stride;      /* Stride (in array elements) */
    double Mhz;      /* Clock frequency */

    init_data(data, MAXELEMS); /* Initialize each element in data */
    Mhz = mhz(0);

    for (size = MAXBYTES; size >= MINBYTES; size >>= 1){
      for (stride = 1; stride <= MAXSTRIDE; stride = stride + 2){
        printf("%d %d %.1f\n", size, stride, run(size, stride, Mhz));
      }
    }
    exit(0);
}
/* $end mountainmain */

/* init_data - initializes the array */
void init_data(double *data, int n)
{
    int i;

    for (i = 0; i < n; i++)
	data[i] = i;
}

/* $begin mountainfuns */
/*
 * test - Iterate over first "elems" elements of array "data"
 *        with stride of "stride".
 */
void test(int elems, int stride) /* The test function */
{
    int i;
    double result = 0.0;
    volatile double sink;

    for (i = 0; i < elems; i += stride) {
	     result += data[i];
    }
    sink = result; /* So compiler doesn't optimize away the loop */
}

/*
 * run - Run test(elems, stride) and return read throughput (MB/s).
 *       "size" is in bytes, "stride" is in array elements, and
 *       Mhz is CPU clock frequency in Mhz.
 */
double run(int size, int stride, double Mhz)
{
    double cycles;
    int elems = size / sizeof(double);

    test(elems, stride);                     /* warm up the cache */       //line:mem:warmup
    cycles = fcyc2(test, elems, stride, 0);  /* call test(elems,stride) */ //line:mem:fcyc
    return (size / stride) / (cycles / Mhz); /* convert cycles to MB/s */  //line:mem:bwcompute
}
/* $end mountainfuns */
