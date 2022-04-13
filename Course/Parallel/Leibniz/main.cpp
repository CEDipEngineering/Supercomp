#include <omp.h>
#include <iostream>
#include <math.h>
#include <chrono>

#define N 10000000

int main(){

    auto begin_sequential = std::chrono::high_resolution_clock::now();
    double pi = 4.0;
    for(int i = 1; i<N; i++){
        pi += (std::pow(-1,i)*4)/(2.0*i+1.0);
    }
    auto end_sequential = std::chrono::high_resolution_clock::now();
    auto elapsed_sequential = std::chrono::duration_cast<std::chrono::nanoseconds>(end_sequential - begin_sequential);
    printf("PI is something like %.24f\n", pi);
    printf("Time measured: %.6f seconds.\n", elapsed_sequential.count() * 1e-9);

    auto begin_parallel = std::chrono::high_resolution_clock::now();
    pi = 4.0;
    int i;
    #pragma omp parallel for reduction(+:pi)
    for (i = 1; i<N; i++){
        pi += (std::pow(-1,i)*4)/(2.0*i+1.0);
    }
    auto end_parallel = std::chrono::high_resolution_clock::now();
    auto elapsed_parallel = std::chrono::duration_cast<std::chrono::nanoseconds>(end_parallel - begin_parallel);
    printf("PI is something like %.24f\n", pi);
    printf("Time measured: %.6f seconds.\n", elapsed_parallel.count() * 1e-9);
    return 0;
}