#!/usr/bin/env python
# coding: utf-8

# In[5]:


text = input("TEXT:")
key1 = input("KEY1:")
key2 = input("KEY2:")
finaltext = ""
alphabet = ['а', 'б', 'в', 'г', 'д', 'е', 'ё', 'ж', 'з', 'и', 'й', 'к', 'л', 'м', 'н', 'о', 'п', 'р', 'с', 'т', 'у', 'ф', 'х', 'ц', 'ч', 'ш', 'щ', 'ъ', 'ы', 'ь', 'э', 'ю', 'я']


for symbol in text:
    index = alphabet.index(symbol)
    if index >= 0:
        print(symbol,"->",index,"->",((index * int(key1) + int(key2)) % (len(alphabet))),alphabet[(index * int(key1) + int(key2)) % (len(alphabet))])
        finaltext += alphabet[(index * int(key1) + int(key2)) % (len(alphabet))]

print(finaltext)


# In[20]:


from random import choice
k = [23,25]
planeText = input()
A = {'а': 0, 'б': 1, 'в': 2, 'г': 3, 'д': 4, 'е': 5, 'ё': 6,
    'ж': 7, 'з': 8, 'и': 9, 'й': 10, 'к': 11, 'л': 12,
    'м': 13, 'н': 14, 'о': 15, 'п': 16, 'р': 17, 'с': 18,
    'т': 19, 'у': 20, 'ф': 21, 'х': 22, 'ц': 23, 'ч': 24,
    'ш': 25, 'щ': 26, 'ъ': 27, 'ы': 28, 'ь': 29, 'э': 30,
    'ю': 31, 'я': 32}
X = []
gamma = [choice(k) for i in range(len(planeText))]
y = []
n = 33
for i in planeText:
    X.append(A[i])
for i in range(len(X)):
    y.append((X[i] + gamma[i]) % n)
cipherText = []
for i in y:
    for j in A.keys():
        if i == A[j]:
            cipherText.append(j)
            break
firstColumn = []
secondColumn = []
def clmnAp(k):
    column = []
    for j in A.keys():
        if (i - k) % n == A[j]:
            column.append(j)
            break
    return column
for i in y:
    firstColumn += clmnAp(k[0])
    secondColumn += clmnAp(k[1])
def forPrint(lst):
    for i, el in enumerate(lst):
        if i == len(lst) - 1:
            print(el, "\\\\")
        else:
            print(el, " & ", end = "")
print("Символьная и цифровая строки исходного открытого текста")
forPrint(planeText)
forPrint(X)
print("Цтфровая строка гаммы наложения")
forPrint(gamma)
print("Цифровая и символьная строки шифртекста")
forPrint(y)
forPrint(cipherText)
print("Символьные колонки для зигзагообразного чтения, соответствующие знакам шифртекста")
forPrint(firstColumn)
forPrint(secondColumn)


# In[21]:


k = [23, 26, 30] # множество к из которого выбирается гамма (3.2)
gamma = [choice(k) for i in range(len(planeText))] # массив из цифровой строки
# гаммы наложения (3.2)
y = [] # массив цифровых эквивалентов символов шифртекста (3.2)
for i in range(len(X)):
    y.append((X[i] + gamma[i]) % n)
cipherText = [] # массив символов шифртекста (3.2)
for i in y:
    for j in A.keys():
        if i == A[j]:
            cipherText.append(j)
            break
firstColumn = [] # первая колонка для <<зигзагообразного чтения>> (3.2)
secondColumn = [] # вторая колонка для <<зигзагообразного чтения>> (3.2)
thirdColumn = [] # третья колонка для <<зигзагообразного чтения>> (3.2)
for i in y:
    firstColumn += clmnAp(k[0])
    secondColumn += clmnAp(k[1])
    thirdColumn += clmnAp(k[2])
def forPrint(lst):
    for i, el in enumerate(lst):
        if (i + 1) % 10 == 0:
            print(el, "\\\\")
        else:
            print(el, " & ", end = "")
print("Символьная и цифровая строки исходного открытого текста")
forPrint(planeText)
forPrint(X)
print("Цтфровая строка гаммы наложения")
forPrint(gamma)
print("Цифровая и символьная строки шифртекста")
forPrint(y)
forPrint(cipherText)
print("Символьные колонки для зигзагообразного чтения, соответствующие знакам шифртекста")
forPrint(firstColumn)
forPrint(secondColumn)
forPrint(thirdColumn)


# In[17]:


A = {'а': 0, 'б': 1, 'в': 2, 'г': 3, 'д': 4, 'е': 5,
    'ё': 6, 'ж': 7, 'з': 8, 'и': 9, 'й': 10, 'к': 11,
    'л': 12, 'м': 13, 'н': 14, 'о': 15, 'п': 16, 'р': 17,
    'с': 18, 'т': 19, 'у': 20, 'ф': 21, 'х': 22, 'ц': 23,
    'ч': 24, 'ш': 25, 'щ': 26, 'ъ': 27, 'ы': 28, 'ь': 29,
    'э': 30, 'ю': 31, 'я': 32}
n = 33
planeText1 = input()
planeText2 = input()
X1 = []
X2 = []
y1 = []
y2 = []
k = [23, 25]
gamma = [choice(k) for i in range(len(planeText1))]
def forX(planeText):
    X = []
    for i in planeText:
        X.append(A[i])
    return X
X1 += forX(planeText1)
X2 += forX(planeText2)
def forY(X):
    y = []
    for i in range(len(X)):
        y.append((X[i] + gamma[i]) % n)
    return y
y1 += forY(X1)
y2 += forY(X2)
cipherText1 = []
cipherText2 = []
def forCipherText(y):
    cipherText = []
    for i in y:
        if str(i).isdigit():
            for k, j in A.items():
                if i == j:
                    cipherText += k
                    break
        else:
            cipherText += i
    return cipherText
cipherText1 += forCipherText(y1)
cipherText2 += forCipherText(y2)
signDifference = [y2[i] - y1[i] for i in range(len(y1))]
def forPrint(lst):
    for i, el in enumerate(lst):
        if i == len(lst) - 1:
            print(el, "\\\\")
        else:
            print(el, " & ", end = "")
print("Символьная и цифровая строки первого открытого текста")
forPrint(planeText1)
forPrint(X1)
print("Цифровая строка знаков гаммы")
forPrint(gamma)
print("Цифровая и символьная строки второго открытого текста")
forPrint(X2)
forPrint(planeText2)
print("Символьная и цифровая строки первого шифртекста")
forPrint(cipherText1)
forPrint(y1)
print("Символьная и цифровая строки второго шифртекста")
forPrint(cipherText2)
forPrint(y2)
print("Цифровая строка разности знаков второго и первого шифртекста")
forPrint(signDifference)


# In[19]:


planeText = input()
gamma = input()
XGamma = []
for i in gamma:
    XGamma.append(A[i])
strPlaneText = []
XPlaneText = []
for i in planeText:
    strPlaneText.append(i)
    XPlaneText.append(A[i])
y = []
for i in range(len(XPlaneText)):
    y.append((XPlaneText[i] + XGamma[i]) % n)
cipherText = []
for i in y:
    if str(i).isdigit():
        for k, j in A.items():
            if i == j:
                cipherText += k
                break
    else:
        cipherText += i
def forPrint(lst):
    for i, el in enumerate(lst):
        if i == len(lst) - 1:
            print(el, "\\\\")
        else:
            print(el, " & ", end = "")
print("Символьная и цифровая строки знаков гаммы")
forPrint(gamma)
forPrint(XGamma)
print("Символьная и цифровая строки открытого текста")
forPrint(strPlaneText)
forPrint(XPlaneText)
print("Символьная и цифровая строки шифртекста")
forPrint(cipherText)
forPrint(y)


# In[11]:


A = {'а': 0, 'б': 1, 'в': 2, 'г': 3, 'д': 4, 'е': 5,
'ё': 6, 'ж': 7, 'з': 8, 'и': 9, 'й': 10, 'к': 11,
'л': 12, 'м': 13, 'н': 14, 'о': 15, 'п': 16, 'р': 17,
'с': 18, 'т': 19, 'у': 20, 'ф': 21, 'х': 22, 'ц': 23,
'ч': 24, 'ш': 25, 'щ': 26, 'ъ': 27, 'ы': 28, 'ь': 29,
'э': 30, 'ю': 31, 'я': 32}
n = 33
gamma = input()
XGamma = []
strGamma = []
for i in gamma:
    strGamma.append(i)
    XGamma.append(A[i])
plainText = input()
XPlainText = []
strPlainText = []
for i in plainText:
    strPlainText.append(i)
    XPlainText.append(A[i])
cipherText = []
XCipherText = []
strCipherText = []
YGamma = []
for i in range(len(XPlainText)):
    YGamma.append((XPlainText[i] + XGamma[i % len(XGamma)]) % n)
for i in YGamma:
    if str(i).isdigit():
        for k, j in A.items():
            if i == j:
                cipherText += k
                break
    else:
        cipherText += i
def forPrint(lst):
    for i, el in enumerate(lst):
        if i == len(lst) - 1:
            print(el, "\\\\")
        else:
            print(el, " & ", end = "")
print("символьная и цифровая строки исходного открытого текста")
forPrint(plainText)
forPrint(XPlainText)
print("цифровая и смивольная (лозунг) строки гаммы наложения")
forPrint(XGamma)
forPrint(gamma)
print("цифровая и символьная строки шифртекста")
forPrint(YGamma)
forPrint(cipherText)


# In[ ]:




