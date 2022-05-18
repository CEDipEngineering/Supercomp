#include "func.hpp"

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


    // Sequential
    auto begin_sequential = std::chrono::high_resolution_clock::now();
    std::vector<std::string> best_a_vec_seq, best_b_vec_seq;
    best_a_vec_seq.reserve(NUM_EXP);
    best_b_vec_seq.reserve(NUM_EXP);
    int scores_seq[NUM_EXP];
    for (int i = 0; i<NUM_EXP; i++){
        scores_seq[i] = calculate_best_match(big, small, best_a_vec_seq[i], best_b_vec_seq[i]);
    }
    auto end_sequential = std::chrono::high_resolution_clock::now();
    auto elapsed_sequential = std::chrono::duration_cast<std::chrono::nanoseconds>(end_sequential - begin_sequential);
    
    // Gather best

    int best_ix_seq = 0;
    int high_score_seq = -999999;
    for(int i = 0; i<NUM_EXP; i++){
        if (scores_seq[i] > high_score_seq){
            high_score_seq = scores_seq[i];
            best_ix_seq = i;
        }
    }

    // Output

    if(SHOW_REPORT){   
        std::cout << "Big string: " << big << std::endl;
        std::cout << "Small string: " << small << std::endl;

        std::cout << "=======================================" << std::endl;
        std::cout << "Sequential: " << std::endl;
        std::cout << "=======================================" << std::endl;
        printf("Time measured: %.6f seconds.\n", elapsed_sequential.count() * 1e-9);
        std::cout << "Match string 1: " << best_a_vec_seq[best_ix_seq] << std::endl;
        std::cout << "Match string 2: " << best_b_vec_seq[best_ix_seq] << std::endl;
        std::cout << "Score: " << scores_seq[best_ix_seq] << std::endl;
    } else {
        std::cout << scores_seq[best_ix_seq] << std::endl;
    }

    return 0;
}