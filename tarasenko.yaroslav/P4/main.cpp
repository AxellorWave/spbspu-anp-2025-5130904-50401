#include <iostream>
#include <iomanip>
#include <cctype>

namespace tarasenko
{
  bool is_vowel(char c)
  {
    return (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' || c == 'y');
  }

  void remove_vowels(const char * string, char * output, size_t len)
  {
    size_t k = 0;
    for (size_t i = 0; i < len; ++i)
    {
      char current = string[i];
      char lower_c = tolower(static_cast< unsigned char >(current));
      if (is_vowel(lower_c))
      {
        continue;
      }
      output[k] = string[i];
      k++;
    }
  }

  void upp_to_low(const char * string, char * output, size_t len)
  {
    for (size_t i = 0; i < len; ++i)
    {
      char current = string[i];
      if (std::isupper(static_cast< unsigned char >(current)))
      {
        output[i] = std::tolower(static_cast< unsigned char >(current));
      }
      else
      {
        output[i] = string[i];
      }
    }
  }

  char * getline(std::istream & in, size_t & len)
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
    len = k;
    if (is_skipws)
    {
      in >> std::skipws;
    }
    return string;
  }
}

int main()
{
  size_t len = 0;
  char * string = tarasenko::getline(std::cin, len);
  char * lower_string = new char[len + 1]{};
  tarasenko::upp_to_low(string, lower_string, len);
  std::cout << string << '\n';
  std::cout << lower_string << '\n';
  delete[] lower_string;
  char * without_vowels = new char[len + 1]{};
  tarasenko::remove_vowels(string, without_vowels, len);
  std::cout << without_vowels << '\n';
  delete[] without_vowels;
  delete[] string;
}
