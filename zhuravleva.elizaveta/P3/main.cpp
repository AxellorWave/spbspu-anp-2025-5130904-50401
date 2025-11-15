#include <iostream>
#include <fstream>
#include <string>

namespace zhuravleva {
  const size_t Max_size = 10000;
  bool isNumber(const std::string& str)
  {
    if (str.empty())
    {
      return false;
    }
    for (char c : str)
    {
      if (!isdigit(c))
      {
        return false;
      }
    }
    return true;
  }

  bool readMatrix(const std::string& filename, int**& matrix, size_t& rows, size_t& cols, bool isfixedsize)
  {
    std::ifstream file(filename);
    file >> rows >> cols;
    if (!file.is_open())
    {
      return false;
    }
    if (isfixedsize && (rows > Max_size/cols))
    {
      return false;
    }
    matrix = new int* [rows];
    for (size_t i = 0; i < rows; i++)
    {
      matrix[i] = new int[cols];
      for (size_t j = 0; j < cols; j++)
      {
          if (!(file >> matrix[i][j]))
          {
            return false;
          }
      }
    }
    return true;
  }

  void freeMatrix(int**& matrix, size_t rows)
  {
    if (matrix)
    {
      for (size_t i = 0; i < rows; i++)
      {
        delete[] matrix[i];
      }
      delete[] matrix;
      matrix = nullptr;
    }
  }

    size_t cols_no_dublicats(int ** matrix, size_t rows, size_t cols)
    {
      size_t count = 0;
      for (size_t j = 0; j < cols; j++)
      {
        bool is_dublics = false;
        for (size_t i = 0; i < rows - 1; i++)
        {
          if (matrix[i][j] == matrix[i + 1][j])
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
    size_t diagonals_no_zero(int** matrix, size_t rows, size_t cols)
    {
      size_t count = 0;
      if (rows == 0 && cols == 0)
      {
        return 0;
      }
      for (size_t shift = 1; shift < cols; shift++)
      {
        bool has_zero = false;
        for (size_t i = 0; i < rows && i + shift < cols; i++)
        {
          if (matrix[i][i + shift] == 0)
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
          if (matrix[i+shift][i] == 0)
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
    std::cerr << "Need 3 arguments: 1) mode 2) input 3) output" << std::endl;
    return 1;
  }
  std::string mode = argv[1];
  if (mode != "1" && mode != "2")
  {
    std::cerr << "Mode must be 1 or 2" << std::endl;
    return 1;
  }
  int** matrix = nullptr;
  size_t rows, cols;

  if (!zhuravleva::readMatrix(argv[2], matrix, rows, cols, mode == "1"))
  {
    std::cerr << "Invalid matrix" << std::endl;
    return 2;
  }
  size_t result_for_cols_no_dublicats = zhuravleva::cols_no_dublicats(matrix, rows, cols);
  size_t result_for_diagonals_no_zero = zhuravleva::diagonals_no_zero(matrix, rows, cols);
  std::ofstream file_output(argv[3]);
  file_output << result_for_cols_no_dublicats<< " ";

  file_output << result_for_diagonals_no_zero;
  zhuravleva::freeMatrix(matrix, rows);
  return 0;
}