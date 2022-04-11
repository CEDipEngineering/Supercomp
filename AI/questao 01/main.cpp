#include <iostream>
#include<vector>
#include<algorithm>
#include<random>

struct Job {
    int id;
    int weight;
};

struct Machine {
    int id;
    int weight;
    std::vector<bool> arr;
};

int main(){
    // Read number of Jobs and capacity of Machines
    int n = 0;
    int W = 0;
    std::cin >> n >> W;

    // Read all Jobs
    std::vector<Job> Jobs;
    Jobs.reserve(n);
    int w;
    for(int i = 0; i<n; i++){
        std::cin >> w;
        Jobs.push_back({i, w});
    }


    // std::cout << "LIDO: " << std::endl;
    // for (auto& j : Jobs){
    //     std::cout << j.id << " " << j.weight << std::endl;
    // }
    // Algorithm
        std::vector<Machine> machines;
        machines.reserve(W);
        for(int i = 0; i<W; i++){
            machines[i] = {i,0,{}};
            machines[i].arr.reserve(n);
            for(int j = 0; j<n; j++){
                machines[i].arr.push_back(false);
            }
        }
        
        int total_time = -1;
        std::sort(Jobs.begin(), Jobs.end(), [](Job a, Job b){return a.weight>b.weight;}); // Jobs in descending order
        for (auto& j : Jobs){
            
            // Find most idle machine
            int lowest = 8191; // big integer
            int index = 0;
            for(int i = 0; i<W; i++){
                if (machines[i].weight < lowest){
                    lowest = machines[i].weight;
                    index = i;
                }
            }
            // Give job j to idlest machine
            machines[index].weight += j.weight;
            machines[index].arr[j.id] = true;
            if (machines[index].weight > total_time){
                total_time = machines[index].weight;
            }
        }

    for(int i = 0; i<W; i++){
        std::cout << "Machine " << i << ": ";
        for(int j = 0; j<n; j++){
            std::cout << machines[i].arr[j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "Total execution time will be: " << total_time << std::endl;
    
    return 0;
}