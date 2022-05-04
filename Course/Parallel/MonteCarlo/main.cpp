#include <iostream>
#include <random>
#include <omp.h>
#include <chrono>
#include <vector>

#define N 10000000

int main(){

    std::cout << "Calculating PI using Monte Carlo!" << std::endl;

    // Sequential
    std::random_device generator;
    std::uniform_real_distribution<float> point_distro(0,1);
    auto begin_sequential = std::chrono::high_resolution_clock::now();
    float x,y;
    long sum = 0;
    for(int i = 0; i<N; i++){
        x = point_distro(generator);
        y = point_distro(generator);
        if((x*x + y*y) <= 1) sum += 4; 
    }
    auto end_sequential = std::chrono::high_resolution_clock::now();
    auto elapsed_sequential = std::chrono::duration_cast<std::chrono::nanoseconds>(end_sequential - begin_sequential);
    printf("PI is something like %.24f\n", (double) sum/N);
    printf("Time measured: %.6f seconds.\n", elapsed_sequential.count() * 1e-9);

    // Parallel
    std::vector<std::default_random_engine> engines;
    for (int i = 0, n = omp_get_max_threads(); i < n; i++) {
        engines.emplace_back(std::default_random_engine(generator()));
    }
    auto begin_parallel = std::chrono::high_resolution_clock::now();
    sum = 0;
    #pragma omp parallel for reduction(+:sum)
    for(int i = 0; i<N; i++){
        std::default_random_engine& g = engines[omp_get_thread_num()];
        x = point_distro(g);
        y = point_distro(g);
        if((x*x + y*y) <= 1) sum += 4; 
    }
    auto end_parallel = std::chrono::high_resolution_clock::now();
    auto elapsed_parallel = std::chrono::duration_cast<std::chrono::nanoseconds>(end_parallel - begin_parallel);
    printf("PI is something like %.24f\n", (double) sum/N);
    printf("Time measured: %.6f seconds.\n", elapsed_parallel.count() * 1e-9);  

    return 0;
}