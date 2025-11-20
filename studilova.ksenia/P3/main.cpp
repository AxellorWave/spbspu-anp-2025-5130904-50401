#include <iostream>
#include <fstream>
#include <cstdlib>

namespace studilova {

    int SaddlePoints(int* matrix, size_t rows, size_t cols) {
        int count = 0;
        
        for (size_t i = 0; i < rows; i++) {
            for (size_t j = 0; j < cols; j++) {
                int current = matrix[i * cols + j];
                bool isMinInRow = true;
                bool isMaxInCol = true;
                
                for (size_t k = 0; k < cols; k++) {
                    if (matrix[i * cols + k] < current) {
                        isMinInRow = false;
                        break;
                    }
                }
                
                for (size_t k = 0; k < rows; k++) {
                    if (matrix[k * cols + j] > current) {
                        isMaxInCol = false;
                        break;
                    }
                }
                
                if (isMinInRow && isMaxInCol) {
                    count++;
                }
            }
        }
        
        return count;
    }

    int MaxDiagonalSum(int* matrix, size_t rows, size_t cols) {
        int max_sum = 0;
        
        for (size_t d = 0; d < cols; d++) {
            int sum = 0;
            size_t i = 0, j = d;
            while (i < rows && j < cols) {
                sum += matrix[i * cols + j];
                i++;
                j++;
            }
            if (sum > max_sum) {
                max_sum = sum;
            }
        }
        
        for (size_t d = 1; d < rows; d++) {
            int sum = 0;
            size_t i = d, j = 0;
            while (i < rows && j < cols) {
                sum += matrix[i * cols + j];
                i++;
                j++;
            }
            if (sum > max_sum) {
                max_sum = sum;
            }
        }
        
        return max_sum;
    }

    const size_t max_size = 10000;
    int static_memory[max_size];
    
    int * create_matrix(const char mode, size_t rows, size_t cols) {
        if (mode == '1') {
            return static_memory;
        } else {
            return (int*)malloc(rows * cols * sizeof(int));
        }
    }

    bool read_matrix(std::ifstream& input, int* matrix, size_t rows, size_t cols) {
        for (size_t i = 0; i < rows * cols; i++) {
            input >> matrix[i];
            if (input.fail()) {
                return false;
            }
        }
        return true;
    }
}

int main(int argc, char * argv[]) {

    using namespace studilova;

    if (argc < 4) {
        std::cerr << "Not enought arguments\n";
        return 1;
    }
    else if (argc > 4) {
        std::cerr << "Too many arguments\n";
        return 1;
    }

    else if (argv[1][0] < '0' || argv[1][0] > '9') {
    std::cerr << "First parameter is not a number\n";
        return 1;
    }
    else if (argv[1][0] != '1' && argv[1][0] != '2') {
        std::cerr << "First parameter is out of range\n";
        return 1;
    }

    const char * num = argv[1];
    const char * input_filename = argv[2];
    const char * output_filename = argv[3];

    int mode = num[0] - '0';

    std::ifstream input_file(input_filename);
    if (!input_file.is_open()) {
        std::cerr << "Can not open input file\n";
        return 2;
    }

    int rows, cols;

    if (rows == 0 || cols == 0) {
        std::ofstream output_file(output_filename);
        output_file << "0 0";
        output_file.close();
        return 0;
    }

    if (mode == 1 && rows * cols > 10000) {
        std::cerr << "Matrix too big\n";
        return 2;
    }

    int * matrix = create_matrix(mode, rows, cols);

    if (!read_matrix(input_file, matrix, rows, cols)) {
        std::cerr << "Invalid matrix data\n";
        if (mode == '2') {
            free(matrix);
        }
        return 2;
    }
    
    input_file.close();

    int result1 = SaddlePoints(matrix, rows, cols);
    int result2 = MaxDiagonalSum(matrix, rows, cols);
    
    return 0;
}