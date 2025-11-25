#include <iostream>
#include <fstream>

namespace zhuravleva {
  const size_t Max_size = 10000;
  int fixed_array[Max_size];



  int* makeMatrix(size_t rows, size_t cols, bool isfixedsize)
  {
    if (rows == 0 || cols == 0)
    {
      return nullptr;
    }
    if (isfixedsize)
    {
      return fixed_array;
    }
    else
    {
      int* matrix = new int [rows * cols];
      return matrix;

    }
  }

  bool readMatrixElements(std::ifstream& file, int* matrix, size_t rows, size_t cols)
  {
    for (size_t i = 0; i < rows; i++)
    {
      for (size_t j = 0; j < cols; j++)
      {
        if (!(file >> matrix[i * cols + j]))
        {
          return false;
        }
      }
    }
    return true;
  }

  void freeMatrix(int*& matrix, bool isfixedsize)
  {
    if (matrix)
    {
      if (!isfixedsize)
      { 
        delete[] matrix;
      }
      matrix = nullptr;
    }
  }

  bool readMatrix(const char* filename, int*& matrix, size_t& rows, size_t& cols, bool isfixedsize)
  {
    std::ifstream file(filename);
    if (!file.is_open())
    {
      return false;
    }
    if (file.peek() == std::ifstream::traits_type::eof())
    {
      rows = 0;
      cols = 0;
      matrix = nullptr;
      return false;
    }
    if(!(file >> rows >> cols))
    {
      matrix = nullptr;
      return false;
    }

    if (rows == 0 || cols == 0)
    {
      matrix = nullptr;
      return true;
    }
    if (isfixedsize && (cols > 0 && rows > Max_size / cols))
    {
      return false;
    }
    matrix = makeMatrix(rows, cols, isfixedsize);
    if (!matrix)
    {
      return false;
    }
    if (!readMatrixElements(file, matrix, rows, cols))
    {
      freeMatrix(matrix, isfixedsize);
      return false;
    }
    return true;
  }

  size_t colsNoDublicats(int* matrix, size_t rows, size_t cols)
  {
    if (rows == 0 || cols == 0 || matrix == nullptr)
    {
      return 0;
    }
    size_t count = 0;
    for (size_t j = 0; j < cols; j++)
    {
      bool is_dublics = false;
      for (size_t i = 0; i < rows - 1; i++)
      {
        if (matrix[i * cols + j] == matrix[(i + 1) * cols + j])
        {
          is_dublics = true;
        }
      }
      if (!is_dublics)
      {
        count++;
      }
    }
    return count;
  }
  size_t diagonalsNoZero(int* matrix, size_t rows, size_t cols)
  {
    if (rows == 0 || cols == 0 || matrix == nullptr)
    {
      return 0;
    }
    size_t count = 0;
    for (size_t shift = 1; shift < cols; shift++)
    {
      bool has_zero = false;
      for (size_t i = 0; i < rows && i + shift < cols; i++)
      {
        if (matrix[i * cols + (i + shift)] == 0)
        {
          has_zero = true;
        }
      }
      if (!has_zero)
      {
        count++;
      }
    }

    for (size_t shift = 1; shift < rows; shift++)
    {
      bool has_zero = false;
      for (size_t i = 0; i < cols && i + shift < rows; i++)
      {
        if (matrix[(i + shift) * cols + i] == 0)
        {
          has_zero = true;
        }
      }
      if (!has_zero)
      {
        count++;
      }
    }
    return count;
  }
}

int main(int argc, char* argv[])
{
  if (argc != 4)
  {
    std::cerr << "Need 3 arguments: 1) mode 2) input 3) output\n";
    return 1;
  }
  const char* mode = argv[1];
  if (mode[0] != '1' && mode[0] != '2')
  {
    std::cerr << "Mode must be 1 or 2\n";
    return 1;
  }
  bool isfixedsize = (mode[0] == '1');
  int* matrix = nullptr;
  size_t rows, cols;

  if (!zhuravleva::readMatrix(argv[2], matrix, rows, cols, isfixedsize))
  {
    std::cerr << "Invalid matrix\n";
    return 2;
  }
  size_t resultForColsNoDublicats = zhuravleva::colsNoDublicats(matrix, rows, cols);
  size_t resultForDiagonalsNoZero = zhuravleva::diagonalsNoZero(matrix, rows, cols);
  std::ofstream file_output(argv[3]);
  file_output << resultForColsNoDublicats<< " ";
  file_output << resultForDiagonalsNoZero << "\n";
  zhuravleva::freeMatrix(matrix, isfixedsize);
  return 0;
}
