#include <thrust/device_vector.h>
#include <thrust/host_vector.h>
#include <thrust/transform_reduce.h>
#include <thrust/functional.h>
#include <thrust/transform.h>
#include <thrust/copy.h>
#include <thrust/fill.h>
#include <thrust/sequence.h>
#include <thrust/execution_policy.h>
#include <iostream>
#include <random>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
#include <cstdio>

#define MATCH 2
#define MISMATCH -1
#define GAP -1
#define SHOW_REPORT false
#define SHOW_MATRIX true

struct score {
    // Character of row;
    char y;
    // Constructor to store char internally
    score(char y) : y(y) { }
    __host__ __device__
    int operator()(const thrust::tuple<char, int, int> t){
        char x = t.get<0>();
        int H_up = t.get<1>();
        int H_diag = t.get<2>();
        // printf("Strings: %s;%s", a, b);
        int up, diag;
        // Branchless evaluation of three possibilities
        up = (H_up+GAP); // Above
        diag = (x != y)*(H_diag+MISMATCH) + (x == y)*(H_diag+MATCH); // Diag
        // printf("Eval: up=(%d); diag=(%c,%c,%d); gap=%d; match=%d; miss=%d;\n", H_up, x, y, H_diag, gap, match, miss);
        // Branchless evaluation of max
        int max = (up > diag)*up +
                  (diag >= up)*diag;
        max = (max>0)*max;
        return max;
    }
};

struct PrintVec {
    __host__ __device__
    int operator()(const int y){
        // Unpack to Sij, Si-1j
        printf("%d ", y);
        return y;
    }
};

struct line_update {
    __host__ __device__
    int operator()(const int& x, const int& y){
        // Unpack to Sij, Si-1j
        int curr = y;
        int left = x-1;
        // Hand compute max
        int max = (left >= curr)*left + (curr > left)*curr;
        max = (max>0)*max;
        // if(max != 0) printf("X = %d Y = %d LEFT = %d CURR = %d OUT = %d\n", x, y, left, curr, max);
        return max;
    }
};


int main(){

    // Inputs
    int len_a, len_b;
    std::cin >> len_a >> len_b;
    std::string a, b;
    std::cin >> a >> b;
    std::string big, small;
    a = "-" + a;
    b = "-" + b;

    if(len_a > len_b){
        big = a;
        small = b;
    } else {
        big = b;
        small = a;
    }

    auto begin_Parallel = std::chrono::high_resolution_clock::now();
    // Parallel

    // Create and fill both row vectors with zero
    thrust::device_vector<int> UpperRow(big.length());
    thrust::device_vector<int> Temp(big.length());
    thrust::fill(UpperRow.begin(), UpperRow.end(), 0);
    thrust::fill(Temp.begin(), Temp.end(), 0);

    // Send string to gpu vector    
    thrust::device_vector<char> big_str(big.length());
    for(int i = 0; i < big.length(); i++){
        big_str[i] = big[i];
    }

    int high_score = 0;
    int curr_score = 0;
    // Debug printing
    if (SHOW_MATRIX){
        thrust::transform(UpperRow.begin(), UpperRow.end(), UpperRow.begin(), PrintVec());
        std::cout << std::endl;
    }
    for(int i = 0; i<small.length()-1; i++){
        
        // First stage, check against above row
        thrust::transform(
            thrust::make_zip_iterator(thrust::make_tuple(big_str.begin()+1, UpperRow.begin()+1, UpperRow.begin())),
            thrust::make_zip_iterator(thrust::make_tuple(big_str.end(), UpperRow.end(), UpperRow.end()-1)),
            Temp.begin()+1, 
            score(small[i+1])
        );

        // Debug printing
        // std::cout << std::endl;
        // thrust::transform(Temp.begin(), Temp.end(), Temp.begin(), PrintVec());
        // std::cout << std::endl;

        // Second stage, check against self left 
        thrust::inclusive_scan(
            Temp.begin(),
            Temp.end(),
            UpperRow.begin(), 
            line_update()
        );
        
        // Debug printing
        if (SHOW_MATRIX){
            thrust::transform(UpperRow.begin(), UpperRow.end(), UpperRow.begin(), PrintVec());
            std::cout << std::endl;
        }

        // Keep track of best score
        curr_score = thrust::reduce(UpperRow.begin(), UpperRow.end(), 0, thrust::maximum<int>());
        if (curr_score > high_score) high_score = curr_score;
    }

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
        std::cout << "Score: " << high_score << std::endl;
    } else {
        std::cout << high_score << std::endl;
    }

    return 0;
}