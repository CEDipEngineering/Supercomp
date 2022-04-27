#include <iostream>
#include <random>
#include <vector>
#include <string>
#include <algorithm>

#define MATCH 2
#define MISMATCH -1
#define GAP -1

#define SHOW_REPORT false

int match_score(std::string sb, std::string a, int start, int subseq_size){
    int score = 0;
    char sa;
    for(int i = 0; i<subseq_size; i++){
        sa = a[i+start];
        if (sa == '-' || sb[i] == '-'){
            score += GAP;
        } else if (sa == sb[i]) {
            score += MATCH;
        } else {
            score += MISMATCH;
        }
    }
    return score;
}

int main(){

    // Inputs
    int len_a, len_b;
    std::cin >> len_a >> len_b;
    std::string a, b;
    std::cin >> a >> b;

    // RNG
    std::default_random_engine generator(1337);
    std::uniform_int_distribution<int> subseq_size_distro(4,len_b);
    const int subseq_size = subseq_size_distro(generator);
    
    std::uniform_int_distribution<int> p_distro(10,100);
    const int p = p_distro(generator);

    // Algorithm
        // Ensure the largest string is a
        int temp_len;
        std::string temp;
        if (len_a < len_b){
            temp_len = len_b;
            temp = b;
            len_b = len_a;
            b = a;
            len_a = temp_len;
            a = b;
        }

        // Random subsize
        // std::cout << "Subsequence size: " << subseq_size << std::endl;
        std::uniform_int_distribution<int> index_distro(0,len_b-subseq_size);

        // Assemble substring b
        int i = index_distro(generator);
        std::string sb;
        for(int j = 0; j<subseq_size; j++){
            sb.push_back(b[i+j]);
        }

        // Try p combinations
        int max_score = std::max(len_a, len_b)*std::max(MISMATCH, GAP); // Pior score possível é tudo em MISMATCH/GAP, dependendo dos pesos
        int ix = 0;
        int best_ix = 0;
        for (int i = 0; i<p; i++){
            ix = index_distro(generator);
            int score = match_score(sb, a, ix, subseq_size);
            if (score > max_score){
                max_score = score;
                best_ix = ix;
            }
        }


    // Output
    if (SHOW_REPORT){
        std::cout << "Sequências: " << std::endl;
        std::cout << sb << std::endl;
        for(int j = 0; j<subseq_size; j++){
            std::cout << a[best_ix+j];
        } 
        std::cout << std::endl << "Score: " << max_score << std::endl;
    } else {
        std::cout << max_score << std::endl;
    }

    return 0;
}