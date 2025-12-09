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

