#include <thrust/device_vector.h>
#include <thrust/host_vector.h>
#include <thrust/transform_reduce.h>
#include <thrust/functional.h>
#include <thrust/transform.h>
#include <thrust/random.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <chrono>
#include <cstdlib>
#include <algorithm>
#include <cmath>
#include <random>
#define N 10000000

struct rng_gpu {
    //INSIRA AQUI O ENGINE A DISTRIBUTION

    __device__ __host__
    double operator() (int seed) {
        thrust::default_random_engine en_gpu((seed*23778956)%151368);
        thrust::uniform_real_distribution<double> d(0,1);
        return d(en_gpu);
    }
};

struct add_squares
{
    __host__ __device__
    double operator()(const double& x, const double& y) {
        return x*x + y*y;
    }
};

struct greater_than_one {
    //INSIRA AQUI O ENGINE A DISTRIBUTION

    __device__ __host__
    double operator() (double num) {
        return num <= 1 ? 1 : 0;
    }
};

int main(int argc, char** argv){

    // if (argc < 2){
    //     std::cerr << "Faltou passar a seed" << std::endl;
    //     return 1;
    // }

    thrust::device_vector<double> X(N);
    thrust::device_vector<double> Y(N);
    thrust::transform(thrust::make_counting_iterator(0), thrust::make_counting_iterator(N), X.begin(), rng_gpu());
    thrust::transform(thrust::make_counting_iterator(N), thrust::make_counting_iterator(2*N), Y.begin(), rng_gpu());
   
    // for (int i = 0; i<10; i++){
    //     std::cout << X[i] << " ";
    // }
    // std::cout << std::endl;

    // for (int i = 0; i<10; i++){
    //     std::cout << Y[i] << " ";
    // }
    // std::cout << std::endl;

    thrust::transform(X.begin(), X.end(), Y.begin(), X.begin(), add_squares());

    // for (int i = 0; i<10; i++){
    //     std::cout << X[i] << " ";
    // }
    // std::cout << std::endl;

    double pi = 4.0*thrust::transform_reduce(X.begin(), X.end(), greater_than_one(), 0, thrust::plus<double>())/(double)N;

    std::cout << "Pi is: " << pi << std::endl;

    return 0;
}