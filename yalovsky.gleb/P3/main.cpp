#include <iostream>
#include <fstream>
#include <cstdlib>

namespace yalovsky{

  bool isNumber(const char * str)
  {
    if (!str || *str == '\0') {
      return false;
    }
    for (size_t i = 0; str[i] != '\0'; i++) {
      if (str[i] < '0' || str[i] > '9') {
        return false;
      }
    }
    return true;
  }
}

int main()
{
  return 0;
}
