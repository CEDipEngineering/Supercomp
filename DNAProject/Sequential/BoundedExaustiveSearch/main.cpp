#include <iostream>
#include <string>

#define MATCH 2
#define MISMATCH -1
#define GAP -1

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



    // Output


    return 0;
}