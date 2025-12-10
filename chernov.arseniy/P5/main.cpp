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
    double side_x, side_y;
    point_t center;
  };

  void scaleByPoint(Shape ** shapes, size_t count, double k, point_t p);
  std::ostream & printShapeInfo(std::ostream & out, const Shape * shape, const char * name);
  std::ostream & printShapesInfo(std::ostream & out, const Shape * const * shapes, const char ** names, size_t count);
}

int main()
{
  using namespace chernov;

  std::ostream & output = std::cout;

  const size_t count = 1;
  Shape * shapes[count];
  const char * names[count];

  shapes[0] = new Rectangle(5, 6, {1, 2});
  names[0] = "Rectangle";
  
  printShapesInfo(output, shapes, names, count);
}

void chernov::scaleByPoint(Shape ** shapes, size_t count, double k, point_t p)
{
  for (size_t i = 0; i < count; ++i) {
    Shape * shape = shapes[i];
    point_t pos = shape->getFrameRect().pos;
    shape->move(p);
    double dx = k * (pos.x - p.x);
    double dy = k * (pos.y - p.y);
    shape->move(dx, dy);
    shape->scale(k);
  }
}

std::ostream & chernov::printShapeInfo(std::ostream & out, const Shape * shape, const char * name)
{
  out << name << ":\n";
  out << "  area: " << shape->getArea() << "\n";
  rectangle_t frame_rect = shape->getFrameRect();
  out << "  frame rectangle:\n";
  out << "    width: " << frame_rect.width << "\n";
  out << "    height: " << frame_rect.height << "\n";
  out << "    position: (" << frame_rect.pos.x << "; " << frame_rect.pos.y << ")\n";
  return out;
}

std::ostream & chernov::printShapesInfo(std::ostream & out, const Shape * const * shapes, const char ** names, size_t count)
{
  double total_area = 0;
  for (size_t i = 0; i < count; ++i) {
    printShapeInfo(out, shapes[i], names[i]) << "\n\n";
    total_area += shapes[i]->getArea();
  }
  out << "Total area: " << total_area << "\n\n";
  return out;
}

chernov::Rectangle::Rectangle(double a, double b, point_t o):
  side_x(a),
  side_y(b),
  center(o)
{}

double chernov::Rectangle::getArea() const
{
  return side_x * side_y;
}

chernov::rectangle_t chernov::Rectangle::getFrameRect() const
{
  return {side_x, side_y, center};
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
  side_x *= k;
  side_y *= k;
}
