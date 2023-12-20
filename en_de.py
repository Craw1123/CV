import numpy as np
# Knapsack Encryptor and Decryptor

# Generate a super-increasing knapsack
def generate_knapsack(n):
    knapsack = [1]
    sum_so_far = 1
    for _ in range(n-1):
        new_element = sum_so_far + knapsack[-1]
        knapsack.append(new_element)
        sum_so_far += new_element
    return knapsack

# Generate a private key
def generate_private_key(n):
    private_key = generate_knapsack(n)
    q = private_key[-1] * 2 + 1
    r = (q - 1) // 2
    return private_key, q, r

# Generate a public key
def generate_public_key(private_key, q, r):
    public_key = [(x * r) % q for x in private_key]
    return public_key

# Encrypt a message using the public key
def encrypt(message, public_key):
    ciphertext = sum(public_key[i] for i, bit in enumerate(message) if bit == '1')
    return ciphertext

# Decrypt a ciphertext using the private key
def decrypt(ciphertext, private_key, q, r):
    s = (ciphertext * pow(r, -1, q)) % q
    decrypted_bits = []
    for i in range(len(private_key) - 1, -1, -1):
        if s >= private_key[i]:
            s -= private_key[i]
            decrypted_bits.insert(0, '1')
        else:
            decrypted_bits.insert(0, '0')
    return ''.join(decrypted_bits)


def create_identity_matrix(n):
    matrix = [[0] * n for _ in range(n - 1)]  # Создаем матрицу из нулей размером (n-1) x n

    for i in range(n - 1):
        matrix[i][i] = 1  # Заполняем диагональные элементы единицами

    return matrix

def print_matrix(matrix):
    # Находим максимальную длину элементов в каждом столбце
    column_widths = [max(len(str(element)) for element in column) for column in zip(*matrix)]

    # Выводим матрицу с выравниванием столбцов
    for row in matrix:
        for i, element in enumerate(row):
            print(f"{element:{column_widths[i]}}", end=" ")
        print()  # Переход на новую строку


def reduce_to_row_echelon_form(matrix):
    num_rows = len(matrix)
    num_cols = len(matrix[0])
    lead = 0

    for row in range(num_rows):
        if lead >= num_cols:
            return matrix

        i = row
        while matrix[i][lead] == 0:
            i += 1
            if i == num_rows:
                i = row
                lead += 1
                if num_cols == lead:
                    return matrix

        matrix[i], matrix[row] = matrix[row], matrix[i]

        pivot = matrix[row][lead]
        if pivot != 0:
            for col in range(num_cols):
                matrix[row][col] /= pivot

        for i in range(num_rows):
            if i != row:
                factor = matrix[i][lead]
                for j in range(num_cols):
                    matrix[i][j] -= factor * matrix[row][j]

        lead += 1

    return matrix
    
def create_zero_matrix(rows, columns):
    matrix = []
    for _ in range(rows):
        row = [0] * columns
        matrix.append(row)
    return matrix

def transpose_matrix(matrix):
    # Определяем количество строк и столбцов в матрице
    rows = len(matrix)
    cols = len(matrix[0])

    # Создаем новую матрицу, где строки и столбцы будут поменяны местами
    transposed_matrix = [[0 for _ in range(rows)] for _ in range(cols)]

    # Проходим по исходной матрице и записываем элементы в новую матрицу
    for i in range(rows):
        for j in range(cols):
            transposed_matrix[j][i] = matrix[i][j]

    return transposed_matrix


def subtract_vectors(vector1, vector2):
    result = []
    if len(vector1) == len(vector2):
        for i in range(len(vector1)):
            result.append(vector1[i] - vector2[i])
    else:
        print("Error: Vectors must have the same length")
    return result
    
def multiply_vector_by_scalar(vector, scalar):
    result = []
    for element in vector:
        result.append(element * scalar)
    return result

#ортогонализации Грама-Шмидта
def dot_product(v1, v2):
    """Compute the dot product of two vectors."""
    result = 0.0
    for i in range(len(v1)):
        result += v1[i] * v2[i]
    return result

def norm_squared(vector):
    """Compute the squared norm of a vector."""
    result = 0.0
    for i in range(len(vector)):
        result += vector[i] ** 2
    return result
    
def GS_D(M):
    n = len(M[0])
    X = [M[0]]  # x0 = b0
    Y = create_zero_matrix(len(M),len(M[0]))
    for j in range(1, n):
        xj = M[j]
        for i in range(j):
            Y[i][j] = dot_product(X[i], M[j]) / norm_squared(X[i])
            xj = subtract_vectors(xj,multiply_vector_by_scalar(X[i],Y[i][j]))
        X.append(xj)

    return X, Y


import math

def round_to_nearest(x):
    # Round a number to the nearest integer
    return int(round(x))
def  LLL_Algorithm_D(M):
    while True:
        X,Y = GS_D(M);
        n = len(M)
        print_matrix(transpose_matrix(M))
        for j in range(1,n):
           # print("here1")
            for i in range(j - 1, -1, -1):
                if abs(Y[i][j]) > 0.5:
                    #print(Y[i][j], "> 0.5")
                    promeshutochniy = multiply_vector_by_scalar(M[i], math.floor(Y[i][j] + 0.5)) #  (yij + 1/2) * bi
                    #print("i =",i,"j =",j,"Y =",Y[i][j],math.floor(Y[i][j] + 0.5))
                    promeshutochniy = subtract_vectors(M[j],promeshutochniy) # bj − yij + 1/2) * bi
                    M[j] = promeshutochniy
                    
        print("\n")
        print_matrix(transpose_matrix(M))
        print("\n")
        #print("\n")
        #print_matrix(M)
        #print("\n")
        X,Y = GS_D(M);
        smth = 0
        for j in range(n-1):
           # print("here2")
           # print(subtract_vectors(X[j+1],multiply_vector_by_scalar(X[j],Y[j][j+1])))
           # print(norm_squared(X[j]) * (3/4))
           # print(X[j])
           # print(norm_squared(subtract_vectors(X[j+1],multiply_vector_by_scalar(X[j],Y[j][j+1]))))
            f = norm_squared(subtract_vectors(X[j+1],multiply_vector_by_scalar(X[j],Y[j][j+1]))) # ||X(j+1) + Y(j,j+1)X(j)||^2
            s = norm_squared(X[j]) * (3/4) # 3/4||X(j)||^2
            if f<s :
                print("f<s")
                ABC = M[j]
                M[j] = M[j+1]
                M[j+1] = ABC
               # print("j before break is",j)
                smth = 1
                break
            else:
                smth = 0
       # print("afret break i should be here and smth = ",smth)
        if smth == 0:
           # print("Return M")
            return M
                
# Example usage
message = '101100111'
private_key, q, r = generate_private_key(len(message))
#private_key = [2, 5, 9, 21, 45, 103, 215, 450, 946]
#q = 2003
#r = 1289
public_key = generate_public_key(private_key, q, r)
ciphertext = encrypt(message, public_key)
decrypted_message = decrypt(ciphertext, private_key, q, r)

print("private_key:",private_key)
print("modul:",q,"mnoshitel:",r)
print("Original message:", message)

print("Decrypted message:", decrypted_message,"\n")

print("Ciphertext:", ciphertext)
print("public_key",public_key)



line = public_key
line.append(-(ciphertext))
matrix = create_identity_matrix(len(line))
matrix.append(line)
print("исходная матрица")
print_matrix(matrix)
#print(type(matrix),type(matrix[0]))
print("\n\n")
transposed_matrix = transpose_matrix(matrix) # для удобного получение векторов-столбцов матрицы
#X,Y = GS_D(transposed_matrix)
print("\n")
check = LLL_Algorithm_D(transposed_matrix)
#print_matrix(check)
print("\n")
print_matrix(transpose_matrix(check))
print("\n")
#print(matrix)
#test = GS_D(transpose_matrix)
#print_matrix(test[0])
#print(LLL_Algorithm_D(transpose_matrix))
#LLL_Algorithm(transpose_matrix)
#res_matrix = reduce_to_row_echelon_form(matrix)
#print("готовая матрица")
#print_matrix(res_matrix)
