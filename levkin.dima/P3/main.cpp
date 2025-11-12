#include <algorithm>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <stdexcept>

namespace levkin
{
enum class MemoryType : int
{
  STATIC_MEMORY = 1,
  DYNAMIC_MEMORY
};

int *copy(int *arr, size_t r, size_t c)
{
  int *copy = new int[r * c];
  for (size_t i = 0; i < r * c; i++)
  {
    copy[i] = arr[i];
  }
  return copy;
}

struct Memory
{
  int *buffer = nullptr;
  bool is_dynamic = false;
  static const size_t MAX_MATRIX_SIZE = 10000;
  static int static_buffer[MAX_MATRIX_SIZE];
  std::ofstream output_stream;
  size_t rows = 0, cols = 0;

  int *init(const char *filename, MemoryType type)
  {
    std::ifstream file_stream(filename);
    if (!file_stream)
    {
      throw std::runtime_error("cant open input file");
    }
    file_stream >> rows >> cols;
    if (file_stream.fail() || rows == 0 || cols == 0)
    {
      throw std::runtime_error("bad dimentions");
    }
    size_t total = rows * cols;
    createBuffer(type, total);
    for (size_t i = 0; i < total; ++i)
    {
      if (!(file_stream >> buffer[i]))
      {
        clearBuffer();
        throw std::runtime_error("failed to read matrix element");
      }
    }
    return buffer;
  }

  int *createBuffer(MemoryType type, size_t size)
  {
    clearBuffer();
    if (type == MemoryType::DYNAMIC_MEMORY)
    {
      buffer = new int[size]();
      is_dynamic = true;
    }
    else
    {
      if (size > MAX_MATRIX_SIZE)
      {
        throw std::bad_alloc();
      }
      buffer = static_buffer;
      is_dynamic = false;
    }
    return buffer;
  }

  void clearBuffer()
  {
    if (is_dynamic && buffer)
    {
      delete[] buffer;
    }
    buffer = nullptr;
    is_dynamic = false;
  }

  void openOutput(const char *filename)
  {
    output_stream.open(filename, std::ios::out | std::ios::trunc);
    if (!output_stream.is_open())
    {
      throw std::runtime_error("cant open output file: ");
    }
  }

  void closeOutput()
  {
    if (output_stream.is_open())
    {
      output_stream.close();
    }
  }

  int *LFT_BOT_CNT()
  {
    if (!buffer || rows == 0 || cols == 0)
      return nullptr;

    int *arr = copy(buffer, rows, cols);
    int top = 0, bottom = static_cast<int>(rows) - 1;
    int left = 0, right = static_cast<int>(cols) - 1;
    int add = 1;
    int visited = 0;
    size_t total = rows * cols;

    while (visited < total)
    {
      for (int j = left; j <= right && visited < total; ++j)
      {
        arr[bottom * cols + j] += add++;
        visited++;
      }
      bottom--;

      for (int j = right; j >= left && visited < total; --j)
      {
        arr[top * cols + j] += add++;
        visited++;
      }
      top++;

      for (int i = bottom; i >= top && visited < total; --i)
      {
        arr[i * cols + left] += add++;
        visited++;
      }
      left++;

      for (int i = top; i <= bottom && visited < total; ++i)
      {
        arr[i * cols + right] += add++;
        visited++;
      }
      right--;
    }

    return arr;
  }

  size_t NUM_COL_LSR() const
  {
    if (!buffer || rows == 0 || cols == 0)
      return 0;

    size_t best_col = 0;
    size_t best_len = 0;

    for (size_t col = 0; col < cols; ++col)
    {
      size_t max_len = 1, cur_len = 1;
      for (size_t row = 1; row < rows; ++row)
      {
        int prev = buffer[(row - 1) * cols + col];
        int curr = buffer[row * cols + col];
        if (curr == prev)
          cur_len++;
        else
          cur_len = 1;
        max_len = std::max(max_len, cur_len);
      }
      if (max_len > best_len)
      {
        best_len = max_len;
        best_col = col;
      }
    }
    return best_col;
  }

  void clean()
  {
    closeOutput();
    clearBuffer();
  }
};

int Memory::static_buffer[Memory::MAX_MATRIX_SIZE] = {};
} // namespace levkin
// ^ это не нейросеть, это просто у меня форматтер такое ставит

int main(int argc, char **argv)
{
  if (argc != 4)
  {
    std::cerr << "arg amount is wrong\n";
    return 1;
  }

  char *mode = argv[1];
  if (mode[0] != '1' && mode[0] != '2')
  {
    std::cerr << "1 = static\n2 = dynamic\n";
    return 1;
  }

  levkin::Memory matrix;
  levkin::MemoryType memory_type = (mode[0] == '1')
                                       ? levkin::MemoryType::STATIC_MEMORY
                                       : levkin::MemoryType::DYNAMIC_MEMORY;

  try
  {
    matrix.init(argv[2], memory_type);
    matrix.openOutput(argv[3]);

    int *result = matrix.LFT_BOT_CNT();
    if (result && matrix.rows > 0 && matrix.cols > 0)
    {
      matrix.output_stream << matrix.rows << ' ' << matrix.cols << '\n';
      for (size_t i = 0; i < matrix.rows; ++i)
      {
        for (size_t j = 0; j < matrix.cols; ++j)
        {
          if (j > 0)
            matrix.output_stream << ' ';
          matrix.output_stream << result[i * matrix.cols + j];
        }
        matrix.output_stream << '\n';
      }
      delete[] result;
    }
    else
    {
      matrix.output_stream << "0 0\n";
      if (result)
        delete[] result;
    }

    size_t best_col = matrix.NUM_COL_LSR();
    matrix.output_stream << best_col << '\n';

    matrix.clean();
    return 0;
  }
  catch (...)
  {
    std::cerr << "unknown error\n";
    matrix.clean();
    return 2;
  }
}
