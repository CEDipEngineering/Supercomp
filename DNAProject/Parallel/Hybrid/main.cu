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
#include <omp.h>

#define MATCH 2
#define MISMATCH -1
#define GAP -1
#define SHOW_REPORT false
#define SHOW_MATRIX false

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

template <typename T>
struct PrintVec {
    __host__ __device__
    T operator()(const T y){
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

int smithwaterman_score_seq(
    thrust::detail::normal_iterator<thrust::device_ptr<int>> UpperRow_begin,
    thrust::detail::normal_iterator<thrust::device_ptr<int>> UpperRow_end,
    thrust::detail::normal_iterator<thrust::device_ptr<int>> Temp_begin,
    thrust::detail::normal_iterator<thrust::device_ptr<int>> Temp_end,
    thrust::detail::normal_iterator<thrust::device_ptr<char>> bigstr_begin,
    thrust::detail::normal_iterator<thrust::device_ptr<char>> bigstr_end,
    thrust::device_vector<char> smallstr,
    int smallStrStart, int smallStrEnd
) {
    thrust::fill(UpperRow_begin, UpperRow_end, 0);
    
    int high_score = 0;
    int curr_score = 0;
    // Debug printing
    if (SHOW_MATRIX){
        thrust::transform(UpperRow_begin, UpperRow_end, UpperRow_begin, PrintVec<int>());
        std::cout << std::endl;
    }
    for(int i = smallStrStart; i<smallStrEnd; i++){
        // First stage, check against above row
        thrust::transform(
            thrust::make_zip_iterator(thrust::make_tuple(bigstr_begin+1, UpperRow_begin+1, UpperRow_begin)),
            thrust::make_zip_iterator(thrust::make_tuple(bigstr_end, UpperRow_end, UpperRow_end-1)),
            Temp_begin+1, 
            score(smallstr[i])
        );

        // Debug printing
        // std::cout << std::endl;
        // thrust::transform(Temp_begin, Temp_end, Temp_begin, PrintVec<int>());
        // std::cout << std::endl;

        // Second stage, check against self left 
        thrust::inclusive_scan(
            Temp_begin,
            Temp_end,
            UpperRow_begin, 
            line_update()
        );
        
        // Debug printing
        if (SHOW_MATRIX){
            thrust::transform(UpperRow_begin, UpperRow_end, UpperRow_begin, PrintVec<int>());
            std::cout << std::endl;
        }

        // Keep track of best score
        curr_score = thrust::reduce(UpperRow_begin, UpperRow_end, 0, thrust::maximum<int>());
        if (curr_score > high_score) high_score = curr_score;
    }
    return high_score;
    // return 0;
}

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

    // Send string to gpu vector    
    thrust::device_vector<char> bigstr(big.length());
    for(int i = 0; i < big.length(); i++){
        bigstr[i] = big[i];
    }

    thrust::device_vector<char> smallstr(small.length());
    for(int i = 0; i < small.length(); i++){
        smallstr[i] = small[i];
    }

    thrust::device_vector<int> UpperRow(big.length());
    thrust::device_vector<int> Temp(big.length());
    
    int curr_size;
    int smallStringStart, smallStringEnd, bigStringStart, bigStringEnd;
    int high_score = MISMATCH * len_a * len_b; 
    int score = 0;
    #pragma omp parallel for reduction(max:high_score)
    for(curr_size = small.length()-1; curr_size>0; curr_size--){
        for (int i = 1; i<small.length()-curr_size+1; i++){
            for (int j = 1; j<big.length()-curr_size+1; j++){
                smallStringStart = i;
                smallStringEnd = i + curr_size;
                bigStringStart = j;
                bigStringEnd = j + curr_size;
                score = smithwaterman_score_seq(
                    UpperRow.begin(), UpperRow.begin()+curr_size, 
                    Temp.begin(), Temp.begin()+curr_size,
                    bigstr.begin() + bigStringStart, bigstr.begin() + bigStringEnd,
                    smallstr, smallStringStart, smallStringEnd
                );
                high_score = score > high_score ? score : high_score;
            }
        }
        curr_size = 0;
        curr_size --;
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