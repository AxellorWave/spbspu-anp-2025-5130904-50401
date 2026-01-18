#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP
#include "shape.hpp"

namespace kuznetsov {
  class Rectangle: public Shape {
    point_t center_;
    double width_, height_;
  public:
    Rectangle(double w, double h, point_t c);
    double getArea() const override;
    rectangle_t getFrameRect() const override;
    void move(point_t p) override;
    void move(double dx, double dy) override;
    void scale(double m) override;
  };
}

#endif

