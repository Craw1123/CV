#!/usr/bin/env python
# coding: utf-8

# In[6]:


import numpy as np
from scipy import stats
import math
import time
import random
from scipy.stats import chi2
from collections import defaultdict
from datetime import datetime
from matplotlib import pyplot as plt
from numba import njit
sample_sizes = np.array([50, 50, 50, 50, 50, 50, 50, 50,50,50])
samples = [];

m=32768
a=23
b=12345
seed_1 = 1
seed_2 = 2

@njit
def gen(size):
    smpl = []
    k = 50; a0 = 258; a = 3; b = 13; c = 7; d = 5; a_next = 0; a_prev = a0; m = 10007;
    for z in range(size):
        smpl1=[]
        for i in range(1,k+1):
            a_next = a * pow(a_prev,3)
            a_next=a_next+b*pow(a_prev,2) 
            a_next=a_next+c*a_prev
            a_next=a_next+d
            a_next =(a_next%m)
            a_prev = a_next
            smpl1.append(a_next)
        smpl.append(smpl1)
    return smpl

zz = int(str(time.time())[-1])
        
def gen2_poshtuch():
    m = 16384
    global seed_1, seed_2, zz
    temp = (seed_1+seed_2**zz + seed_1 ^ seed_2) % m + 1 
    zz = (zz + seed_2) % 4 
    seed_2 = seed_1	
    seed_1 = temp
    if seed_1 > 10000:
        return gen2_poshtuch()
    return seed_1    
    
def gen2(size):
    smpl = []
    for i in range(1,size):
        smpl.append(gen2_poshtuch())
    return smpl

def grp(n, x):
    n_ints = 1 + math.floor(math.log(n, 2))
    width = (math.floor(x[-1])+1-math.floor(x[0]))/n_ints

    p = []
    count = 0
    l_boarder = math.floor(x[0])
    r_boarder = l_boarder + width
    int_mids = []
    i = 0

    while i < len(x):
        if x[i] >= r_boarder:
            int_mids.append(l_boarder)
            p.append(count)
            count = 0
            l_boarder = r_boarder
            r_boarder += width
        else:
            count += 1
            i+=1
    int_mids.append(l_boarder)
    p.append(count)

    return(int_mids, width, p)




def xi_square(sample):
    alpha = 0.05
    
    x, width, p = grp(len(sample), sample)
    sigma = 0
    minn = min(sample)
    maxx = max(sample)
    for i in range(len(x)-1):
        theor_prob = dstf(x[i+1]) - dstf(x[i])
        sigma += (p[i] - len(sample)*theor_prob)**2/(len(sample)*theor_prob)

    theor_prob = dstf(x[-1]+width) - dstf(x[-1])
    sigma += (p[-1] - len(sample)*theor_prob)**2/(len(sample)*theor_prob)

    ta = chi2.ppf(1 - alpha, df=len(x) - 1)
    if sigma <= ta:
        print(len(sample), round(sigma,5), round(ta,5), "Принимаем")
    else:
        print(len(sample), round(sigma,5), round(ta,5), "Отверагаем")






def dstf(x):
    f = (x-minn)/maxx
    return f

for i in sample_sizes:
    listt = gen2(i)   
    listt.sort()
    samples.append(listt)



for i,k in enumerate(samples):
    #3
    npar = np.array(k)
    print(i+1,") Среднее значение:",npar.mean(),", Отклонение от среднего:",npar.std(),", Коэффициент вариации:", npar.std()/npar.mean(),sep="")
    if abs(int((npar.std()/npar.mean())*100)) <= 55:
        print("Выборка однородна")
    else:
        print("Выборка не однородна")
    #4
    minn = min(k)
    maxx = max(k)
    xi_square(k)
    
    
    
    
#5   
sizes = [1000]
for i in range(100000, 1100000, 100000):
    sizes.append(i)
time_gen1 = []
time_gen2 = [] 
time_randint = []
for size in sizes:
 
    temp_time = time.time()
    gen(size)
    time_gen1.append(time.time() - temp_time)
 
    temp_time = time.time()
    gen2(size)
    time_gen2.append(time.time() - temp_time)
    
    
    temp_time = time.time()
    for i in range(size):
        random.randint(0,100000)
    time_randint.append(time.time() - temp_time)

plt.plot(sizes, time_gen1, label='1 метод генерации')
plt.plot(sizes, time_gen2, label='2 метод генерации')
plt.plot(sizes, time_randint, label='встроенный метод генерации')
plt.legend()
plt.show()


# In[ ]:




