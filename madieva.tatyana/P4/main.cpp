#include <iostream>
void uni_two_merge(const char * str1, const char * str2, size_t size, char * res) {
  size_t i = 0, k = 0, l = 0;
  while(i < size - 1) {
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
int main() {
  int const max_size = 256;
  char str1[max_size] = {};
  char a = 'a';
  size_t i = 0;
  while(std::cin.get(a) && a != '\n') {
    if (i = max_size - 1) {
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
  while(str2[j] != '\0') {
    j++;
  }
  size_t size = i + j;
  char * res = reinterpret_cast<char*>(malloc(sizeof(char) * size + 1));
  if (res == nullptr) {
    return 2;
  }
  uni_two_merge(str1, str2, size, res);
  free(res);
  return 0;
}
