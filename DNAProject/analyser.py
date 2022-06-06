#!/usr/bin/python3
import os
import pandas as pd
import time
import subprocess
import numpy as np
import matplotlib.pyplot as plt
import pickle
from mpl_toolkits.mplot3d import Axes3D
import random
import itertools as it

# Run with nohup python3 analyser.py & > nohup.txt

class Test():
    def __init__(self, size_a, size_b, time):
        self.size_a = size_a
        self.size_b = size_b
        self.time = time
    def __repr__(self):
        return f"{self.size_a=}, {self.size_b=}, {self.time=}"
    def __str__(self):
        return f"{self.size_a=}, {self.size_b=}, {self.time=}"

def gen_seq(n, m):
     seq=[str(n)+'\n',
          str(m)+'\n',
          ''.join(random.choices(['A','T','C','G','-'],k=n))+'\n',
          ''.join(random.choices(['A','T','C','G','-'],k=m))]
     return ''.join(seq)

if __name__ == "__main__":  
    print("Starting tests...")

    exe = [
        ('Sequential/LocalSearch/main', 500), 
        ('Sequential/SmithWaterman/main', 500), 
        ('Sequential/ExaustiveSearch/main', 100), 
        ('Sequential/BoundedExaustiveSearch/main', 500),
        ('Parallel/GPU/main', 100),
        ('Parallel/Hybrid/main', 200),
        ('Parallel/LocalSearch/main_par', 200),
        ('Parallel/LocalSearch/main_seq', 200)
    ]


    time_dict = {}
    for x, s in exe:
        time_dict[x] = []
    progress = 0
    for ex, max_size in exe:
        for n, m in it.product(list(range(10, max_size+1, 10)), list(range(10, max_size+1, 10))):
            print("\rStart test {0},{2} with exe {1}".format(n, ex, m), end='\r')
            r = gen_seq(n, m)
            sub_list = []
            repeats = 3
            for _ in range(repeats):
                start=time.perf_counter()
                subprocess.run(['./'+ex], input=r, text=True, capture_output=True)
                end=time.perf_counter()
                sub_list.append((end-start)*1000)
                # print(f"File {ex} took {(end-start)*1000000:0.2f}us to run on input {i}")
            time_dict[ex].append(Test(n, m, np.mean(sub_list)))
        # print("Done with ", i)
        progress += 1
        print("\r" + " "*100, end = '\r')
        print("Done with {0}".format(ex))
        print(f"\rProgress: {progress}/{len(exe)} ({100*progress/len(exe):.02f}%) last_test=({n}, {m})", end='\r')
    with open('/home/user/Supercomp/DNAProject/cache_big.pickle', 'wb') as f:
        pickle.dump(time_dict, f)
