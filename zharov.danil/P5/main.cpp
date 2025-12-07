#include <iostream>

namespace zharov {
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
    virtual ~Shape() = default;
  };

  struct Rectangle: Shape {
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
}

zharov::Rectangle::Rectangle(double width, double height, point_t pos):
  width_(width),
  height_(height),
  pos_(pos)
{}

double zharov::Rectangle::getArea() const
{
  return width_ * height_;
}

zharov::rectangle_t zharov::Rectangle::getFrameRect() const
{
  rectangle_t frame_rect;
  frame_rect.width = width_;
  frame_rect.height = height_;
  frame_rect.pos = pos_;
  return frame_rect;
}

void zharov::Rectangle::move(point_t p)
{
  pos_ = p;
}

void zharov::Rectangle::move(double dx, double dy)
{
  pos_.x += dx;
  pos_.y += dy;
}

void zharov::Rectangle::scale(double k)
{
  width_ *= k;
  height_ *= k;
}

void scaleByPoint(zharov::Shape * shapes, size_t size, zharov::point_t p, double k)
{
  for (size_t i = 0; i < size; ++i) {
    zharov::point_t pos = shapes[i].getFrameRect().pos;
    shapes[i].move(p);
    shapes[i].scale(k);
    double dx = k * (pos.x - p.x);
    double dy = k * (pos.y - p.y);
    shapes[i].move(dx, dy);
  }
}

zharov::rectangle_t getFrameRectAll(zharov::Shape * shapes, size_t size)
{

}

int main()
{}
