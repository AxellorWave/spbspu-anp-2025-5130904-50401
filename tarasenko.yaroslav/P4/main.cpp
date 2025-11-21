#include <iostream>
#include <iomanip>

char * getline(std::istream & in, size_t & size)
{
  bool is_skipws = in.flags() & std::ios_base::skipws;
  if (is_skipws)
  {
    in >> std::noskipws;
  }
  char * string = nullptr;
  size_t k = 0;
  char last_symbol = '0';
  while (in)
  {
    in >> last_symbol;
    if (last_symbol == '\n')
    {
      break;
    }
    char * temp_mem = new char[k + 1];
    for (size_t i = 0; i < k; ++i)
    {
      temp_mem[i] = string[i];
    }
    temp_mem[k] = last_symbol;
    delete[] string;
    string = temp_mem;
    k++;
  }
  char * temp_mem = new char[k + 1];
  for (size_t i = 0; i < k; ++i)
  {
    temp_mem[i] = string[i];
  }
  temp_mem[k] = '\0';
  delete[] string;
  string = temp_mem;
  size = k;
  if (is_skipws)
  {
    in >> std::skipws;
  }
  return string;
}

int main()
{
  size_t size = 0;
  std::cout << getline(std::cin, size) << ' ' << size << '\n';
}
