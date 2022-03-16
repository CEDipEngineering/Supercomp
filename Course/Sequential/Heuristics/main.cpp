#include <iostream>
#include<vector>
#include<algorithm>
#include<random>

struct Item {
    int id;
    int weight;
    int value;
};

struct Knapsack {
    int id;
    int weight;
    bool arr[];
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

    // Print output
    
    return 0;
}