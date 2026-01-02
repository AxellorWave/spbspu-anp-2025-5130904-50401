#ifndef SHAPE_HPP
#define SHAPE_HPP
#include "geom.hpp"
namespace kuznetsov {

  struct Shape {
    virtual double getArea() const = 0;
    virtual rectangle_t getFrameRect() const = 0;
    virtual void move(point_t p) = 0;
    virtual void move(double dx, double dy) = 0;
    virtual void scale(double m) = 0;
    virtual ~Shape() = default;
  };

}

#endif

