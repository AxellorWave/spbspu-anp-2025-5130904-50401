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
int main()
{}
