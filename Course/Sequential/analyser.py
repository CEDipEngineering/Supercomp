import os
import time
import subprocess

exe = ['Heuristics/main', 'LocalSearch/main', 'Random/main']

time_dict = {}
for x in exe:
    time_dict[x] = []
proc_list = []
for i in range(1,2):
    for ex in exe:
        with open('Tests/tests/test{0}.txt'.format(i), 'r') as f:
            start=time.perf_counter()
            proc_list.append(subprocess.run(['./'+ex], input=f.read(), text=True, capture_output=True))
            end=time.perf_counter()
            time_dict[ex].append(end-start)
            print(f"File {ex} took {(end-start)*1000000:0.2f}us to run on input {i}")
            print(proc_list[-1].stdout)