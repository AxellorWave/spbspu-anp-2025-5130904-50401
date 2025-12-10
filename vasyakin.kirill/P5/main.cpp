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
  virtual void move(const point_t& p) = 0;
  virtual void move(double dx, double dy) = 0;
  virtual void scale(double k) = 0;
  virtual ~Shape() = default;
};

struct Rectangle : Shape
{
  Rectangle(double width, double height, point_t pos);
  double getArea() const override;
  rectangle_t getFrameRect() const override;
  void move(const point_t& p) override;
  void move(double dx, double dy) override;
  void scale(double k) override;
private:
  double width_, height_;
  point_t pos_;
};

Rectangle::Rectangle(double width, double height, point_t pos) :
  width_(width),
  height_(height),
  pos_(pos)
{}

double Rectangle::getArea() const
{
  return width_ * height_;
}

rectangle_t Rectangle::getFrameRect() const
{
  rectangle_t fr_rect;
  fr_rect.width = width_;
  fr_rect.height = height_;
  fr_rect.pos = pos_;
  return fr_rect;
}

void Rectangle::move(const point_t& p)
{
  pos_ = p;
}

void Rectangle::move(double dx, double dy)
{
  pos_.x += dx;
  pos_.y += dy;
}

void Rectangle::scale(double k)
{
  width_ *= k;
  height_ *= k;
}

int main()
{
  return 0;
}
