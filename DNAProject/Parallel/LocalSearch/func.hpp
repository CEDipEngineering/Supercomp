#include <iostream>
#include <random>
#include <vector>
#include <string>
#include <algorithm>
#include <omp.h>
#include <chrono>
#define MATCH 2
#define MISMATCH -1
#define GAP -1
#define NUM_EXP 50
#define SHOW_REPORT false

int match_score(std::string a, std::string b){
    // a and b must be of equal size
    int score = 0;
    int size = a.length();
    for (int i = 0; i<size; i++){
        if (a[i] == '-' || b[i] == '-'){
            score += GAP;
        } else if (a[i] == b[i]) {
            score += MATCH;
        } else {
            score += MISMATCH;
        }
    }
    return score;
}

int calculate_best_match(std::string big, std::string small, std::string &best_a, std::string &best_b){
    int len_a = (int) big.size();
    int len_b = (int) small.size();

    const int NUM_EXPERIMENTS = (int) std::sqrt(len_a*len_b*2);
    int high_score = -9999999; 

    // RNG
    std::random_device generator;
    std::uniform_int_distribution<int> subseq_size_distro(1,len_b);
    std::uniform_int_distribution<int> p_distro(20, len_a*2); // Run enough times to probably try most combinations

    for(int _ = 0; _<NUM_EXPERIMENTS; _++){

        const int subseq_size = subseq_size_distro(generator);        
        const int p = p_distro(generator);

        // Random subsize
        // std::cout << "Subsequence size: " << subseq_size << std::endl;
        std::uniform_int_distribution<int> index_small_distro(0,len_b-subseq_size);
        std::uniform_int_distribution<int> index_big_distro(0,len_a-subseq_size);

        // Assemble substring b
        int i = index_small_distro(generator);
        std::string base = small.substr(i, subseq_size);

        // Try p combinations
        int max_score = len_a*(-3); // Pior score possível é tudo em MISMATCH/GAP, dependendo dos pesos


        std::string match;
        int ix = 0;
        int score = -999999;
        for (int i = 0; i<p; i++){
            ix = index_big_distro(generator);
            score = match_score(base, big.substr(ix, subseq_size));
            // If its the best for this size so far
            if (score > max_score){
                max_score = score;
                match = big.substr(ix, subseq_size);
            }
        }
        // if this score beats best known score so far;
        if (score>high_score){
            high_score = score;
            best_a = base;
            best_b = match;
        }
    }
    return high_score;
}