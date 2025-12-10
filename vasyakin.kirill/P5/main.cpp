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

struct Triangle : Shape
{
  Triangle(point_t a, point_t b, point_t c);
  double getArea() const override;
  rectangle_t getFrameRect() const override;
  void move(const point_t& p) override;
  void move(double dx, double dy) override;
  void scale(double k) override;
  point_t getCenter() const;
private:
  point_t a_, b_, c_;
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

Triangle::Triangle(point_t a, point_t b, point_t c) :
  a_(a),
  b_(b),
  c_(c)
{}

point_t Triangle::getCenter() const
{
  point_t center;
  center.x = (a_.x + b_.x + c_.x) / 3.0;
  center.y = (a_.y + b_.y + c_.y) / 3.0;
  return center;
}

double Triangle::getArea() const
{
  return 0.5 * std::abs((b_.x - a_.x) * (c_.y - a_.y) - (c_.x - a_.x) * (b_.y - a_.y));
}

rectangle_t Triangle::getFrameRect() const
{
  rectangle_t fr_rect;
  double min_x = std::min({a_.x, b_.x, c_.x});
  double max_x = std::max({a_.x, b_.x, c_.x});
  double min_y = std::min({a_.y, b_.y, c_.y});
  double max_y = std::max({a_.y, b_.y, c_.y});
  fr_rect.pos.x = (min_x + max_x) / 2.0;
  fr_rect.pos.y = (min_y + max_y) / 2.0;
  fr_rect.width = max_x - min_x;
  fr_rect.height = max_y - min_y;
  return fr_rect;
}

void Triangle::move(const point_t& p)
{
  point_t center = getCenter();
  double dx = p.x - center.x;
  double dy = p.y - center.y;
  move(dx, dy);
}

void Triangle::move(double dx, double dy)
{
  a_.x += dx;
  a_.y += dy;
  b_.x += dx;
  b_.y += dy;
  c_.x += dx;
  c_.y += dy;
}

void Triangle::scale(double k)
{
  point_t center = getCenter();
  a_.x = center.x + (a_.x - center.x) * k;
  a_.y = center.y + (a_.y - center.y) * k;
  b_.x = center.x + (b_.x - center.x) * k;
  b_.y = center.y + (b_.y - center.y) * k;
  c_.x = center.x + (c_.x - center.x) * k;
  c_.y = center.y + (c_.y - center.y) * k;
}

int main()
{
  return 0;
}
