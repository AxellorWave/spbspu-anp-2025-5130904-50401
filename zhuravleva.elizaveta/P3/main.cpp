#include <iostream>
#include <fstream>

namespace zhuravleva {
  const size_t Max_size = 10000;

  int* makeMatrix(size_t rows, size_t cols, bool isfixedsize, int* fixed_buffer)
  {
    if (rows == 0 || cols == 0)
    {
      return nullptr;
    }
    if (isfixedsize)
    {
      if (cols > 0 && rows > (Max_size / cols))
      {
        return nullptr;
      }
      return fixed_buffer;
    }
    else
    {
      int* matrix = new int[rows * cols];
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

  void freeMatrix(int* matrix, bool isfixedsize)
  {
    if (matrix && !isfixedsize)
    {
      delete[] matrix;
    }
  }

std::ifstream& readMatrix(std::ifstream& file, int** matrix, size_t& rows, size_t& cols, bool isfixedsize, int* fixed_buffer)
  {
    if (!(file >> rows >> cols))
    {
      *matrix = nullptr;
      return file;
    }

    if (rows == 0 || cols == 0)
    {
      *matrix = nullptr;
      return file;
    }

    if (isfixedsize && (cols > 0 && rows > Max_size / cols))
    {
      *matrix = nullptr;
      return file;
    }

    *matrix = makeMatrix(rows, cols, isfixedsize, fixed_buffer);
    if (!*matrix)
    {
      return file;
    }

    if (!readMatrixElements(file, *matrix, rows, cols))
    {
      if (!isfixedsize) {
        delete[] *matrix;
      }
      *matrix = nullptr;
    }
    return file;
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
  int fixed_buffer[zhuravleva::Max_size];
  std::ifstream file_input(argv[2]);
  if (!file_input) {
    std::cerr << "Error: Cannot open input file.\n";
    return 1;
  }
  zhuravleva::readMatrix(file_input, &matrix, rows, cols, isfixedsize, fixed_buffer);
  if (!file_input || matrix == nullptr) {
    std::cerr << "Error: Invalid matrix data or read error.\n";
    return 2;
  }
  size_t resultForColsNoDublicats = zhuravleva::colsNoDublicats(matrix, rows, cols);
  size_t resultForDiagonalsNoZero = zhuravleva::diagonalsNoZero(matrix, rows, cols);
  std::ofstream file_output(argv[3]);
  if (!file_output) {
    std::cerr << "Error: Cannot open output file.\n";
    zhuravleva::freeMatrix(matrix, isfixedsize);
    return 1;
  }
  file_output << resultForColsNoDublicats << " ";
  file_output << resultForDiagonalsNoZero << "\n";
  zhuravleva::freeMatrix(matrix, isfixedsize);
  matrix = nullptr;
  return 0;
}
