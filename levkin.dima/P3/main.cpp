#include <algorithm>
#include <cstddef>
#include <exception>
#include <fstream>
#include <iostream>
#include <stdexcept>

namespace levkin {

enum class MemoryType : int {
  STATIC_MEMORY = 1,
  DYNAMIC_MEMORY
};

int* copy(int* arr, size_t r, size_t c)
{
  int* copy = new int[r * c]();
  for (size_t i = 0; i < r * c; ++i) {
    copy[i] = arr[i];
  }
  return copy;
}

struct Memory {
  static const size_t MAX_MATRIX_SIZE = 10000;
  static int static_buffer[MAX_MATRIX_SIZE];

  int* buffer = nullptr;
  bool is_dynamic = false;
  std::ofstream output_stream;
  size_t rows = 0;
  size_t cols = 0;

  int* init(const char* filename, MemoryType type)
  {
    std::ifstream file_stream(filename);
    if (!file_stream) {
      throw std::runtime_error("cant open input file");
    }

    file_stream >> rows >> cols;
    if (file_stream.fail() || rows == 0 || cols == 0) {
      throw std::length_error("bad dimentions");
    }

    size_t total = rows * cols;
    createBuffer(type, total);

    for (size_t i = 0; i < total; ++i) {
      if (!(file_stream >> buffer[i])) {
        clearBuffer();
        throw std::runtime_error("failed to read matrix element");
      }
    }
    return buffer;
  }

  int* createBuffer(MemoryType type, size_t size)
  {
    clearBuffer();
    if (type == MemoryType::DYNAMIC_MEMORY) {
      buffer = new int[size]();
      is_dynamic = true;
    } else {
      if (size > MAX_MATRIX_SIZE) {
        throw std::bad_alloc();
      }
      buffer = static_buffer;
      is_dynamic = false;
    }
    return buffer;
  }

  void clearBuffer()
  {
    if (is_dynamic && buffer) {
      delete[] buffer;
    }
    buffer = nullptr;
    is_dynamic = false;
  }

  void openOutput(const char* filename)
  {
    output_stream.open(filename);
    if (!output_stream.is_open()) {
      throw std::runtime_error("cant open output file: ");
    }
  }

  void closeOutput()
  {
    if (output_stream.is_open()) {
      output_stream.close();
    }
  }

  int* LFT_BOT_CNT()
  {
    int* arr = copy(buffer, rows, cols);
    int border_padding[4] = {0, 0, 0, 1};

    int x = 0;
    int y = static_cast<int>(rows) - 1;
    int going_mode = 0;  // 0=right, 1=top, 2=left, 3=bottom
    int i = 1;

    while (i <= static_cast<int>(rows * cols)) {
      if (i == static_cast<int>(rows * cols)) {
        arr[cols * y + x] += i;
        break;
      }

      bool can_we_go_this_way = true;
      switch (going_mode) {
        case 0:
          can_we_go_this_way = x + 1 < (static_cast<int>(cols) - border_padding[0]);
          break;
        case 1:
          can_we_go_this_way = y - 1 > border_padding[1] - 1;
          break;
        case 2:
          can_we_go_this_way = x - 1 > border_padding[2] - 1;
          break;
        case 3:
          can_we_go_this_way = y + 1 < (static_cast<int>(rows) - border_padding[3]);
          break;
      }

      if (!can_we_go_this_way) {
        switch (going_mode) {
          case 0:
            going_mode = 1;
            border_padding[0]++;
            break;
          case 1:
            going_mode = 2;
            border_padding[1]++;
            break;
          case 2:
            going_mode = 3;
            border_padding[2]++;
            break;
          case 3:
            going_mode = 0;
            border_padding[3]++;
            break;
        }
        continue;
      }

      arr[cols * y + x] += i;
      i++;

      switch (going_mode) {
        case 0:
          x += 1;
          break;
        case 1:
          y -= 1;
          break;
        case 2:
          x -= 1;
          break;
        case 3:
          y += 1;
          break;
      }
    }
    return arr;
  }

  size_t NUM_COL_LSR() const
  {
    if (!buffer || rows == 0 || cols == 0) {
      return 0;
    }

    size_t best_col = 0;
    size_t best_len = 0;

    for (size_t col = 0; col < cols; ++col) {
      size_t max_len = 1;
      size_t cur_len = 1;

      for (size_t row = 1; row < rows; ++row) {
        int prev = buffer[(row - 1) * cols + col];
        int curr = buffer[row * cols + col];

        if (curr == prev) {
          ++cur_len;
        } else {
          cur_len = 1;
        }
        max_len = std::max(max_len, cur_len);
      }

      if (max_len > best_len) {
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

}  // namespace levkin

int main(int argc, char** argv)
{
  if (argc != 4) {
    std::cerr << "arg amount is wrong\n";
    return 1;
  }

  char* mode = argv[1];
  if (mode[0] != '1' && mode[0] != '2') {
    std::cerr << "1 = static\n2 = dynamic\n";
    return 1;
  }

  levkin::Memory matrix;
  levkin::MemoryType memory_type =
    (mode[0] == '1')
      ? levkin::MemoryType::STATIC_MEMORY
      : levkin::MemoryType::DYNAMIC_MEMORY;

  try {
    matrix.init(argv[2], memory_type);
    matrix.openOutput(argv[3]);

    int* result = matrix.LFT_BOT_CNT();
    matrix.output_stream << matrix.rows << ' ' << matrix.cols << '\n';

    for (size_t i = 0; i < matrix.rows * matrix.cols; ++i) {
      matrix.output_stream << result[i] << " ";
    }

    matrix.output_stream << "\n";
    delete[] result;

    size_t best_col = matrix.NUM_COL_LSR();
    matrix.output_stream << best_col << '\n';

    matrix.clean();
    return 0;

  } catch (std::length_error& e) {
    return 0;

  } catch (std::exception& e) {
    std::cerr << "unknown error: " << e.what() << "\n";
    matrix.clean();
    return 1;
  }
}
