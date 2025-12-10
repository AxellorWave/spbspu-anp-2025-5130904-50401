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

  class Rectangle: public Shape {
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

  class Triangle: public Shape {
    point_t a_, b_, c_;
    point_t center_;
  public:
    Triangle(point_t a, point_t b, point_t c);
    double getArea() const override;
    rectangle_t getFrameRect() const override;
    void move(point_t p) override;
    void move(double dx, double dy) override;
    void scale(double m) override;
  };

  class Square: public Shape {
    double edge_;
    point_t center_;
  public:
    Square(point_t c, double len);
    double getArea() const override;
    rectangle_t getFrameRect() const override;
    void move(point_t p) override;
    void move(double dx, double dy) override;
    void scale(double m) override;
  };


  void scaleByPnt(Shape** fs, size_t size, point_t p, double m);
  double getSumArea(Shape** array, size_t size);
  rectangle_t getGenericFrame(Shape** array, size_t size);

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

void kuznetsov::scaleByPnt(Shape** fs, size_t size, point_t p, double m)
{
  for (size_t i = 0; i < size; ++i) {
    point_t c = fs[i]->getFrameRect().pos;
    double dx = (c.x - p.x) * (m - 1);
    double dy = (c.y - p.y) * (m - 1);
    fs[i]->move(dx, dy);
    fs[i]->scale(m);
  }
}

double kuznetsov::getSumArea(Shape** array, size_t size)
{
  double finalArea = 0.0;
  for (size_t i = 0; i < size; ++i) {
    finalArea += array[i]->getArea();
  }
  return finalArea;
}

kuznetsov::rectangle_t kuznetsov::getGenericFrame(Shape** array, size_t size)
{
  rectangle_t genericFrame;
  rectangle_t fr = array[0]->getFrameRect();
  double maxY = fr.pos.y + fr.height / 2;
  double minY = fr.pos.y - fr.height / 2;
  double maxX = fr.pos.x + fr.width / 2;
  double minX = fr.pos.x - fr.width / 2;

  for (size_t i = 1; i < size; ++i) {
    Shape* f = array[i];
    fr = f->getFrameRect();
    maxY = std::max(maxY, fr.pos.y + fr.height / 2);
    maxX = std::max(maxX, fr.pos.x + fr.width / 2);
    minY = std::min(minY, fr.pos.y - fr.height / 2);
    minX = std::min(minX, fr.pos.x - fr.width / 2);
  }
  double cx = (maxX + minX) / 2;
  double cy = (maxY + minY) / 2;
  genericFrame.width = maxX - minX;
  genericFrame.height = maxY - minY;
  genericFrame.pos = {cx, cy};
  return genericFrame;
}

kuznetsov::Triangle::Triangle(point_t a, point_t b, point_t c):
  a_(a),
  b_(b),
  c_(c)
{
  double x = (a.x + b.x + c.x) / 2;
  double y = (a.y + b.y + c.y) / 2;
  center_ = {x, y};
}

double kuznetsov::Triangle::getArea() const
{
  double s = a_.x * (b_.y - c_.y);
  s = b_.x * (c_.y - a_.y) + s;
  s = c_.x * (a_.y - b_.y) + s;
  s = 0.5 * std::abs(s);
  return s;
}

kuznetsov::rectangle_t kuznetsov::Triangle::getFrameRect() const
{
  rectangle_t frame;
  double maxX = std::max(a_.x, std::max(b_.x, c_.x));
  double minX = std::min(a_.x, std::min(b_.x, c_.x));
  double maxY = std::max(a_.y, std::max(b_.y, c_.y));
  double minY = std::min(a_.y, std::min(b_.y, c_.y));
  double cx = (maxX + minX) / 2;
  double cy = (maxY + minY) / 2;
  frame.height = maxY - minY;
  frame.width = maxX - minX;
  frame.pos = {cx, cy};
  return frame;
}

void kuznetsov::Triangle::move(point_t p)
{
  center_ = p;
}

void kuznetsov::Triangle::move(double dx, double dy)
{
  center_.x += dx;
  center_.y += dy;
}

void kuznetsov::Triangle::scale(double m)
{
  point_t dpa {center_.x - a_.x, center_.y - a_.y};
  point_t dpb {center_.x - b_.x, center_.y - b_.y};
  point_t dpc {center_.x - c_.x, center_.y - c_.y};

  a_.x = center_.x + dpa.x * m;
  a_.y = center_.y + dpa.y * m;

  b_.x = center_.x + dpb.x * m;
  b_.y = center_.y + dpb.y * m;

  c_.x = center_.x + dpc.x * m;
  c_.y = center_.y + dpc.y * m;
}

kuznetsov::Square::Square(point_t c, double len):
  center_(c),
  edge_(len)
{}

double kuznetsov::Square::getArea() const
{
  return edge_ * edge_;
}

kuznetsov::rectangle_t kuznetsov::Square::getFrameRect() const
{
  rectangle_t frame;
  frame.height = edge_;
  frame.width = edge_;
  frame.pos = center_;
  return frame;
}

void kuznetsov::Square::move(point_t p)
{
  center_ = p;
}

void kuznetsov::Square::move(double dx, double dy)
{
  center_.x += dx;
  center_.y += dy;
}

void kuznetsov::Square::scale(double m)
{
  edge_*= m;
}
