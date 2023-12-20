#!/usr/bin/env python
# coding: utf-8

# In[10]:


from math import log2
 
n = int(input())
el_list = list(map(int, input().split()))
letter_dict = {
    0:'a', 1:'b', 2:'c', 3:'d', 4:'e', 5:'f', 6:'g', 7:'h',
    8:'i', 9:'j', 10:'k', 11:'l', 12:'m', 13:'n', 14:'o',
    15:'p', 16:'q', 17:'r', 18:'s', 19:'t', 20:'u', 21:'v',
    22:'w', 23:'x', 24:'y', 25:'z', 26:' ',
}
str_result = ''
 
sum_letter = el_list[0]
first_letter = int(log2(sum_letter))
str_result += letter_dict[first_letter]
 
for i in range(1, n):
    diff = el_list[i] - sum_letter
    num = int(log2(abs(diff)))
    str_result += letter_dict[num]
    sum_letter += 2**num
 
print(str_result)


# In[ ]:




