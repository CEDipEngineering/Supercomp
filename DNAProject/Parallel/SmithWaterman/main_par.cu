#include <iostream>
#include <random>
#include <vector>
#include <string>
#include <algorithm>
#include <omp.h>
#include <chrono>
#include <thrust/device_vector.h>
#include <thrust/host_vector.h>
#include <thrust/transform_reduce.h>
#include <thrust/functional.h>
#include <thrust/transform.h>
#define MATCH 2
#define MISMATCH -1
#define GAP -1
#define SHOW_REPORT false


int main(){

    // Inputs
    int len_a, len_b;
    std::cin >> len_a >> len_b;
    std::string a, b;
    std::cin >> a >> b;
    std::string big, small;

    if(len_a > len_b){
        big = a;
        small = b;
    } else {
        big = b;
        small = a;
    }

    auto begin_Parallel = std::chrono::high_resolution_clock::now();
    // Parallel



    // Output
    auto end_Parallel = std::chrono::high_resolution_clock::now();
    auto elapsed_Parallel = std::chrono::duration_cast<std::chrono::nanoseconds>(end_Parallel - begin_Parallel);
    if (SHOW_REPORT){
        std::cout << "Big string: " << big << std::endl;
        std::cout << "Small string: " << small << std::endl;

        std::cout << "=======================================" << std::endl;
        std::cout << "Parallel: " << std::endl;
        std::cout << "=======================================" << std::endl;
        printf("Time measured: %.6f seconds.\n", elapsed_Parallel.count() * 1e-9);
        std::cout << "Match string 1: " << best_a_vec_par[best_ix_par] << std::endl;
        std::cout << "Match string 2: " << best_b_vec_par[best_ix_par] << std::endl;
        std::cout << "Score: " << scores_par[best_ix_par] << std::endl;
    } else {
        std::cout << scores_par[best_ix_par] << std::endl;
    }

    return 0;
}