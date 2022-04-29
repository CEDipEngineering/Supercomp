#!/usr/bin/env python3
import random
import itertools as it

n_list = list(range(10,201,10))
m_list = list(range(10,201,10))
i = 0
for n, m in it.product(n_list, m_list):
     file = 'in/dna{0}.seq'.format(i)
     i += 1
     f = open(file, 'w')
     seq=[str(n)+'\n',
          str(m)+'\n',
          ''.join(random.choices(['A','T','C','G','-'],k=n))+'\n',
          ''.join(random.choices(['A','T','C','G','-'],k=m))]
     f.writelines(seq)
     f.close()
     # print(''.join(seq))
