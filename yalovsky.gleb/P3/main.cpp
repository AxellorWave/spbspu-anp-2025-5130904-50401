#include <iostream>
#include <fstream>
#include <cstdlib>

namespace yalovsky{

  bool isNumber(const char * str)
  {
    if (!str || *str == '\0') {
      return false;
    }
    for (size_t i = 0; str[i] != '\0'; i++) {
      if (str[i] < '0' || str[i] > '9') {
        return false;
      }
    }
    return true;
  }

  void readMatrix(std::istream & in, int * matrix, size_t rows, size_t cols)
  {
    for (size_t i = 0; i < rows * cols; i++) {
      in >> matrix[i];
      if(!in) {
       throw std::runtime_error("Invalid matrix data");
      }
    }
  }

  void writeMatrix(std::ostream & out, const int * matrix, size_t rows, size_t cols)
  {
    out << rows << " " << cols;
    for (size_t i = 0; i < rows * cols; i++) {
      out << " " << matrix[i];
    }
    out << "\n";
  }

  void copyMatrix(const int * source, int * dest, size_t rows, size_t cols)
  {
    for (size_t i = 0; i < rows * cols; i++) {
      dest[i] = source[i];
    }
  }

  void spiralTransformLFTTOPCLK(int * matrix, size_t rows, size_t cols)
  {
    if (rows == 0 || cols == 0) {
      return;
    }

    int count = 1;
    size_t top = 0;
    size_t bottom = rows - 1;
    size_t left = 0;
    size_t right = cols - 1;

    while (top <= bottom && left <= right) {
      for (size_t i = left; i <= right; i++) {
        matrix[top * cols + i] -= count++;
      }
      top++;

      for (size_t j = top; j <= bottom; j++) {
        matrix[j * cols + right] -= count++;
      }
      right--;

      if (top <= bottom) {
        for (size_t i = right; i >= left; i--) {
          matrix[bottom * cols + i] -= count++;
        }
        bottom--;
      }

      if (left <= right) {
        for (size_t i = bottom; i >= top; i--) {
          matrix[i * cols + left] -= count++;
        }
        left++;
      }
    }
  }

  void spiralTransformLFTBOTCNT(int * matrix, size_t rows, size_t cols)
  {
    if (rows == 0 || cols == 0) {
      return;
    }

    int count = 1;
    size_t top = 0;
    size_t bottom = rows - 1;
    size_t left = 0;
    size_t right = cols - 1;

    while (top <= bottom && left <= right) {
      for (size_t i = left; i <= right; i++) {
        matrix[bottom * cols + i] += count++;
      }
      bottom--;

      for (size_t j = bottom; j >= top; j--) {
        matrix[j * cols + right] += count++;
      }
      right--;

      if (top <= bottom) {
        for (size_t i = right; i >= left; i--) {
          matrix[top * cols + i] += count++;
        }
        top++;
      }
      if (left <= right) {
        for (size_t i = top; i <= bottom; i++){
          matrix[i * cols + left] += count++;
        }
        left++;
      }
    }
  }
}

int main(int argc, char ** argv)
{
    if (argc != 4) {
    std::cerr << (argc < 4 ? "Not enough arguments\n" : "Too many arguments\n");
    return 1;
  }

  if (!yalovsky::isNumber(argv[1])) {
    std::cerr << "First parameter is not a number\n";
    return 1;
  }

  char* endptr;
  long mode = std::strtol(argv[1], &endptr, 10);
  if (*endptr != '\0' || (mode != 1 && mode != 2)) {
    std::cerr << "First parameter is out of range\n";
    return 1;
  }

  std::ifstream input(argv[2]);
  if (!input.is_open()) {
    std::cerr << "Cannot open input file: " << argv[2] << "\n";
    return 2;
  }

  std::ofstream output(argv[3]);
  if (!output.is_open()) {
    std::cerr << "Cannot open output file: " << argv[3] << "\n";
    return 2;
  }

  size_t rows = 0;
  size_t cols = 0;

  input >> rows >> cols;

  if (!input) {
    std::cerr << "Invalid input\n";
    return 2;
  }

  const size_t MAX_STATIC_SIZE = 10000;
  if (mode == 1 && rows * cols > MAX_STATIC_SIZE) {
    std::cerr << "Matrix too large for static alloc\n";
    return 2;
  }

  try {
    if (mode == 1) {
      int staticMatrix[MAX_STATIC_SIZE];
      yalovsky::readMatrix(input, staticMatrix, rows, cols);

      int staticCopy[MAX_STATIC_SIZE];
      yalovsky::copyMatrix(staticMatrix, staticCopy, rows, cols);

      yalovsky::spiralTransformLFTTOPCLK(staticMatrix, rows, cols);
      output << "LFT-TOP-CLK result:\n";
      yalovsky::writeMatrix(output, staticMatrix, rows, cols);

      yalovsky::spiralTransformLFTBOTCNT(staticCopy, rows, cols);
      output << "LFT-BOT-CNT result:\n";
      yalovsky::writeMatrix(output, staticCopy, rows, cols);

    } else {
      int * dynamicMatrix = (int*)std::malloc(rows * cols * sizeof(int));
      if (!dynamicMatrix) {
        std::cerr << "Memory alloc failed\n";
        return 2;
      }

      yalovsky::readMatrix(input, dynamicMatrix, rows, cols);

      int * dynamicCopy = (int*)std::malloc(rows * cols * sizeof(int));
      if (!dynamicCopy) {
        std::cerr << "Memory alloc failed\n";
        std::free(dynamicMatrix);
        return 2;
      }
      yalovsky::copyMatrix(dynamicMatrix, dynamicCopy, rows, cols);

      yalovsky::spiralTransformLFTTOPCLK(dynamicMatrix, rows, cols);
      output << "LFT-TOP-CLK result:\n";
      yalovsky::writeMatrix(output, dynamicMatrix, rows, cols);

      yalovsky::spiralTransformLFTBOTCNT(dynamicCopy, rows, cols);
      output << "LFT-BOT-CNT result:\n";
      yalovsky::writeMatrix(output, dynamicCopy, rows, cols);

      std::free(dynamicCopy);
      std::free(dynamicMatrix);
    }
  } catch (const std::exception& e) {
    std::cerr << "Invalid input - invalid matrix data\n";
    return 2;
  }
  return 0;
}
