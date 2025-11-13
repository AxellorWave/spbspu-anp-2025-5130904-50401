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
    for (size_t i = 0; i <= rows * cols; i++) {
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
        for (size_t i = right; i <= bottom; i++) {
          matrix[bottom * cols + i] -= count++;
        }
        bottom--;
      }

      if (left <= right) {
        for (size_t i = bottom; i >= top && i <= rows - 1; i++) {
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

      for (size_t j = bottom; j >= top && j <= rows - 1; j--) {
        matrix[j * cols + right] += count++;
      }
      right--;

      if (top <= bottom) {
        for (size_t i = right; i >= left && i <= cols - 1; i--) {
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

int main()
{
  return 0;
}
