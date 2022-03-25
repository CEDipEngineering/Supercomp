#include <iostream>
#include<vector>
#include<algorithm>
#include<random>
#include<bitset>

#define BITSIZE 48


struct Item {
    int id;
    int weight;
    int value;
};

int main(){
    // Read number of items and capacity of knapsacks
    int n = 0;
    int W = 0;
    std::cin >> n >> W;

    // Read all items
    std::vector<Item> items;
    items.reserve(n);
    int w,v;
    for(int i = 0; i<n; i++){
        std::cin >> w >> v;
        items.push_back({i, w, v});
    }

    // Algorithm
    std::bitset<BITSIZE> best(0);
    long value, weight, maxValue, maxWeight;
    maxValue = -1;
    maxWeight = -1;
    for(long i = 0; i<(2<<(n-1)); i++){
        std::bitset<BITSIZE> b(i);
        value = 0;
        weight = 0;
        for(long j = 0; j<n; j++){
            if(b[j]){
                value+=items[j].value;
                weight+=items[j].weight;
            }
        }
        // std::cout << "Knapsack: " << b << " Value: " << value << " Weight: " << weight << std::endl;
        if(weight<=W && value>=maxValue){
            best = b;
            maxValue = value;
            maxWeight = weight;
        }
    }

    // Output
    std::cout << "Best knapsack possible: "; 
    for(long i = 0; i<n; i++){
        std::cout << best[i] << " "; 
    }
    std::cout << "Weight: " << maxWeight << " Value: " << maxValue << std::endl;
    return 0;
}