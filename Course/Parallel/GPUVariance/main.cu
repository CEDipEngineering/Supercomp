#include <thrust/device_vector.h>
#include <thrust/host_vector.h>
#include <thrust/functional.h>
#include <thrust/transform.h>
#include <iostream>
#include <fstream>

int main() {
    std::vector<double> diff;
    std::ifstream StockFile("./stocks.csv");
    std::string line;
    while(std::getline(StockFile, line)){
        // int i = 0;
        // for(char c: line){
        //     if (c == ' ') break;
        //     i++;
        // }
        double appl, micr;
        // appl = std::stof(line.substr(0, i));
        micr = std::stof(line); //.substr(i, line.size()-i));
        diff.push_back(micr);
    }

    const double inv_n = 1.0/diff.size();

    thrust::device_vector<double> V(diff);
    double sum = thrust::reduce(V.begin(), V.end());

    const double mean = inv_n * sum;

    std::cout << "Mean: " << mean << std::endl; 

    
    thrust::device_vector<double> var(V.size());
    thrust::transform(V.begin(), V.end(), thrust::constant_iterator<double>(-mean), var.begin(), thrust::plus<double>());
    thrust::transform(var.begin(), var.end(), V.begin(), thrust::square<double>());
    const double variance = thrust::reduce(V.begin(), V.end()) * inv_n;

    std::cout << "Variance: " << variance << std::endl;

    thrust::device_vector<double> V2(V.size()-1);
    thrust::transform(V.begin()+1, V.end(), V.begin(), V2.begin(), thrust::minus<double>());
    
}