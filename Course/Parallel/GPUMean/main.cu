#include <thrust/device_vector.h>
#include <thrust/host_vector.h>
#include <thrust/functional.h>
#include <thrust/transform.h>
#include <iostream>
#include <fstream>

int main() {
    std::vector<float> diff;
    std::ifstream StockFile("./stocks2.csv");
    std::string line;
    while(std::getline(StockFile, line)){
        int i = 0;
        for(char c: line){
            if (c == ' ') break;
            i++;
        }
        float appl, micr;
        appl = std::stof(line.substr(0, i));
        micr = std::stof(line.substr(i, line.size()-i));
        diff.push_back(appl-micr);
    }

    const double inv_n = 1.0/diff.size();

    thrust::device_vector<float> V(diff);
    float sum = thrust::reduce(V.begin(), V.end());

    const double mean = inv_n * sum;

    std::cout << "Mean: " << mean << std::endl;
}