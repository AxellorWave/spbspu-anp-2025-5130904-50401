#include <iostream>
#include <iomanip>
#include <memory>
#include <cctype>

namespace chernov {
  void resize(char ** str, size_t old_size, size_t new_size);
  char * getline(std::istream & input, size_t & size, size_t step_size);
  int hasSam(const char * str1, const char * str2, size_t size1, size_t size2);
  void latRmv(char * new_str, const char * old_str, size_t old_size);
}

void chernov::resize(char ** str, size_t old_size, size_t new_size)
{
  char * new_str = reinterpret_cast< char * >(malloc(sizeof(char) * new_size));
  if (new_str == nullptr) {
    free(*str);
    throw std::bad_alloc();
  }
  size_t size = std::min(old_size, new_size);
  for (size_t i = 0; i < size; ++i) {
    new_str[i] = (*str)[i];
  }
  free(*str);
  *str = new_str;
}

char * chernov::getline(std::istream & input, size_t & size, const size_t step_size)
{
  bool is_skipws = input.flags() & std::ios_base::skipws;
  if (is_skipws) {
    input >> std::noskipws;
  }
  size_t str_size = step_size, i = 0;
  char * str = nullptr;
  chernov::resize(&str, 0, str_size);
  while (input) {
    if (i >= str_size - 1) {
      chernov::resize(&str, str_size, str_size + step_size);
      str_size += step_size;
    }
    char ch = 0;
    input >> ch;
    if (ch == '\n') {
      break;
    }
    str[i] = ch;
    ++i;
  }
  str[i] = '\0';
  size = i;
  if (is_skipws) {
    input >> std::skipws;
  }
  return str;
}

int chernov::hasSam(const char * str1, const char * str2, size_t size1, size_t size2)
{
  for (size_t i = 0; i < size1; ++i) {
    for (size_t j = 0; j < size2; ++j) {
      if (str1[i] == str2[j]) {
        return 1;
      }
    }
  }
  return 0;
}

void chernov::latRmv(char * new_str, const char * old_str, size_t old_size)
{
  size_t i = 0;
  for (size_t j = 0; j < old_size; ++j) {
    if (!std::isalpha(old_str[j])) {
      new_str[i] = old_str[j];
      ++i;
    }
  }
  new_str[i] = 0;
}

int main()
{
  std::istream & input = std::cin;
  size_t size = 0, step_size = 10;
  char * str = nullptr;
  try {
    str = chernov::getline(input, size, step_size);
  } catch (const std::bad_alloc & e) {
    std::cerr << "badAllocError\n";
    return 1;
  }
  if (!input) {
    free(str);
    std::cerr << "badError\n";
    return 2;
  }

  size_t second_size = 3;
  char second_str[] = "abs";
  int result_has_sam = chernov::hasSam(str, second_str, size, second_size);
  std::cout << "HAS-SAM: " << result_has_sam << "\n";

  char * result_lat_rmv = reinterpret_cast< char * >(malloc(sizeof(char) * size));
  chernov::latRmv(result_lat_rmv, str, size);
  std::cout << "LAT-RMV: " << result_lat_rmv << "\n";
  free(result_lat_rmv);

  free(str);
}
