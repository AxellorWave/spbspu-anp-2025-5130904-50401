#include <cstddef>
#include <fstream>
#include <iostream>
#include <new>
#include <stdexcept>

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

namespace levkin {

enum class MemoryType : int {
    STATIC_MEMORY = 1,
    DYNAMIC_MEMORY
};

struct Memory {
    int* buffer = nullptr;
    bool is_dynamic = false;
    static const size_t MAX_MATRIX_SIZE = 10000;
    static int static_buffer[MAX_MATRIX_SIZE];

    std::ofstream output_stream;  

    size_t rows = 0, cols = 0;  

    int* init(const char* filename, MemoryType type) {
        std::ifstream file_stream(filename);
        if (!file_stream) {
            throw std::runtime_error("сant open input file: " + std::string(filename));
        }

        file_stream >> rows >> cols;
        createBuffer(type, rows * cols);

        for (size_t i = 0; i < rows * cols; ++i) {
            file_stream >> buffer[i];
        }
        return buffer;
    }

    int* createBuffer(MemoryType type, size_t size) {
        clearBuffer();
        if (type == MemoryType::DYNAMIC_MEMORY) {
            buffer = new int[size]();
            is_dynamic = true;
        } else {
            if (size > MAX_MATRIX_SIZE) throw std::bad_alloc();
            buffer = static_buffer;
            is_dynamic = false;
        }
        return buffer;
    }

    void clearBuffer() {
        if (is_dynamic && buffer) delete[] buffer;
        buffer = nullptr;
        is_dynamic = false;
    }

    void openOutput(const char* filename) {
        output_stream.open(filename, std::ios::out | std::ios::trunc);
        if (!output_stream.is_open()) {
            throw std::runtime_error("cant open output file: " + std::string(filename));
        }
    }

    void output() {
        if (!output_stream.is_open()) {
            throw std::runtime_error("call openOutput() first");
        }
        if (buffer == nullptr) {
            throw std::runtime_error("Buffer is empty — call init() first");
        }

        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                output_stream << buffer[i * cols + j];
                if (j + 1 < cols) output_stream << ' ';
            }
            output_stream << '\n';
        }
    }

    void closeOutput() {
        if (output_stream.is_open()) {
            output_stream.close();
        }
    }

    int* LFT_BOT_CNT() { return buffer; }
    int* NUM_COL_LSR() { return buffer; }

    
    void clean() {
        closeOutput();
        clearBuffer();
    }
};

int Memory::static_buffer[Memory::MAX_MATRIX_SIZE] = {};

} // namespace levkin
//   ^ это не нейросеть, это просто у меня форматтер такое ставит

int main(int argc, char **argv)
{
  if (argc != 4)
  {
    std::cerr << "number of arguments is wrong\n";
    return 1;
  }

  char * mode = argv[1];
  if (!(mode[0] == '1' || mode[1] == '0'))
  {
    std::cerr << "1 = dynamic\n2 = static\n";
    return 1;
  }
  levkin::Memory matrix;
  levkin::MemoryType memory_type = (mode[0] =='1')
                                       ? levkin::MemoryType::DYNAMIC_MEMORY
                                       : levkin::MemoryType::STATIC_MEMORY;
  try
  {
    matrix.init(argv[2], memory_type);
    matrix
  } catch (...) {
      matrix.clean();
      return 1;
  }

  std::ifstream input(argv[2]);
  std::ofstream ouput(argv[3]);
}
