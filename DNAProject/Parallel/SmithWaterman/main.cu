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
#include <omp.h>
#include <chrono>
#include <cstdio>
#define MATCH 2
#define MISMATCH -1
#define GAP -1
#define SHOW_REPORT false

struct line_process { 
    // Access to both strings
    const char *a, *b;
    // Constructor to store them internally
    line_process(const char* a, const char* b) : a(a), b(b) { }
    __host__ __device__
    int operator()(const thrust::tuple<int, int, int>& y){
        int diag = y.get<0>();
        int up = y.get<1>();
        int i = y.get<2>();
        int j = y.get<2>();
        printf("i: %d; j: %d; up: %d; diag: %d;\n", i,j,up,diag);
        int gap, match, miss;
        // Branchless evaluation of three possibilities
        gap = (a[i] == '-' || b[j-1] == '-') * (up - 1);
        match = (a[i-1] == b[j-1]) * (diag + 2);
        miss = (a[i-1] != b[j-1]) * (diag - 1);
        // std::max not allowed, hand compute
        int max = gap;
        if (match > max) max = match;
        if (miss > max) max = miss;
        if (max < 0) max = 0;
        return max; // Can never be negative, since at least 1 of the above will be 0
    }
};

struct score {

    char y;
    // Constructor to store them internally
    score(char y) : y(y) { }
    __host__ __device__
    int operator()(const thrust::tuple<char, int, int> t){
        char x = t.get<0>();
        int H_up = t.get<1>();
        int H_diag = t.get<2>();
        // printf("Strings: %s;%s", a, b);
        int gap, match, miss;
        // Branchless evaluation of three possibilities
        gap = (y == '-')*(H_up+GAP);
        match = (x == y && !(y == '-'))*(H_diag+MATCH);
        miss = (x != y && !(y == '-'))*(H_diag+MISMATCH);
        printf("Eval: up=(%d); diag=(%c,%c,%d); gap=%d; match=%d; miss=%d;\n", H_up, x, y, H_diag, gap, match, miss);
        // Branchless evaluation of max
        int max = ((gap >= match) && (gap > miss))*gap +
                  ((match > gap) && (match >= miss))*match +
                  ((miss >= gap) && (miss > match))*miss;
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
        int max = (left > curr)*left + (curr > left)*curr;
        max = (max>0)*max;
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

    // std::cout << "Big string: \t\t" << big << std::endl;
    // std::cout << "Small string: \t\t" << small << std::endl;

    thrust::device_vector<int> UpperRow(big.length());
    thrust::device_vector<int> Temp(big.length());

    thrust::fill(UpperRow.begin(), UpperRow.end(), 0);
    thrust::fill(Temp.begin(), Temp.end(), 0);
    
    thrust::device_vector<char> big_str(big.length());
    for(int i = 0; i < big.length(); i++){
        big_str[i] = big[i];
    }
    // thrust::device_vector<char> small_str(small.length());
    // for(int i = 0; i < small.length(); i++){
    //     small_str[i] = small[i];
    // }

    int high_score = 0;
    int curr_score = 0;
    for(int i = 0; i<small.length()-1; i++){
        // First stage, check against above row
        thrust::transform(
            thrust::make_zip_iterator(thrust::make_tuple(big_str.begin()+1, UpperRow.begin()+1, UpperRow.begin())),
            thrust::make_zip_iterator(thrust::make_tuple(big_str.end(), UpperRow.end(), UpperRow.end()-1)),
            Temp.begin()+1, 
            score(small[i+1])
        );
        std::cout << std::endl;
        // thrust::transform(Temp.begin(), Temp.end(), Temp.begin(), PrintVec());
        // std::cout << std::endl;
        // Second stage, check against self left 
        thrust::inclusive_scan(
            Temp.begin(),
            Temp.end(),
            UpperRow.begin(), 
            line_update()
        );
        // thrust::transform(UpperRow.begin(), UpperRow.end(), UpperRow.begin(), PrintVec());
        // std::cout << std::endl;
        curr_score = thrust::reduce(UpperRow.begin(), UpperRow.end(), 0, thrust::maximum<int>());
        if (curr_score > high_score) high_score = curr_score;
    }
    std::cout << "Highest score was: " << high_score << std::endl;


   // Output
    auto end_Parallel = std::chrono::high_resolution_clock::now();
    auto elapsed_Parallel = std::chrono::duration_cast<std::chrono::nanoseconds>(end_Parallel - begin_Parallel);
    // if (SHOW_REPORT){
    //     std::cout << "Big string: " << big << std::endl;
    //     std::cout << "Small string: " << small << std::endl;

    //     std::cout << "=======================================" << std::endl;
    //     std::cout << "Parallel: " << std::endl;
    //     std::cout << "=======================================" << std::endl;
    //     printf("Time measured: %.6f seconds.\n", elapsed_Parallel.count() * 1e-9);
    //     std::cout << "Match string 1: " << best_a_vec_par[best_ix_par] << std::endl;
    //     std::cout << "Match string 2: " << best_b_vec_par[best_ix_par] << std::endl;
    //     std::cout << "Score: " << scores_par[best_ix_par] << std::endl;
    // } else {
    //     std::cout << scores_par[best_ix_par] << std::endl;
    // }

    return 0;
}