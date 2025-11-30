#include <algorithm>
#include <exception>
#include <fstream>
#include <iostream>
#include <stdexcept>

enum MemoryType { STATIC_MEMORY = 1, DYNAMIC_MEMORY };

int *copy_matrix(int const *arr, int r, int c) {
  int *copy = new int[r * c]();
  for (int i = 0; i < r * c; ++i) {
    copy[i] = arr[i];
  }
  return copy;
}

int *create_buffer(MemoryType type, int size, bool *is_dynamic) {
  const int MAX_MATRIX_SIZE = 10000;
  static int static_buffer[MAX_MATRIX_SIZE];

  if (type == DYNAMIC_MEMORY) {
    *is_dynamic = true;
    return new int[size]();
  } else {
    if (size > MAX_MATRIX_SIZE) {
      throw std::bad_alloc();
    }
    *is_dynamic = false;
    return static_buffer;
  }
}

void clear_buffer(int *&buffer, bool is_dynamic) {
  if (is_dynamic && buffer) {
    delete[] buffer;
  }
  buffer = nullptr;
}

int *init_matrix(const char *filename, MemoryType type, int *rows, int *cols,
                 int **buffer, bool *is_dynamic) {
  std::ifstream file_stream(filename);
  if (!file_stream) {
    throw std::runtime_error("cant open input file");
  }
  file_stream >> *rows >> *cols;
  if (file_stream.fail()) {
    throw std::runtime_error("file is wrong");
  }
  if (*rows == 0 || *cols == 0) {
    throw std::length_error("bad dimensions");
  }
  int total = *rows * *cols;
  *buffer = create_buffer(type, total, is_dynamic);
  for (int i = 0; i < total; ++i) {
    if (!(file_stream >> (*buffer)[i])) {
      clear_buffer(*buffer, *is_dynamic);
      throw std::runtime_error("failed to read matrix element");
    }
  }
  return *buffer;
}

int *lft_bot_ctn(int *buffer, int rows, int cols) {
  int *arr = copy_matrix(buffer, rows, cols);
  int border_padding[4] = {0, 0, 0, 1};
  int x = 0;
  int y = rows - 1;
  int going_mode = 0;
  int i = 1;
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
      can_we_go_this_way = y - 1 > border_padding[1] - 1;
      break;
    case 2:
      can_we_go_this_way = x - 1 > border_padding[2] - 1;
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

int num_col_lsr(int *buffer, int rows, int cols) {
  if (!buffer || rows == 0 || cols == 0) {
    return 0;
  }
  int best_col = 0;
  int best_len = 0;
  for (int col = 0; col < cols; ++col) {
    int max_len = 1;
    int cur_len = 1;
    for (int row = 1; row < rows; ++row) {
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

void write_result(const char *filename, int rows, int cols, int *result,
                  int best_col) {
  std::ofstream output_stream(filename);
  if (!output_stream.is_open()) {
    throw std::runtime_error("cant open output file");
  }
  output_stream << rows << ' ' << cols << '\n';
  for (int i = 0; i < rows * cols; ++i) {
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
  int rows = 0, cols = 0;
  int *buffer = nullptr;
  bool is_dynamic = false;
  try {
    init_matrix(argv[2], memory_type, &rows, &cols, &buffer, &is_dynamic);
    int *result = lft_bot_ctn(buffer, rows, cols);
    int best_col = num_col_lsr(buffer, rows, cols);
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
