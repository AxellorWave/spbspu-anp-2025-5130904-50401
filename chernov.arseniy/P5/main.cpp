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
    virtual ~Shape() = default;
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
  rectangle_t getTotalFrameRect(const Shape * const * shapes, size_t count);
  std::ostream & printShapeInfo(std::ostream & out, const Shape * shape, const char * name);
  std::ostream & printShapesInfo(std::ostream & out, const Shape * const * shapes, const char ** names, size_t count);
}

int main()
{
  using namespace chernov;

  std::ostream & output = std::cout;
  int result = 0;

  const size_t count = 2;
  Shape * shapes[count];
  const char * names[count];

  shapes[0] = new Rectangle(5, 6, {1, 2});
  names[0] = "Rectangle 1";

  shapes[1] = new Rectangle(10, 2, {-10, 3});
  names[1] = "Rectangle 2";

  printShapesInfo(output, shapes, names, count);

  output << "\n\nEnter x, y and k: ";

  double x = 0, y = 0, k = 0;
  while (std::cin >> x >> y >> k) {
    if (k <= 0) {
      std::cerr << "k cannot be less than or equal to zero\n";
      result = 1;
      break;
    }
    scaleByPoint(shapes, count, k, {x, y});
    output << "\n\n";
    printShapesInfo(output, shapes, names, count);
    output << "\n\nEnter x, y and k: ";
  }

  if (std::cin.fail() && (!std::cin.eof() || k == 0)) {
    std::cerr << "bad input\n";
    result = 1;
  }

  for (size_t i = 0; i < count; ++i) {
    delete shapes[i];
  }
  return result;
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

chernov::rectangle_t chernov::getTotalFrameRect(const Shape * const * shapes, size_t count)
{
  rectangle_t frame = shapes[0]->getFrameRect();
  double min_x = frame.pos.x - frame.width / 2;
  double min_y = frame.pos.y - frame.height / 2;
  double max_x = min_x + frame.width;
  double max_y = min_y + frame.height;
  for (size_t i = 1; i < count; ++i) {
    frame = shapes[i]->getFrameRect();
    min_x = std::min(min_x, frame.pos.x - frame.width / 2);
    min_y = std::min(min_y, frame.pos.y - frame.height / 2);
    max_x = std::max(max_x, frame.pos.x + frame.width / 2);
    max_y = std::max(max_y, frame.pos.y + frame.height / 2);
  }
  double width = max_x - min_x;
  double height = max_y - min_y;
  point_t pos = {min_x + width / 2, min_y + height / 2};
  return {width, height, pos};
}

std::ostream & chernov::printShapeInfo(std::ostream & out, const Shape * shape, const char * name)
{
  out << name << ":\n";
  out << "  area: " << shape->getArea() << "\n";
  rectangle_t frame = shape->getFrameRect();
  out << "  frame rectangle:\n";
  out << "    width: " << frame.width << "\n";
  out << "    height: " << frame.height << "\n";
  out << "    position: (" << frame.pos.x << "; " << frame.pos.y << ")\n";
  return out;
}

std::ostream & chernov::printShapesInfo(std::ostream & out, const Shape * const * shapes, const char ** names, size_t count)
{
  double total_area = 0;
  for (size_t i = 0; i < count; ++i) {
    printShapeInfo(out, shapes[i], names[i]) << "\n";
    total_area += shapes[i]->getArea();
  }
  out << "Total area: " << total_area << "\n";
  rectangle_t frame = getTotalFrameRect(shapes, count);
  out << "Total frame rectangle:\n";
  out << "  width: " << frame.width << "\n";
  out << "  height: " << frame.height << "\n";
  out << "  position: (" << frame.pos.x << "; " << frame.pos.y << ")\n";
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
