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

    // Parallel
    auto begin_Parallel = std::chrono::high_resolution_clock::now();
    std::string best_a_vec_par[NUM_EXP] = {""}, best_b_vec_par[NUM_EXP] = {""};
    int scores_par[NUM_EXP];
    #pragma omp parallel for
    for (int i = 0; i<NUM_EXP; i++){
        scores_par[i] = calculate_best_match(big, small, best_a_vec_par[i], best_b_vec_par[i]);
    }
    auto end_Parallel = std::chrono::high_resolution_clock::now();
    auto elapsed_Parallel = std::chrono::duration_cast<std::chrono::nanoseconds>(end_Parallel - begin_Parallel);

    // Gather best

    int best_ix_par = 0;
    int high_score_par = -999999;
    for(int i = 0; i<NUM_EXP; i++){
        if (scores_par[i] > high_score_par){
            high_score_par = scores_par[i];
            best_ix_par = i;
        }
    }

    // Output
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