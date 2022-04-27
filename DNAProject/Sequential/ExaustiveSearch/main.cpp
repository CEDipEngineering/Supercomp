#include <iostream>
#include <string>
#include <set>
#include <vector>

#define MATCH 2
#define MISMATCH -1
#define GAP -1

#define SHOW_REPORT false

std::set<std::string> make_all_subsets(std::string a){
    std::set<std::string> sub;
    int n = a.length();
    for(int i = 0; i<=n; i++){
        for(int j = 1; j<=n-i; j++){
            sub.insert(a.substr(i,j)); // Start at i, grab j letters
        }
    }
    return sub;
}

int score_pair(std::string a, std::string b){
    /*
    ONLY FOR a AND b OF EQUAL SIZE
    */
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

int main(){

    int size_a, size_b;
    std::cin >> size_a >> size_b;

    std::string a, b;
    std::cin >> a >> b;

    // std::cout << size_a << ": " << a << std::endl << size_b << ": " << b << std::endl;
    std::set<std::string> all_a = make_all_subsets(a);
    std::set<std::string> all_b = make_all_subsets(b);

    std::string best_a, best_b;
    int best_score=-99999;
    int score = 0;
    std::string big, small;
    std::cout << "Starting computation with " << all_a.size()*all_b.size() << " elements" << std::endl;
    for(auto& el_a : all_a){
        for(auto& el_b : all_b){
            // Same size, just run score_pair
            // std::cout << el_a << ";" << el_b << std::endl;
            if(el_a.length() == el_b.length()){
                score = score_pair(el_a, el_b);
                if(score > best_score){
                    best_score = score;
                    best_a = el_a;
                    best_b = el_b;
                }
            } else { // Different Sizes, handle every case
                if (el_a.length()>el_b.length()){
                    big = el_a;
                    small = el_b;
                } else {
                    big = el_b;
                    small = el_a;
                }
                int n = big.length() - small.length();
                for (int i  = 0; i<n; i++){
                    score = score_pair(small, big.substr(i, small.length()));
                    if(score > best_score){
                        best_score = score;
                        best_a = small;
                        best_b = big.substr(i, small.length());
                    }
                }
            }
        }
    }
    
    
    
    // Output
    if(SHOW_REPORT){
        std::cout << "Score for best possible match is "<< best_score << " between " << std::endl << best_a << " and "<< std::endl << best_b << std::endl;
    } else {
        std::cout << best_score << std::endl;
    }


    return 0;
}