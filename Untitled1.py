#!/usr/bin/env python
# coding: utf-8

# In[5]:


import matplotlib.pyplot as plt
import math
x = [100,500,1000,1500,5000,7500,10000,15000]
#y1 = [0.0064,0.0452,0.0433,0.0693,1.3187,0.3164,0.7301,1.6869]
#y2 = [0.0039,0.0287,0.0318,0.0478,0.6262,0.5933,0.6827,2.283]
#y3 = [0.0004,0.0006,0.0006,0.0006,0.0008,0.001,0.0012,0.002]
y4 = [0,4,9,15,137,388,647,1335]
y5 = [0,4,9,15,139,386,643,1329]
#plt.plot(x, y1, label = "Прямой поиск")
#plt.plot(x, y2, label= "Бинарный поиск")
#plt.plot(x, y3, label = "Multimap")
plt.plot(x, y4, label = "Простой хэш")
plt.plot(x, y5, label = "Сложный хэш")
#plt.yscale('log')
plt.legend()


# In[ ]:





# In[ ]:




