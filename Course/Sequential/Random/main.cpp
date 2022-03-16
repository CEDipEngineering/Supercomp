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
    std::vector<bool> arr;
};

void print_knapsack(Knapsack k){
    std::cout << "Knapsack " << k.id << std::endl;
    std::cout << "Weight: " << k.weight << std::endl;
    for(int i = 0; i<(int)k.arr.size(); i++){
        std::cout << k.arr[i] << " ";
    }
    std::cout << std::endl;
}

void print_knapsack(Knapsack k, std::vector<Item> items){
    std::cout << "Knapsack " << k.id << std::endl;
    std::cout << "Weight: " << k.weight << std::endl;
    int totalValue = 0;
    for(int i = 0; i<(int)k.arr.size(); i++){
        std::cout << k.arr[i] << " ";
        totalValue += k.arr[i]*items[i].value;
    }
    std::cout << std::endl << "Total value: " << totalValue << std::endl;
}

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