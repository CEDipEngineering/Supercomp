main_par: main.cu
	nvcc -arch=sm_70 -std=c++14 main_par.cu -o main_par

run: main_par
	./main_par < in.txt
