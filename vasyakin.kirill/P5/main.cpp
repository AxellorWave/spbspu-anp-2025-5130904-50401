#include <iostream>

struct point_t
{
  double x, y;
};

struct rectangle_t
{
  double width, height;
  point_t pos;
};

struct Shape
{
  virtual double getArea() const = 0;
  virtual rectangle_t getFrameRect() const = 0;
  virtual void move(point_t p) = 0;
  virtual void move(double dx, double dy) = 0;
  virtual void scale(double k) = 0;
  virtual ~Shape() = default;
};

struct Rectangle : Shape
{
  Rectangle(double width, double height, point_t pos);
  double getArea() const override;
  rectangle_t getFrameRect() const override;
  void move(point_t p) override;
  void move(double dx, double dy) override;
  void scale(double k) override;
private:
  double width_, height_;
  point_t pos_;
};

int main()
{
  return 0;
}
