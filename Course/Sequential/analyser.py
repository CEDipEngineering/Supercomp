import os
import time
import subprocess

exe = ['Heuristics/main', 'LocalSearch/main', 'Random/main']

time_dict = {}
for x in exe:
    time_dict[x] = []
for i in range(1,2):
    with open('Tests/tests/test{0}.txt'.format(i), 'r') as f:
        for ex in exe:
            start=time.perf_counter()
            proc = subprocess.run(['./'+ex], input=f.read(), text=True, capture_output=True)
            end=time.perf_counter()
            time_dict[ex].append(end-start)
            print(f"File {ex} took {(end-start)*1000000:0.2f}us to run on input {i}")
            print(proc.stdout)