#ifndef SQUARE_HPP
#define SQUARE_HPP
#include "rectangle.hpp"
namespace kuznetsov {

  class Square: public Rectangle {
  public:
    Square(double len, point_t c);
  };

}

#endif

