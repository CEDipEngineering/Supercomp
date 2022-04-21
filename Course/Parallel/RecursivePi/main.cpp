#include <omp.h>
#include <iostream>
#include <iomanip>
static long num_steps = 1024l*1024*1024*2;

#define MIN_BLK  1024*1024*256

static double sum = 0;

void pi_r(long Nstart, long Nfinish, double step) {
    long i,iblk;
    if (Nfinish-Nstart < MIN_BLK){
        #pragma omp parallel for reduction(+:sum)
        for (i = Nstart; i < Nfinish; i++){
            double x = (i+0.5)*step;
            sum += 4.0/(1.0+x*x);
        }
    } else {
        iblk = Nfinish-Nstart;
        #pragma omp task firstprivate(Nstart, Nfinish, iblk, step)
        {
            pi_r(Nstart,         Nfinish-iblk/2,step);
        } // end omp task
        #pragma omp task firstprivate(Nstart, Nfinish, iblk, step)
        {
            pi_r(Nfinish-iblk/2, Nfinish,       step);
        } // end omp task

        #pragma omp taskwait
    }
}

int main () {
    long i;
    double step, pi;
    double init_time, final_time;
    step = 1.0/(double) num_steps;
    init_time = omp_get_wtime();
    #pragma omp parallel 
    {
        #pragma omp single
        pi_r(0, num_steps, step);
    }
    pi = step * sum;
    final_time = omp_get_wtime() - init_time;

    std::cout << "for " << num_steps << " steps pi = " << std::setprecision(15) << pi << " in " << final_time << " secs\n";
}

/*
Sequencial
for 2147483648 steps pi = 3.14159265358977 in 7.73675969999999 secs

Parallel
for 2147483648 steps pi = 3.14159265358983 in 1.20446539999989 secs

7.1/1.2 = 6.4x Speedup

*/