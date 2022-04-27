#include <iostream>
#include <string>
#include <chrono>


#define MATCH 2
#define MISMATCH -1
#define GAP -1

int match_score(std::string a, std::string b){
    // a and b must be of equal size
    int score = 0;
    int size = a.length();
    for (int i = 0; i<size; i++){
        if (a[i] == b[i]){
            score += MATCH;
        } else if (a[i] == '-' || b[i] == '-') {
            score += MISMATCH;
        } else {
            score += GAP;
        }
    }
    return score;
}

int main(){

    int size_a, size_b;
    std::cin >> size_a >> size_b;

    std::string a, b;
    std::cin >> a >> b;


    // Algorithm
        /*
        Starting at size of smallest string, check all indexes that fit largest string for substrings of same size.
        Keeping track of best score and strings through calculations, run through all substrings for this size then reduce size by 1 and repeat,
        until a score exceeds best known score.
        When a better score is found, replace best score and substrings, and then check if new best is MATCH * size; if so, no better score could come
        from smaller substrings, therefore, return best score and relative substrings.
        */
        auto begin = std::chrono::high_resolution_clock::now();

        std::string big, small;
        if (size_a>size_b) {
            big = a;
            small = b;
        } else {
            big = b;
            small = a;
        }
        int size_big, size_small;
        size_big = big.size();
        size_small = small.size();

        int curr_size = size_small;
        int high_score = -9999;
        int score = 0;
        std::string match_small, match_big;
        std::string sa, sb;
        unsigned long num_iter = 0;
        while (curr_size > 0){
            // For every substring you can fit in the smaller sequence:
            for (int i = 0; i<=size_small-curr_size; i++){
                sa = small.substr(i, curr_size);
                // Try every substring you can fit on the bigger sequence:
                for (int j = 0; j<=size_big-curr_size; j++){
                    num_iter++;
                    sb = big.substr(j, curr_size);
                    score = match_score(sa, sb);
                    if (score > high_score){
                        high_score = score;
                        match_small = sa;
                        match_big = sb;
                        // Can't get any better than full match
                        if (score > curr_size*MATCH) {
                            break;
                        }
                        std::cout << high_score << " " << match_small << std::endl << match_big << std::endl;
                    }
                }
            }
            // break;
            curr_size--;
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);


    // Output
    std::cout << std::endl << std::endl;
    std::cout << "REPORT:" << std::endl;
    std::cout << "Best Score found: " << high_score << std::endl;
    std::cout << "Strings that produced said score: " << std::endl << match_small << std::endl << match_big << std::endl;
    std::cout << "Number of iterations: " << num_iter << std::endl;
    printf("Time measured: %.6f seconds.\n", elapsed.count() * 1e-9);

    return 0;
}