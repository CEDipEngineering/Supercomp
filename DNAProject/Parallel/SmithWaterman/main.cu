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
    __host__ __device__
    int operator()(const thrust::tuple<char, char, int> t){
        char x = t.get<0>();
        char y = t.get<1>();
        int H = t.get<2>();
        // printf("Strings: %s;%s", a, b);
        int gap, match, miss;
        // Branchless evaluation of three possibilities
        gap = (x == '-' || y == '-');
        match = (x == y && !gap);
        miss = (x != y && !gap);
        printf("Eval: chars=(%c,%c,%d); gap=%d; match=%d; miss=%d;\n", x, y, H, gap, match, miss);
        int max = gap;
        if (match > max) max = match;
        if (miss > max) max = miss;
        if (max < 0) max = 0;
        return max;
    }
};

struct line_update {
    __host__ __device__
    int operator()(const thrust::tuple<int, int>& y){
        // Unpack to Sij, Si-1j
        int curr = y.get<0>();
        int left = y.get<1>()-1;
        // Hand compute max
        int max = curr;
        if (left > max) max = left;
        if (max < 0) max = 0;
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

    std::cout << "Big string: " << big << std::endl;
    std::cout << "Small string: " << small << std::endl;

    thrust::device_vector<int> UpperRow(big.length());
    thrust::device_vector<int> Temp(big.length());

    thrust::fill(UpperRow.begin(), UpperRow.end(), 0);
    thrust::fill(Temp.begin(), Temp.end(), 0);

    // thrust::counting_iterator<int> first(0);
    // thrust::counting_iterator<int> last = first + big.length();
    
    // thrust::transform(
    //     thrust::make_zip_iterator(thrust::make_tuple(UpperRow.begin(), UpperRow.begin()+1, first)), 
    //     thrust::make_zip_iterator(thrust::make_tuple(UpperRow.end()-1, UpperRow.end(), last)), 
    //     Temp.begin()+1, 
    //     line_process((big.c_str()), (small.c_str()))
    // );
    
    thrust::device_vector<char> big_str(big.length());
    for(int i = 0; i < big.length(); i++){
        big_str[i] = big[i];
    }
    thrust::device_vector<char> small_str(small.length());
    for(int i = 0; i < small.length(); i++){
        small_str[i] = small[i];
    }
    // thrust::generate(big_str.begin(), big_str.end(), thrust::make_constant_iterator<char>(22));
    
    
    thrust::transform(
        thrust::make_zip_iterator(thrust::make_tuple(big_str.begin(), small_str.begin(), UpperRow.begin())),
        thrust::make_zip_iterator(thrust::make_tuple(big_str.end(), small_str.end(), UpperRow.end())),
        big_str.begin(), 
        score()
    );
    std::cout << std::endl;


    // thrust::transform(thrust::make_counting_iterator(0), thrust::make_counting_iterator(3), X.begin(), score(const_cast<char *>(big.c_str()), const_cast<char *>(small.c_str())));


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