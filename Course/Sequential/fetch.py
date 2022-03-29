import urllib.request

for i in range(1,9):
    capacity = urllib.request.urlopen("https://people.sc.fsu.edu/~jburkardt/datasets/knapsack_01/p0{0}_c.txt".format(i)).read().decode().replace(' ', '')
    weights = urllib.request.urlopen("https://people.sc.fsu.edu/~jburkardt/datasets/knapsack_01/p0{0}_w.txt".format(i)).read().decode().replace(' ', '')
    values = urllib.request.urlopen("https://people.sc.fsu.edu/~jburkardt/datasets/knapsack_01/p0{0}_p.txt".format(i)).read().decode().replace(' ', '')
    solution = urllib.request.urlopen("https://people.sc.fsu.edu/~jburkardt/datasets/knapsack_01/p0{0}_s.txt".format(i)).read().decode()
    wv = list(zip(weights.split('\n')[:-2],values.split('\n')[:-2]))
    wv = list(map(lambda x: ' '.join(x), wv))
    with open('test{0}.txt'.format(i), 'w') as f:
        f.write("{0} {1}\n{2}".format(int(capacity),len(weights.split('\n'))-1,'\n'.join(wv)))

    with open('solution{0}.txt'.format(i), 'w') as f:
        f.write(solution.replace('\n', ' '))