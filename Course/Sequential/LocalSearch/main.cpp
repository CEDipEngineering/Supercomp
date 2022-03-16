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
    std::default_random_engine generator(1337);
    std::uniform_real_distribution<double> coin_distro(0,1);
    std::uniform_int_distribution<int> random_item_distro(0,n-1);

    const int bag_amount = 10;
    std::vector<Knapsack> bags;
    for(int i = 0; i<bag_amount; i++){
        bags.push_back({i,0,{}});
        bags[i].arr.reserve(n);
        for(int j = 0; j<n; j++) {
            bags[i].arr.push_back(false);
        }
    }

    double coin;
    for(int i = 0; i<bag_amount; i++){
        for(int j = 0; j<n; j++){
            bags[i].arr[j] = false;
            coin = coin_distro(generator);
            if(coin > 0.5){
                if (bags[i].weight + items[j].weight < W){
                    bags[i].arr[j] = true;
                    bags[i].weight += items[j].weight;
                }
            }
        }
    }

    // Try to add another item to each bag
    for(int i = 0; i<bag_amount; i++){
        int ix = random_item_distro(generator);
        if(!bags[i].arr[ix]){
            if (bags[i].weight + items[ix].weight < W){
                bags[i].arr[ix] = true;
                bags[i].weight += items[ix].weight;
            }
        }
    }



    // Print output
    for(auto& el: bags){
        print_knapsack(el, items);
    }
    
    return 0;
}