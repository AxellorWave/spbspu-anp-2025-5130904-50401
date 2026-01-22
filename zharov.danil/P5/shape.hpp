#ifndef SHAPE_HPP
#define SHAPE_HPP
#include "geom.hpp"

namespace zharov
{
  struct Shape {
  virtual double getArea() const = 0;
  virtual rectangle_t getFrameRect() const = 0;
  virtual void move(point_t p) = 0;
  virtual void move(double dx, double dy) = 0;
  virtual void scale(double positiv_k) = 0;
  virtual ~Shape() = default;
  };

  void scaleByPoint(Shape * shapes[], size_t size, point_t p, double positive_k);
  double getAreaAll(Shape * shapes[], size_t size);
  rectangle_t getFrameRectAll(Shape * shapes[], size_t size);
}
#endif
