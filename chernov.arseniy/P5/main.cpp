#include <iostream>

namespace chernov {
  struct point_t {
    double x, y;
  };

  struct rectangle_t {
    double width, height, pos;
  };

  struct Shape {
    virtual double getArea() const = 0;
    virtual rectangle_t getFrameRect() const = 0;
    virtual void move(point_t p) = 0;
    virtual void move(double dx, double dy) = 0;
    virtual void scale(double k) = 0;
  };
}

int main()
{}
