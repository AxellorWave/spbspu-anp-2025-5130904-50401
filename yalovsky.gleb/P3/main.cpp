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

}

int main()
{
  return 0;
}
