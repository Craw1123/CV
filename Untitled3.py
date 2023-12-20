#!/usr/bin/env python
# coding: utf-8

# In[5]:


import numpy as np
from scipy import stats
import math
import random
from scipy.stats import chi2
from collections import defaultdict
sample_sizes = np.array([50, 50, 50, 50, 50, 50, 50, 50,50,50])
theta = 1000
sigma = 66400
samples_normal = [abs(np.random.normal(theta, sigma, size=n).astype(int)) for n in sample_sizes]



def grouping(n, x):
    n_ints = 1 + math.floor(math.log(n, 2))
    #n_ints = 5*math.log(n)
    #n_ints = math.sqrt(n)
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


# In[20]:


def xi_square(sample):
    alpha = 0.05
    
    x, width, p = grouping(len(sample), sample)
    sigma = 0
    for i in range(len(x)-1):
        theor_prob = dist_func(x[i+1]) - dist_func(x[i])
        sigma += (p[i] - len(sample)*theor_prob)**2/(len(sample)*theor_prob)

    theor_prob = dist_func(x[-1]+width) - dist_func(x[-1])
    sigma += (p[-1] - len(sample)*theor_prob)**2/(len(sample)*theor_prob)

    ta = chi2.ppf(1 - alpha, df=len(x) - 1)
    if sigma <= ta:
        print(len(sample), round(sigma,5), round(ta,5), "Принимаем")
    else:
        print(len(sample), round(sigma,5), round(ta,5), "Отклоняем")
        #print(sigma, ta)


# In[16]:


def dist_func(x):
    f = (x-1)/999
    return f


#print(samples_normal)

for i,k in enumerate(samples_normal):
    #print(k.min(),k.max())
    #3
    print(i+1,") 弌�亠亟仆亠亠:",k.mean(),", ��从仍仂仆亠仆亳亠:",k.std(),", �仂���亳�亳亠仆� 于舒�亳舒�亳亳:", k.mean()/k.std(),sep="")
    if abs(int((k.mean()/k.std())*100)) <= 33:
        print("�亟仆仂�仂亟仆舒")
    else:
        print("�亠 仂亟仆仂�仂亟仆舒")
    #4
    print(k)
    xi_square((k.tolist()))


# In[ ]:




