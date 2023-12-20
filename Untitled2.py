#!/usr/bin/env python
# coding: utf-8

# In[ ]:


import random
import math
from scipy.stats import chi2
import time
from matplotlib import pyplot as plt

seed_lcg = 1#задаем первый элемент последовательности lcg


#модернизированный линейный конгруэнтный метод (1, 10000)
def lcg():
    global seed_lcg
    m = 10007 #модуль (простое число)
    a = 16807 #множитель 
    c = seed_lcg ^ m #приращение (xor для большего периода)
    seed_lcg = (a*seed_lcg + c) % m + 1 #классическая формула lcg + 1
    if seed_lcg > 10000: #исключаем значения от 10001 до 10006
        return lcg()
    return seed_lcg

k = 1 #задаем начальное значение степени
#модернизированный квадратичный конгруэнтный метод
def gen():
    smpl = []
    k = 1000; a0 = 258; a = 3; b = 22; c = 7; d = 5; a_next = 0; a_prev = a0; m = 10007;
    for i in range(1,k+1):
        a_next = a * pow(a_prev,3)
        a_next=a_next+b*pow(a_prev,2) 
        a_next=a_next+c*a_prev
        a_next=a_next+d
        a_next =(a_next%m)
        a_prev = a_next
        smpl.append(a_next)    
    return smpl
    
    
print(gen())            


#функция для группировки элементов отсортированной выборки
def grouping(n, x):
    n_ints = 1 + math.floor(math.log(n, 2)) #число интервалов по Стёрджессу
    width = (math.floor(x[-1])+1-math.floor(x[0]))/n_ints #разбиваем на n_ints равных интервалов
    p = [] #список числа попаданий элементов в каждый интервал
    count = 0 #счётчик
    l_boarder = math.floor(x[0]) #левая граница текущего интервала
    r_boarder = l_boarder + width #правая граница текущего интервала
    int_mids = [] #список с левыми границами (для теоретической вероятности)
    i = 0

    while i < len(x): #обход всей выборки и подсчет попаданий в каждый интервал
        if x[i] >= r_boarder:
            int_mids.append(l_boarder)
            p.append(count)
            count = 0
            l_boarder = r_boarder #если зашли за правую границу,
            r_boarder += width #сдвигаем границы на следующий интервал
        else:
            count += 1
            i+=1
    int_mids.append(l_boarder)
    p.append(count)

    return(int_mids, width, p)

def xi_square(sample):
    alpha = 0.05 #задаем уровень значимости
    
    x, width, p = grouping(len(sample), sample)
    sigma = 0 #статистика Пирсона
    for i in range(len(x)-1):
        theor_prob = dist_func(x[i+1]) - dist_func(x[i]) #теоретическая вероятность попадания элемента в каждый интервал
        sigma += (p[i] - len(sample)*theor_prob)**2/(len(sample)*theor_prob) #формула статистики Пирсона
    theor_prob = dist_func(x[-1]+width) - dist_func(x[-1]) #последний интервал
    sigma += (p[-1] - len(sample)*theor_prob)**2/(len(sample)*theor_prob)

    ta = chi2.ppf(1 - alpha, df=len(x) - 1) #встроенная функция для проверки критерия
    if sigma <= ta: #проверяем, попадает ли значение в критическую область
        print(round(sigma,5), "<=", round(ta,5), "не отвергается")
    else:
        print(round(sigma,5), ">", round(ta,5), "отвергается")

def dist_func(x): #функция распределения равномерного распределения
    f = (x-1)/9999
    return f


sample_lcg = []
for i in range(10):
    arr = []
    for j in range(50):
        arr.append(lcg())
    print(*arr, sep=", ", end="\n\n")
    sample_lcg.append(sorted(arr))

means_lcg = []
sd_lcg = []
var_coef_lcg = []
for arr in sample_lcg:
    mean = sum(arr)/len(arr)
    sd = math.sqrt(sum((x-mean)**2 for x in arr)/len(arr))
    var_coef = sd/mean
    
    means_lcg.append(mean)
    sd_lcg.append(sd)
    var_coef_lcg.append(var_coef)
print(var_coef_lcg)

for arr in sample_lcg:
    xi_square(arr)

sample_lfg = []
for i in range(10):
    arr = []
    for j in range(50):
        arr.append(lfg())  
    print(*arr, sep=", ", end="\n\n")
    sample_lfg.append(sorted(arr))

means_lfg = []
sd_lfg = []
var_coef_lfg = []
for arr in sample_lfg:
    mean = sum(arr)/len(arr)
    sd = math.sqrt(sum((x-mean)**2 for x in arr)/len(arr))
    var_coef = sd/mean
    
    means_lfg.append(mean)
    sd_lfg.append(sd)
    var_coef_lfg.append(var_coef)
print(var_coef_lfg)

for arr in sample_lfg:
    xi_square(arr)

sample_rand = []
for i in range(10):
    arr = []
    for j in range(50):
        arr.append(random.randint(1, 10000))
    sample_rand.append(sorted(arr))

means_rand = []
sd_rand = []
var_coef_rand = []
for arr in sample_rand:
    mean = sum(arr)/len(arr)
    sd = math.sqrt(sum((x-mean)**2 for x in arr)/len(arr))
    var_coef = sd/mean
    
    means_rand.append(mean)
    sd_rand.append(sd)
    var_coef_rand.append(var_coef)
print(var_coef_rand)

temp_arr = [i for i in range(1, 51)]
temp_mean = sum(temp_arr)/len(temp_arr)
temp_sd = math.sqrt(sum((x-temp_mean)**2 for x in temp_arr)/len(temp_arr))
temp_var_coef = temp_sd/temp_mean
print(temp_var_coef)


time_lcg = []
time_lfg = []
time_rand = []

sizes = [1000]
for i in range(100000, 1100000, 100000):
    sizes.append(i)

for size in sizes:
    
    temp_time = time.time()
    for i in range(size):
        lcg()
    time_lcg.append(time.time() - temp_time)
    
    temp_time = time.time()
    for i in range(size):
        lfg()
    time_lfg.append(time.time() - temp_time)
    
    temp_time = time.time()
    for i in range(size):
        random.randint(1, 10000)
    time_rand.append(time.time() - temp_time)

plt.figure(figsize=(10, 8))
plt.title("Зависимость времени генерации от количества элементов")
plt.plot(sizes, time_lcg, label='Первый метод')
plt.plot(sizes, time_lfg, label='Второй метод')
plt.plot(sizes, time_rand, label='Стандартный метод')
plt.xlabel('Количество элементов')
plt.ylabel('Время в секундах')
plt.ticklabel_format(style='plain')
plt.legend()
plt.show()


# In[ ]:





# In[ ]:




