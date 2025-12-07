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
  void scaleByPoint(Shape * shapes[], size_t size, point_t p, double k);
  double getAreaAll(Shape * shapes[], size_t size);
  rectangle_t getFrameRectAll(Shape * shapes[], size_t size);
  void printInfo(Shape * shapes[], size_t size);
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

void zharov::scaleByPoint(Shape * shapes[], size_t size, point_t p, double k)
{
  for (size_t i = 0; i < size; ++i) {
    point_t pos = shapes[i]->getFrameRect().pos;
    shapes[i]->move(p);
    shapes[i]->scale(k);
    double dx = k * (pos.x - p.x);
    double dy = k * (pos.y - p.y);
    shapes[i]->move(dx, dy);
  }
}

double zharov::getAreaAll(Shape * shapes[], size_t size)
{
  double sum = 0;
  for (size_t i = 0; i < size; ++i) {
    sum += shapes[i]->getArea();
  }
  return sum;
}

zharov::rectangle_t zharov::getFrameRectAll(Shape * shapes[], size_t size)
{
  rectangle_t frame = shapes[0]->getFrameRect();
  double min_x = frame.pos.x - frame.width / 2;
  double max_x = frame.pos.x + frame.width / 2;
  double min_y = frame.pos.y - frame.height / 2;
  double max_y = frame.pos.y + frame.height / 2;
  for (size_t i = 0; i < size; ++i) {
    frame = shapes[i]->getFrameRect();
    min_x = std::min(min_x, frame.pos.x - frame.width / 2);
    max_x = std::max(max_x, frame.pos.x + frame.width / 2);
    min_y = std::min(min_y, frame.pos.y - frame.height / 2);
    max_y = std::max(max_y, frame.pos.y + frame.height / 2);
  }
  frame.width = max_x - min_x;
  frame.height = max_y - min_y;
  frame.pos = {min_x + frame.width / 2, min_y + frame.height / 2};
  return frame;
}

void zharov::printInfo(Shape * shapes[], size_t size)
{
  for (size_t i = 0; i < size; ++i) {
    rectangle_t frame = shapes[i]->getFrameRect();
    std::cout << i << " area: " << shapes[i]->getArea() << "\n"; 
    std::cout <<"  frame width: " << frame.width << " ";
    std::cout <<"  frame hight: " << frame.height << " ";
    std::cout <<"  frame center: (" << frame.pos.x << ", " << frame.pos.y << ")\n";
  }
  rectangle_t frame_all = getFrameRectAll(shapes, size);
  std::cout << "All area: " << getAreaAll(shapes, size) << "\n";
  std::cout << "All frame width: " << frame_all.width << " ";
  std::cout << "All frame hight: " << frame_all.height << " ";
  std::cout << "All frame center: (" << frame_all.pos.x << ", " << frame_all.pos.y << ")\n";
}

int main()
{
  double k = 0.0;
  zharov::point_t p;
  std::cout << "Enter k, p.x, p.y: ";
  std::cin >> k >> p.x >> p.y;

  zharov::Shape * shapes[3] = {};
  try {
    shapes[0] = new zharov::Rectangle(5, 7, {0,0});
    shapes[1] = new zharov::Rectangle(4, 4, {3,8});
    shapes[2] = new zharov::Rectangle(8, 1, {-5,7});
  } catch (...) {}
  zharov::printInfo(shapes, 3);
  zharov::scaleByPoint(shapes, 3, {p.x, p.y}, k);
  std::cout << "\n";
  zharov::printInfo(shapes, 3);

  delete shapes[0];
  delete shapes[1];
  delete shapes[2];
}
