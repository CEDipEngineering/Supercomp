
#include <iostream>
#include <string>
#include <vector>
#include <bits/stdc++.h>

const int MATCH = 2;
const int MISMATCH = -1;
const int GAP = -1;

int w(std::string a, std::string b, int i, int j){
    if (a[i] == b[j]){
        return MATCH;
    } else if(a[i] == '-' || b[j] == '-'){
        return GAP;
    } else {
        return MISMATCH;
    }
}

int main(){
    struct Cross {
        int value;
        int direction;
        int i;
        int j;
        /*
        1 2
        0 X 
        X will have direction equal to the value of the matrix above
        */
    };
    
    // Read sizes and strings
    int n, m;
    std::cin >> n >> m;
    std::string a, b;
    std::cin >> a >> b;

    // Print out sizes and strings for debug
    std::cout << n << " " << m << " " << std::endl;
    std::cout << a << std::endl;
    std::cout << b << std::endl;
    a = "-" + a;
    b = "-" + b;
    n++;
    m++;
    // Initialize array
    Cross H[n][m];

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            H[i][j] = {0, 0, i, j};
        }
    }

    Cross diagonal, insertion, deletion;
    int max; 
    int index;
    for(int i=1; i<n; i++){
        for (int j=1; j<m; j++){
            diagonal = {H[i-1][j-1].value + w(a, b, i, j), 1};
            deletion = {H[i-1][j].value - 1, 2};
            insertion = {H[i][j-1].value - 1, 0};
            // std::cout<<"diagonal: "<<diagonal.value<<" "<<diagonal.direction<<std::endl;
            // std::cout<<"deletion: "<<deletion.value<<" "<<deletion.direction<<std::endl;
            // std::cout<<"insertion: "<<insertion.value<<" "<<insertion.direction<<std::endl;
            
            max = -3;
            index = 1;
            if(deletion.value > max){
                max = deletion.value;
                index = deletion.direction;
            }
            if(insertion.value > max){
                max = insertion.value;
                index = insertion.direction;
            }
            if(diagonal.value > max){
                max = diagonal.value;
                index = diagonal.direction;
            }
            // No negatives
            if (max < 0){max = 0;}

            // Store result in H
            H[i][j].value = max;
            H[i][j].direction = index;
        }
    }

    int max_i = n, max_j = m, max_v;
    max_v = -1;
    for(int i=0; i<n; i++){
        for (int j=0; j<m; j++){
            if(H[i][j].value > max_v){
                max_i = i;
                max_j = j;
                max_v = H[i][j].value;
            }   
        }
    }

    std::vector<Cross> walk;
    walk.push_back(H[max_i][max_j]);
    bool done = false;
    Cross next_step, curr_step;
    int i = 0;
    while (!done){
        curr_step = walk[i];
        if (curr_step.direction == 0){
            next_step = H[curr_step.i][curr_step.j-1];
        } else if (curr_step.direction == 1){
            next_step = H[curr_step.i-1][curr_step.j-1];
        } else if (curr_step.direction == 2){
            next_step = H[curr_step.i-1][curr_step.j];
        } else {
            // Not good, should never happen
            std::cerr << "Error in backtracking!";
            return 1;
        }
        if(next_step.value == 0 || (next_step.i == 0 && next_step.j == 0)){
            done = true;
            continue;
        }
        walk.push_back(next_step);
        i++;        
    }

    std::string best_match = "";
    for (auto& el: walk){
        best_match.push_back(a[el.i]);
    }

    for(int i=0; i<n; i++){
        for (int j=0; j<m; j++){
            std::cout << H[i][j].value << " ";
        }
        std::cout << std::endl;
    }
    std::reverse(best_match.begin(), best_match.end());
    std::cout << "Best match found is: " << best_match << std::endl;

    
    return 0;
}