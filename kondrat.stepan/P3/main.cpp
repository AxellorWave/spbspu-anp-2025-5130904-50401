#include <cstdlib>
#include <cstddef>
#include <iostream>
#include <fstream>

namespace kondrat
{

}



int main(int argc, char ** argv)
{
  if (argc < 4)
  {
    std::cerr << "Not enough arguments\n";
    return 1;
  }
  else if (argc > 4)
  {
    std::cerr << "Too many arguments";
    return 1;
  }

  int num = 0;
  try
  {
    num = std::stoi(argv[1]);
  }
  catch(const std::invalid_argument&)
  {
    std::cerr << "First parameter is not a number\n";
    return 1;
  }
  if (num != 1 && num != 2)
  {
    std::cerr << "First parameter is out of range\n";
    return 1;
  }
}
