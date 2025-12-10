#include <iostream>

namespace chernov {
  struct point_t {
    double x, y;
  };

  struct rectangle_t {
    double width, height;
    point_t pos;
  };

  struct Shape {
    virtual double getArea() const = 0;
    virtual rectangle_t getFrameRect() const = 0;
    virtual void move(point_t p) = 0;
    virtual void move(double dx, double dy) = 0;
    virtual void scale(double k) = 0;
  };

  struct Rectangle: Shape {
    Rectangle(double a, double b, point_t o);
    double getArea() const override;
    rectangle_t getFrameRect() const override;
    void move(point_t p) override;
    void move(double dx, double dy) override;
    void scale(double k) override;
    double side_a, side_b;
    point_t center;
  };
}

int main()
{}

chernov::Rectangle::Rectangle(double a, double b, point_t o):
  side_a(a),
  side_b(b),
  center(o)
{}

double chernov::Rectangle::getArea() const
{
  return side_a * side_b;
}

chernov::rectangle_t chernov::Rectangle::getFrameRect() const
{
  return {side_a, side_b, center};
}

void chernov::Rectangle::move(point_t p)
{
  center = p;
}

void chernov::Rectangle::move(double dx, double dy)
{
  move({center.x + dx, center.y + dy});
}

void chernov::Rectangle::scale(double k)
{
  side_a *= k;
  side_b *= k;
}