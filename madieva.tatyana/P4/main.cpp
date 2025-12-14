#include <iostream>
#include <memory>
#include <cctype>
void uni_two_merge(const char * str1, const char * str2, size_t size, char * res)
{
  size_t i = 0, k = 0, l = 0;
  while (i < size - 1) {
    if (str1[k] != '\0') {
      res[i] = str1[k];
      k++;
      i++;
    }
    if (str2[l] != '\0') {
      res[i] = str2[l];
      i++;
      l++;
    }
  }
  res[i] = '\0';
}
void add_nums(const char * str1, const char * str2, size_t size1, size_t size2, char * res)
{
  size_t i = 0;
  while (i < size1) {
    res[i] = str1[i];
    i++;
  }
  size_t j = 0;
  while (j < size2) {
    if (std::isdigit(str2[j])) {
      res[i] = str2[j];
      i++;
    }
    j++;
  }
  res[i] = '\0';
}
int main() {
  int const max_size = 256;
  char str1[max_size] = {};
  char a = 'a';
  size_t i = 0;
  while (std::cin.get(a) && a != '\n') {
    if (i >= max_size - 1) {
        return 2;
    }
    str1[i] = a;
    i++;
  }
  if (std::cin.fail()) {
    return 1;
  }
  const char * str2 = {"def "};
  size_t j = 0;
  while (str2[j] != '\0') {
    j++;
  }
  size_t size = i + j + 1;
  char * res_two_merge = reinterpret_cast< char* >(malloc(sizeof(char) * size));
  if (res_two_merge == nullptr) {
    return 2;
  }
  uni_two_merge(str1, str2, size, res_two_merge);
  for (size_t p = 0; p < size - 1; p++) {
    std::cout << res_two_merge[p];
  }
  std::cout << "\n";
  free(res_two_merge);
  size_t num_of_digits = 0;
  size_t size2 = 0;
  while (str2[size2] != '\0') {
    if (std::isdigit(str2[size2])) {
      num_of_digits++;
    }
    size2++;
  }
  char * res_add_nums = reinterpret_cast< char* >(malloc(sizeof(char) * i + num_of_digits + 1));
  if (res_add_nums == nullptr) {
    return 2;
  }
  add_nums(str1, str2, i, size2, res_add_nums);
  for (size_t p = 0; p < i + num_of_digits; p++) {
    std::cout << res_add_nums[p];
  }
  std::cout << "\n";
  free(res_add_nums);
  return 0;
}
