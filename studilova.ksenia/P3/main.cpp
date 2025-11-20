#include <iostream>
#include <fstream>
#include <cstdlib>

namespace studilova {

    int saddlePoints(int* matrix, size_t rows, size_t cols)
    {
        int count = 0;

        for (size_t i = 0; i < rows; i++)
        {
            for (size_t j = 0; j < cols; j++)
            {
                int current = matrix[i * cols + j];
                bool isMinInRow = true;
                bool isMaxInCol = true;

                for (size_t k = 0; k < cols; k++)
                {
                    if (matrix[i * cols + k] < current)
                    {
                        isMinInRow = false;
                        break;
                    }
                }

                for (size_t k = 0; k < rows; k++)
                {
                    if (matrix[k * cols + j] > current)
                    {
                        isMaxInCol = false;
                        break;
                    }
                }

                if (isMinInRow && isMaxInCol)
                {
                    count++;
                }
            }
        }

        return count;
    }

    int maxDiagonalSum(int* matrix, size_t rows, size_t cols)
    {
        int maxSum = 0;

        for (size_t d = 0; d < cols; d++)
        {
            int sum = 0;
            size_t i = 0;
            size_t j = d;
            while (i < rows && j < cols)
            {
                sum += matrix[i * cols + j];
                i++;
                j++;
            }
            if (sum > maxSum)
            {
                maxSum = sum;
            }
        }

        for (size_t d = 1; d < rows; d++)
        {
            int sum = 0;
            size_t i = d;
            size_t j = 0;
            while (i < rows && j < cols)
            {
                sum += matrix[i * cols + j];
                i++;
                j++;
            }
            if (sum > maxSum)
            {
                maxSum = sum;
            }
        }

        return maxSum;
    }

    const size_t maxSize = 10000;
    int staticMemory[maxSize];

    int* createMatrix(int mode, size_t rows, size_t cols)
    {
        if (mode == 1)
        {
            return staticMemory;
        }
        else
        {
            return (int*)malloc(rows * cols * sizeof(int));
        }
    }

    bool readMatrix(std::ifstream& input, int* matrix, size_t rows, size_t cols)
    {
        for (size_t i = 0; i < rows * cols; i++)
        {
            input >> matrix[i];
            if (input.fail())
            {
                return false;
            }
        }
        return true;
    }
}

int main(int argc, char* argv[])
{
    using namespace studilova;

    if (argc < 4)
    {
        std::cerr << "Not enough arguments\n";
        return 1;
    }
    else if (argc > 4)
    {
        std::cerr << "Too many arguments\n";
        return 1;
    }

    else if (argv[1][0] < '0' || argv[1][0] > '9')
    {
    std::cerr << "First parameter is not a number\n";
        return 1;
    }
    else if (argv[1][0] != '1' && argv[1][0] != '2')
    {
        std::cerr << "First parameter is out of range\n";
        return 1;
    }

    const char* num = argv[1];
    const char* inputFilename = argv[2];
    const char* outputFilename = argv[3];

    int mode = num[0] - '0';

    std::ifstream inputFile(inputFilename);
    if (!inputFile.is_open())
    {
        std::cerr << "Can not open input file\n";
        return 2;
    }

    int rows = 0;
    int cols = 0;
    inputFile >> rows >> cols;

    if (rows == 0 || cols == 0)
    {
        std::ofstream outputFile(outputFilename);
        outputFile << "0 0\n";
        outputFile.close();
        return 0;
    }

    if (mode == 1 && rows * cols > maxSize)
    {
        std::cerr << "Matrix too big\n";
        return 2;
    }

    int* matrix = createMatrix(mode, rows, cols);

    if (!readMatrix(inputFile, matrix, rows, cols))
    {
        std::cerr << "Invalid matrix data\n";
        if (mode == 2)
        {
            free(matrix);
        }
        return 2;
    }

    inputFile.close();

    int result1 = saddlePoints(matrix, rows, cols);
    int result2 = maxDiagonalSum(matrix, rows, cols);

    std::ofstream outputFile(outputFilename);
    outputFile << result1 << " " << result2 << std::endl;
    outputFile.close();
    
    if (mode == 2)
    {
        free(matrix);
    }

    return 0;
}