#include <iostream>

namespace kuznetsov {
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
    virtual void scale(double m) = 0;
    virtual ~Shape() = default;
  };

  class Rectangle: Shape {
    double width_, height_;
    point_t center_;
  public:
    Rectangle(double w, double h, point_t c);
    double getArea() const override;
    rectangle_t getFrameRect() const override;
    void move(point_t p) override;
    void move(double dx, double dy) override;
    void scale(double m) override;
  };


}




int main()
{
}

kuznetsov::Rectangle::Rectangle(double w, double h, point_t c):
  width_(w),
  height_(h),
  center_(c)
{}

double kuznetsov::Rectangle::getArea() const
{
  return width_ * height_;
}

kuznetsov::rectangle_t kuznetsov::Rectangle::getFrameRect() const
{
  return {width_, height_, center_};
}

void kuznetsov::Rectangle::move(point_t p)
{
  center_ = p;
}

void kuznetsov::Rectangle::move(double dx, double dy)
{
  center_.x += dx;
  center_.y += dy;
}

void kuznetsov::Rectangle::scale(double m)
{
  width_ *= m;
  height_ *= m;
}
