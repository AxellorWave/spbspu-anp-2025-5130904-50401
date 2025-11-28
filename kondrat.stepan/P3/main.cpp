#include <cstdlib>
#include <cstddef>
#include <iostream>
#include <fstream>

namespace kondrat
{
  bool check_args(int argc, char ** argv, int & mode)
  {
    if (argc < 4)
    {
      std::cerr << "Not enough arguments\n";
      return false;
    }
    else if (argc > 4)
    {
      std::cerr << "Too many arguments\n";
      return false;
    }

    char * s = argv[1];
    if (!s || s[0] == '\0')
    {
      std::cerr << "First parameter is not a number\n";
      return false;
    }

    if (s[1] != '\0')
    {
      std::cerr << "First parameter is not a number\n";
      return false;
    }

    if (s[0] < '0' || s[0] > '9')
    {
      std::cerr << "First parameter is not a number\n";
      return false;
    }

    int val = s[0] - '0';

    if (val != 1 && val != 2)
    {
      std::cerr << "First parameter is out of range\n";
      return false;
    }

    mode = val;
    return true;
  }

  bool read_matrix_dimensions(std::ifstream & fin, size_t & rows, size_t & cols)
  {
    int tmp_r = 0, tmp_c = 0;

    if (!(fin >> tmp_r >> tmp_c))
    {
      std::cerr << "Invalid matrix\n";
      return false;
    }

    if (tmp_r < 0 || tmp_c < 0)
    {
      std::cerr << "Invalid matrix\n";
      return false;
    }

    rows = static_cast<size_t>(tmp_r);
    cols = static_cast<size_t>(tmp_c);
    return true;
  }

  std::ifstream & fill_matrix(std::ifstream & fin, int * m, size_t rows, size_t cols)
  {
    size_t total = rows * cols;
    for (size_t i = 0; i < total; i++)
    {
      fin >> m[i];
    }
    return fin;
  }

  size_t num_col_lsr(int * m, size_t rows, size_t cols)
  {
    if (rows == 0 || cols == 0)
    {
      return 0;
    }

    size_t best_col = 0;
    size_t max_len = 0;

    for (size_t j = 0; j < cols; j++)
    {
      size_t curr_len = 1;
      size_t col_max = 1;

      for (size_t i = 1; i < rows; i++)
      {
        if (m[i * cols + j] == m[(i - 1) * cols + j])
        {
          curr_len++;
          if (curr_len > col_max)
          {
            col_max = curr_len;
          }
        }
        else
        {
          curr_len = 1;
        }
      }

      if (col_max > max_len)
      {
        max_len = col_max;
        best_col = j;
      }
    }

    return best_col + 1;
  }

  size_t cnt_loc_min(int * m, size_t rows, size_t cols)
  {
    if (rows < 3 || cols < 3)
    {
      return 0;
    }

    size_t count = 0;

    for (size_t i = 0; i < rows; ++i)
    {
      for (size_t j = 0; j < cols; ++j)
      {
        if ((i != 0 && i != (rows - 1)) && (j != 0 && j != (cols - 1)))
        {
          int num = m[i * cols + j];
          bool isLocMin = true;
          for (int i_i = -1; i_i < 2; ++i_i)
          {
            for (int j_j = -1; j_j < 2; ++j_j)
            {
              if (!(i_i == 0 && j_j == 0))
              {
                isLocMin = isLocMin && (num < m[(i + i_i) * cols + (j + j_j)]);
              }
            }
          }
          count += isLocMin;
        }
      }
    }
    return count;
  }

}

int main(int argc, char ** argv)
{
  int mode = 0;

  if (!kondrat::check_args(argc, argv, mode))
  {
    return 1;
  }

  std::ifstream fin(argv[2]);
  if (!fin)
  {
    std::cerr << "Cannot open input file\n";
    return 2;
  }

  size_t rows = 0, cols = 0;
  if (!kondrat::read_matrix_dimensions(fin, rows, cols))
  {
    return 2;
  }

  std::ofstream fout(argv[3]);
  if (!fout)
  {
    std::cerr << "Cannot open output file\n";
    return 2;
  }

  if (mode == 1)
  {
    int m_stack[10000];

    kondrat::fill_matrix(fin, m_stack, rows, cols);
    if (!fin)
    {
      std::cerr << "Invalid matrix\n";
      return 2;
    }

    fin.close();

    size_t local_min_count = kondrat::cnt_loc_min(m_stack, rows, cols);
    size_t best_col = kondrat::num_col_lsr(m_stack, rows, cols);

    fout << local_min_count << ' ' << best_col << "\n";
  }
  else
  {
    int * m = reinterpret_cast< int * >(malloc(rows * cols * sizeof(int)));
    if (!m)
    {
      std::cerr << "Bad allocation memory\n";
      return 3;
    }

    kondrat::fill_matrix(fin, m, rows, cols);
    if (!fin)
    {
      std::cerr << "Invalid matrix\n";
      free(m);
      return 2;
    }

    fin.close();

    size_t local_min_count = kondrat::cnt_loc_min(m, rows, cols);
    size_t best_col = kondrat::num_col_lsr(m, rows, cols);

    fout << local_min_count << ' ' << best_col << "\n";

    free(m);
  }
}
