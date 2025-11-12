#include <fstream>
#include <iostream>
#include <new>

namespace levkin
{

enum class MemoryType : int
{
  STATIC_MEMORY = 1,
  DYNAMIC_MEMORY
};

struct Memory
{
  int *buffer = nullptr;
  bool is_dynamic = false;
  static const size_t MAX_MATRIX_SIZE = 10000;
  static int static_buffer[MAX_MATRIX_SIZE];

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
      std::fill(static_buffer, static_buffer + size, 0);
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

  int *LFT_BOT_CNT() {}
  int *NUM_COL_LSR() {}
};

int Memory::static_buffer[Memory::MAX_MATRIX_SIZE] = {};

} // namespace levkin

int main(int argc, char **argv)
{
  if (argc != 4 || !(argv[1][0] == '1' || argv[1][0] == '2'))
  {
    std::cout << "wrong arg input\n";
    return 1;
  }

  std::ifstream input(argv[2]);
  std::ofstream ouput(argv[3]);
}
