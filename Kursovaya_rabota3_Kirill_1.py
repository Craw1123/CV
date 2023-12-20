#!/usr/bin/env python
# coding: utf-8

# In[1]:


import pandas as pd
import numpy as np
import random
import math
import seaborn as sns
import scipy
get_ipython().run_line_magic('matplotlib', 'inline')
import matplotlib.pyplot as plt
import warnings
warnings.filterwarnings('ignore')


# ## Первое задание

# $1.1$ Моделирование Пуассоновской случайных величин 
# 
# Предположим мы имеем случайную величину y~u[0,1]. Её легко превратить в биноминальное распределение, через распределение Бернулли. Вероятность успеха p $\in$ (0,1) будет делить отрезок [0,1] на [0,0] и (0,1], с помощью которых и получаем бернулевскую случайную величину. Пуассоновская случайная величина есть биноминальная с параметром (n,$\frac{λ}{n}$), поэтому при моделировании необходимо учесть параметр n и сопоставить мощность ЭВМ, самое точное моделирование при n$\rightarrow\infty$ и зависимость от n уже теряется. В нашем случае n взять как 100000. Необходимость как уже сказано выше , в применении n операций теряется лишь  на бесконечности, для этого просто воспользоваться уже самой плотностью Пуассоновской случайной величины, например np.random.Poisson(λ)

# In[4]:


#Превращение равномерного распределения на [0,1] в распределение Бернулли
def BernulliDist(p):
    y = np.random.random()
    if y<p:
        s = 1
    else:
        s = 0
    return s

_lambda=7.5
#биномиальное распределение
n=100000
def BinomialDist(n, p):
    return sum([BernulliDist(p) for j in range(n)])


#Пуассоновское распределение
def PoissonDist(_lambda):
    return BinomialDist(n, _lambda/n)

#Добавление расчетов для разных выборок в список и их отрисовка
PoisSamples = []
sample_sizes = [5, 10, 100, 200, 400, 600, 800, 1000]
for i in range(len(sample_sizes)):
    PoisSamples.append([PoissonDist(_lambda) for k in range(sample_sizes[i])])


fig, ax = plt.subplots(nrows=4, ncols=2, figsize=(20,15))
for i in range(len(sample_sizes)):
    sns.histplot(
        PoisSamples[i],
        ax=ax[i//2, i%2],
        bins=max(PoisSamples[i])
    )
    ax[i//2, i%2].set_title('Выборка из {} элементов'.format(sample_sizes[i]))
plt.show()


# $1.2$ Треугольное распределение
# 
# $\begin{equation}
# F(x) = 
#     \begin{cases}
#       \frac{2x}{θ},\text {если x$\in[0,\theta]$} \newline
#       \frac{2(1-x)}{1-θ}, \text {если x$\in(0,1]$} \newline
#       0, иначе
#     \end{cases}
# \end{equation}\$\
# Для моделирования такой случайной велчины с помощью случайного числа от 0 до 1, воспользуемся обратным преобразованием.
# 
# Найдем F(x):\
# $F(\alpha)=\int\frac{2x}{θ}dx=\frac{2}{\theta}\int xdx=\frac{2}{\theta}\cdot\frac{x^2}{2}=\frac{x^2}{\theta}$ для x$\in$[0,0] \
# F(x)=$\int\frac{2(1-x)}{1-\theta}dx=\frac{2}{1-\theta}\cdot (-\int(1-x)d(1-x))=-\frac{(1-x)^2}{1-\theta}+C, $ для x$\in$(0,1] \ 
# Так как это функция распределения случайной величины и в конечной точке полуинтервал а должна быть равной одному => C=1 
# 
# Итого
# 
# 
#   $\begin{equation}
# F(x) = 
#     \begin{cases}
#           0, x< 0 \newline
#       \frac{x^2}{θ},\text { x$\in[0,\theta]$} \newline
#       1- \frac{(1-x)^2}{1-θ}, \text { x$\in(0,1]$} \newline
#       1, x>1
#     \end{cases}
# \end{equation}\$ 
# 
# Выводим обратную $F^{-1}(y)$\
# F(x)= $\frac{x^2}{\theta}=>x=\sqrt{θ\cdot F(x)}$ 
# 
# F(x)=$1- \frac{(1-x)^2}{1-θ}=> x=1-\sqrt{(1-\theta)\cdot (1-F(x))}$
# 
# 
# $\begin{equation}
# F^{-1}(x) = 
#     \begin{cases}
#           \sqrt{\theta \cdot y}, y\in[0,\theta] \newline
#       1-\sqrt{(1-\theta)\cdot (1-x)}, y\in [\theta,1] \newline
#     \end{cases}
# \end{equation}\$

# In[5]:


#Функция треугольного распределения
def TriangularDist(_theta):
    y=np.random.random()
    if y < 0 or y > 1:
        return 0
    elif y < _theta:
        return np.sqrt(_theta * y)
    else:
        return 1 - np.sqrt((1-_theta)*(1-y))
    
TriangularSamples = []
sample_sizes = [5, 10, 100, 200, 400, 600, 800, 1000]

# Генерируем треугольную выборку
_theta = 0.55
for i in range(len(sample_sizes)):
    TriangularSamples.append([TriangularDist(_theta) for j in range(sample_sizes[i])])
        
#отрисовка
fig, ax = plt.subplots(nrows=4, ncols=2, figsize=(20,15))
for i in range(len(sample_sizes)):
    sns.histplot(
        TriangularSamples[i],
        ax=ax[i//2, i%2],
        bins=20
    )
    ax[i//2, i%2].set_title('Выборка из {} элементов'.format(sample_sizes[i]))
plt.show()


# $Метод\ моментов$

# $1.3$ По методу моментов, k-ый момент случайной величины можно записать в виде:
# 
# $E(x^k)=f(\theta)$, где f($\theta$) - функция от неизвестного параметра.
# 
# Выразив $\theta$ (если это возможно, т.к. первый момент может оказаться константой), мы получим  оценку с помощью метода моментов.
# 
# По закону больших чисел, любое среднее, в том числе и выборочное, сходится к своему математическому ожиданию.
# 
# Первый момент есть математическое ожидание.

# $1.4$ Для Пуассона:
# 
# E(x)=$\lambda =>$выборочное среднее есть оценка для f$(\lambda)=\lambda$

# In[6]:


for sample in PoisSamples:
    print("Оценка параметра лямбда на выборке из {} элементов -".format(len(sample)), np.array(sample).mean())


# $1.5$ Для треугольного  распределения:
# 
# E(x)=$\frac{1+\theta}{3}=>$выборочное среднее будет оценкой для $f(\theta)=\frac{1+\theta}{3}=> T(x)=3\hat{x}-1$ будет оценкой для $f(\theta)=\theta$  

# In[7]:


for sample in TriangularSamples:
    print("Оценка параметра лямбда на выборке из {} элементов -".format(
        len(sample)), 3  * np.array(sample).mean() - 1)


# $1.6$ По методу максимального правдоподобия 
# 
# Введем функцию правдоподобия:
# 
# $L(\theta|x_1.....x_n)=P(x_1|\theta)\cdot P(x_2|\theta)\cdot .....\cdot P(x_n|\theta)$ - для дискретного случая
# 
# $L(\theta|x_1.....x_n)=f(x_1|\theta)\cdot f(x_2|\theta)\cdot.....\cdot f(x_n|\theta)$ - для непрерывного случая
# 
# Где P-функция вероятности, а f-функция плотности распределения случайной величины. Также считаем, что все n наблюдений были независимы и одинаковы распределены.
# 
# Суть метода заключается в максимизации функции:
# 
# $L(\theta|x_1.....x_n)\underset{\theta}{\rightarrow} max$, тогда:
# 
# $\hat{\theta}=argmax(L(\theta|x_1.....x_n))$ будет оценкой нашего параметра $\theta$

# $1.7$ Для Пуассона:
# 
# $L(\lambda|x_1.....x_n)=\frac{\lambda^{x_1}}{x_1!}\cdot e^{-\lambda}\cdot .....\cdot \frac{\lambda^{x_n}}{x_n!}=e^{-\lambda n}\prod_{i=1}^{n}\frac{\lambda^{x_i}}{x_i!}$
# 
# Можно рассмотреть функцию $\ln{L}$, т.к. $y=\ln{x}$ неубывающая монотонная функция, экстремум останутся в тех же точках 
# 
# $\ln{L}=\ln{(e^{-\lambda n}\prod_{i=1}^{n}\frac{\lambda^{x_i}}{x_i!})}=\ln{e^{-\lambda n}}+\ln{(\prod_{i=1}^{n}\frac{\lambda^{x_i}}{x_i!})}=-\lambda n+\sum_{i=1}^{n}\ln{(\frac{\lambda^{x_i}}{x_i!})}=-\lambda n +\sum_{i=1}^{n}(x_i\ln{(\lambda)}-\ln{(x_i!)}) $
# 
# Максимум будем искать с помощью производной:
# 
# $\frac{\partial \ln{L}}{\partial \lambda}=-n+\sum_{i=1}^{n}(x_i\cdot \frac{1}{\lambda}-0)=-n+\frac{1}{\lambda}\sum_{i=1}^{n}x_i=0 \\ 
# \frac{1}{\lambda}\sum_{i=1}^{n}x_i=n=>\lambda=\frac{1}{n}\sum_{i=1}^{n}x_i=\hat{x}-экстремум$
# 
# Вторая производная $\frac{\partial^2 \ln{L}}{\partial \lambda}=-\sum x_i\cdot \frac{1}{\lambda^2}$ всегда отрицательная для наших $x_i$ и $\lambda(x\geq0,\lambda \geq0)=>$ точка максимума $\hat{x}$
# 
# $T(x)=\hat{x}$ оценка ММП для $\lambda$

# $1.8$ В питоне реализуем это с помощью численных методов и библиотеки Scipy

# In[8]:


#По методу максиамльного правдоподобия

from scipy.optimize import minimize
from scipy.special import factorial

def MLE(_lambda, x):
    x=np.array(x)
    n=x.size
    l = - _lambda * n + np.sum(x * np.log(_lambda) - np.log(factorial(x)))
    return -l

_lambda_start = 4
for sample in PoisSamples:
    print("Оценка параметра лямбда на выборке из {} элементов -".format(
        len(sample)), round(minimize(MLE, _lambda_start, args=sample).x[0], 4))


# $1.9$ 

# 

# In[9]:


def MLE_triangular(_theta, x):
    #n = x.size
    s1 = []
    s2 = []
    k=0
    m=0
    s1_mult = 1
    s2_mult = 1
    for elem in x:
        if  0 <= elem <= _theta:
            k+=1
            s1.append(elem)
        elif _theta <= elem <= 1:
            m+=1
            s2.append(elem)
        else:
            return 0
    for elem1 in s1:
        s1_mult *= elem1
    for elem2 in s2:
        s2_mult *= (1 - elem2)
    l = ((2 / _theta) ** k) * s1_mult * ((2 / (1 - _theta)) ** m) * s2_mult
    
    return  -l

_theta_start = 0.55
for sample in TriangularSamples:
    print("Оценка параметра тетта на выборке из {} элементов -".format(
        len(sample)), round(minimize(MLE_triangular, _theta_start, args=sample).x[0], 4))


# ## Второе задание

# $2.1$ Попробуем найти оптимальную оценку для распределения Пуассона
# 
# x~Pois$(\lambda)$ 
# 
# P{x=k}=$\frac{\lambda^k}{k!}\cdot e^{-\lambda}$
# 
# 1) Воспользуемся достаточной статистикой 
# 
# Статистика S(x) называется достаточной относительно параметра $\theta$, если фунцкия правдоподобия $L(x,\theta|S(x)=S)$ не зависит от $\theta$ не при каких S
# 
# Найти ее можно через критерий факторизации, который выглядит следующим образом:
# 
# 

# 1.10 продолжение фото

# $1.11 фото$

# In[120]:


_lambda0 = 7.5
_lambda1 = 7.6
n=10000
pois=np.random.poisson(_lambda1, size=(n, n))
s = pois.sum(axis=1)


# In[137]:


t = (np.log(_lambda1/_lambda0) * s - n * (_lambda1 - _lambda0)) 


# In[153]:


plt.figure(figsize=(20,7))
#sns.histplot(t)
sns.distplot(t, kde = True, hist=True, fit = scipy.stats.norm)
plt.show()


# Хоть мы и смогли выразить наше распределение (lnl(х)) через среднее, но цпт выше подтверждает нашу гипотезу у нормальности распределения
