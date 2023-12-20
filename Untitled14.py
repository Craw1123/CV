#!/usr/bin/env python
# coding: utf-8

# In[5]:


n = int(input())
paintings = []
for i in range(n):
    a, b = map(int, input().split())
    paintings.append((a, b))

paintings.sort()

max_score = paintings[-1][1]
min_size = paintings[0][0]
max_size = paintings[-1][0]
s = sum(p[1] for p in paintings)

ans = s - (max_size - min_size)
for i in range(n-2, -1, -1):
    max_size = max(max_size, paintings[i+1][0])
    min_size = min(min_size, paintings[i][0])
    s = s - paintings[i+1][1] + max_score
    ans = max(ans, s - (max_size - min_size))

print(ans)


# In[8]:


n = int(input())
a = [int(input()) for i in range(n)]

# функция для проверки, является ли массив отсортированным
def is_sorted(arr):
    return all(arr[i] <= arr[i+1] for i in range(len(arr)-1))

# функция для разворота подотрезка длины 2
def reverse_2(arr, i):
    arr[i:i+2] = arr[i:i+2][::-1]

# функция для разворота подотрезка длины 3
def reverse_3(arr, i):
    arr[i:i+3] = arr[i:i+3][::-1]

# сортировка пузырьком с подсчетом количества операций 1
count = 0
while not is_sorted(a):
    for i in range(n-1):
        if a[i] > a[i+1]:
            if i+2 < n and a[i+2] < a[i]:
                reverse_3(a, i)
            else:
                reverse_2(a, i)
            count += 1
            break

print(count)


# In[ ]:




