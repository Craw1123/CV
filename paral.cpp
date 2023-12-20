#include <iostream>
#include <vector>
#include <omp.h>
#include <chrono>

void printMatrix(const std::vector<std::vector<double>>& matrix) {
    for (const auto& row : matrix) {
        for (double element : row) {
            std::cout << element << " ";
        }
        std::cout << std::endl;
    }
}

std::vector<std::vector<double>> inverseMatrix(const std::vector<std::vector<double>>& matrix, int n) {
    std::vector<std::vector<double>> augmentedMatrix(n, std::vector<double>(2 * n, 0));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            augmentedMatrix[i][j] = matrix[i][j];
            if (i == j) {
                augmentedMatrix[i][j + n] = 1; 
            }
        }
    }

    auto start_time = std::chrono::steady_clock::now();

    #pragma omp parallel for schedule(static)
    for (int i = 0; i < n; ++i) {
        double pivot = augmentedMatrix[i][i];
        for (int j = 0; j < 2 * n; ++j) {
            augmentedMatrix[i][j] /= pivot;
        }

        #pragma omp simd
        for (int k = 0; k < n; ++k) {
            if (k != i) {
                double factor = augmentedMatrix[k][i];
                for (int j = 0; j < 2 * n; ++j) {
                    augmentedMatrix[k][j] -= factor * augmentedMatrix[i][j];
                }
            }
        }
    }

    auto end_time = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_seconds = end_time - start_time;
    std::cout << "Время выполнения: " << elapsed_seconds.count() << " секунд\n";

    std::vector<std::vector<double>> result(n, std::vector<double>(n, 0));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            result[i][j] = augmentedMatrix[i][j + n];
        }
    }

    return result;
}

std::vector<std::vector<double>> createMatrix(int rows, int cols) {
    std::vector<std::vector<double>> matrix(rows, std::vector<double>(cols, 0.0));
    return matrix;
}

int main() {
    int matrixSize = 1000;
    std::vector<std::vector<double>> inputMatrix = createMatrix(matrixSize, matrixSize);
    std::vector<std::vector<double>> inversed = inverseMatrix(inputMatrix, matrixSize);
    return 0;
}

