#!/usr/bin/env python3
import os
import pandas as pd
import time
import subprocess
import numpy as np
import matplotlib.pyplot as plt

test_list = list(map(lambda x: 'in/'+x, os.listdir('in')))

exe = ['Sequential/LocalSearch/main', 'Sequential/SmithWaterman/main', 'Sequential/ExaustiveSearch/main', 'Sequential/BoundedExaustiveSearch/main']

time_dict = {}
for x in exe:
    time_dict[x] = []
for i in test_list:
    for ex in exe:
        # print(i)
        with open(i, 'r') as f:
            r = f.read()
        sub_list = []
        for _ in range(10):
            start=time.perf_counter()
            subprocess.run(['./'+ex], input=r, text=True, capture_output=True)
            end=time.perf_counter()
            sub_list.append((end-start)*1000)
            # print(f"File {ex} took {(end-start)*1000000:0.2f}us to run on input {i}")
        time_dict[ex].append(np.mean(sub_list))
    print("Done with ", ex)

df = pd.DataFrame(data=time_dict.values(), columns=test_list, index=exe)

series_local_search = df.loc[exe[0]]
series_heuristic_search = df.loc[exe[1]]
plt.figure(figsize=(18,12))
plt.plot(series_local_search)
plt.plot(series_heuristic_search)
plt.xticks(range(len(test_list)), test_list, rotation=90)
plt.savefig("Analysis.pdf")
