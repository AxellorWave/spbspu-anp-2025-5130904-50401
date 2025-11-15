#include <iostream>
#include <fstream>
#include <string>

namespace zhuravleva {
	const int Max_size = 10000;
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

  int cols_no_dublicats(int ** matrix, size_t rows, size_t cols)
  {
    int count = 0;
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
  int diagonals_no_zero(int** matrix, size_t rows, size_t cols)
  {
    int count = 0;
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