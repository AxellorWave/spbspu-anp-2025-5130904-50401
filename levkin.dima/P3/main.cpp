#include <algorithm>
#include <cstddef>
#include <exception>
#include <fstream>
#include <iostream>
#include <stdexcept>

enum MemoryType { STATIC_MEMORY = 1, DYNAMIC_MEMORY };

int *copy_matrix(int const *arr, size_t r, size_t c) {
  int *copy = new int[r * c]();
  for (size_t i = 0; i < r * c; ++i) {
    copy[i] = arr[i];
  }
  return copy;
}

int *create_buffer(MemoryType type, size_t size, bool &is_dynamic) {
  const size_t MAX_MATRIX_SIZE = 10000;

  if (type == DYNAMIC_MEMORY) {
    is_dynamic = true;
    return new int[size]();
  } else {
    if (size > MAX_MATRIX_SIZE) {
      throw std::bad_alloc();
    }
    static int static_buffer[MAX_MATRIX_SIZE];
    is_dynamic = false;
    return static_buffer;
  }
}

void clear_buffer(int *buffer, bool is_dynamic) {
  if (is_dynamic && buffer) {
    delete[] buffer;
  }
  buffer = nullptr;
}

int *init_matrix(const char *filename, MemoryType type, size_t &rows,
                 size_t &cols, int **buffer, bool &is_dynamic) {
  std::ifstream file_stream(filename);
  if (!file_stream) {
    throw std::runtime_error("cant open input file");
  }
  file_stream >> rows >> cols;
  if (file_stream.fail()) {
    throw std::runtime_error("file is wrong");
  }
  if (rows == 0 || cols == 0) {
    throw std::length_error("bad dimensions");
  }
  size_t total = rows * cols;
  *buffer = create_buffer(type, total, is_dynamic);
  for (size_t i = 0; i < total; ++i) {
    if (!(file_stream >> (*buffer)[i])) {
      clear_buffer(*buffer, is_dynamic);
      throw std::runtime_error("failed to read matrix element");
    }
  }
  return *buffer;
}

int *lft_bot_ctn(int const *buffer, size_t rows, size_t cols) {
  int *arr = copy_matrix(buffer, rows, cols);
  size_t border_padding[4] = {0, 0, 0, 1};
  size_t x = 0;
  size_t y = rows - 1;
  size_t going_mode = 0;
  size_t i = 1;
  while (i <= rows * cols) {
    if (i == rows * cols) {
      arr[cols * y + x] += i;
      break;
    }
    bool can_we_go_this_way = true;
    switch (going_mode) {
    case 0:
      can_we_go_this_way = x + 1 < (cols - border_padding[0]);
      break;
    case 1:
      can_we_go_this_way = y > border_padding[1];
      break;
    case 2:
      can_we_go_this_way = x > border_padding[2];
      break;
    case 3:
      can_we_go_this_way = y + 1 < rows - border_padding[3];
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

size_t num_col_lsr(int const *buffer, size_t rows, size_t cols) {
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

void write_result(const char *filename, size_t rows, size_t cols, int *result,
                  size_t best_col) {
  std::ofstream output_stream(filename);
  if (!output_stream.is_open()) {
    throw std::runtime_error("cant open output file");
  }
  output_stream << rows << ' ' << cols << '\n';
  for (size_t i = 0; i < rows * cols; ++i) {
    output_stream << result[i] << " ";
  }
  output_stream << "\n" << best_col << '\n';
  output_stream.close();
}

int main(int argc, char **argv) {
  if (argc != 4) {
    std::cerr << "arg amount is wrong\n";
    return 1;
  }
  char *mode = argv[1];
  if (mode[0] != '1' && mode[0] != '2') {
    std::cerr << "1 = static\n2 = dynamic\n";
    return 1;
  }

  MemoryType memory_type = (mode[0] == '1') ? STATIC_MEMORY : DYNAMIC_MEMORY;
  size_t rows = 0, cols = 0;
  int *buffer = nullptr;
  bool is_dynamic = false;
  try {
    init_matrix(argv[2], memory_type, rows, cols, &buffer, is_dynamic);
    int *result = lft_bot_ctn(buffer, rows, cols);
    size_t best_col = num_col_lsr(buffer, rows, cols);
    write_result(argv[3], rows, cols, result, best_col);
    delete[] result;
    clear_buffer(buffer, is_dynamic);
    return 0;
  } catch (std::length_error &e) {
    return 0;
  } catch (std::exception &e) {
    std::cerr << "unknown error: " << e.what() << "\n";
    clear_buffer(buffer, is_dynamic);
    return 1;
  }
}
