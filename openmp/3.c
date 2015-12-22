#include "omp.h"
#include "rdtsc.h"
#include <stdio.h>

#define THREADS 16
#define ITERATIONS 10000
#define ITERATIONS_IN_BLOCK 100

int main(){
    unsigned long long start, finish;
    int a;
    omp_set_num_threads(THREADS);
    start = rdtsc();

    #pragma omp parallel for schedule (static, ITERATIONS_IN_BLOCK)
        for (int i = 0; i < ITERATIONS; i++) 
            a = 1;
    
    finish = rdtsc();
    printf("static:  %llu\n", finish - start);
    
    
    start = rdtsc();
    
    #pragma omp parallel for schedule (dynamic, ITERATIONS_IN_BLOCK)
        for (int i = 0; i < ITERATIONS; i++) 
            a = 1;

    finish = rdtsc();
    printf("dynamic: %llu\n", finish - start);

    return 0;
}

/*
static:  1223296
dynamic: 738952
*/