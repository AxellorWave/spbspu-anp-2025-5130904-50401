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

  struct Polygon: Shape {
    Polygon(point_t * points, size_t size);
    ~Polygon();
    Polygon(const Polygon & polygon);
    Polygon & operator=(const Polygon & polygon);
    Polygon(Polygon && polygon);
    Polygon & operator=(Polygon && polygon);
    double getArea() const override;
    rectangle_t getFrameRect() const override;
    void move(point_t p) override;
    void move(double dx, double dy) override;
    void scale(double k) override;
  private:
    point_t pos_;
    point_t * points_;
    size_t size_;
  };

  struct Concave: Shape {
    Concave(point_t * points, point_t pos);
    Concave(point_t p1, point_t p2, point_t p3, point_t pos);
    ~Concave();
    Concave(const Concave & concave);
    Concave & operator=(const Concave & concave);
    Concave(Concave && concave);
    Concave & operator=(Concave && concave);
    double getArea() const override;
    rectangle_t getFrameRect() const override;
    void move(point_t p) override;
    void move(double dx, double dy) override;
    void scale(double k) override;
  private:
    point_t pos_;
    point_t * points_;
  };

  double getAreaUni(point_t * points, size_t size);
  rectangle_t getFrameRectUni(point_t * points, size_t size);
  point_t getCentroid(point_t * points, size_t size);
  void scaleByPoint(Shape * shapes[], size_t size, point_t p, double k);
  double getAreaAll(Shape * shapes[], size_t size);
  rectangle_t getFrameRectAll(Shape * shapes[], size_t size);
  void printInfo(Shape * shapes[], size_t size);
}

zharov::Rectangle::Rectangle(double width, double height, point_t pos):
  Shape(),
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

zharov::Polygon::Polygon(point_t * points, size_t size):
  Shape(),
  size_(size),
  pos_(getCentroid(points, size)),
  points_(new point_t[size])
{
  if (size_ < 3) {
    throw std::invalid_argument("Not enough points");
  }
  points_ = new point_t[size_];
  for (size_t i = 0; i < size; ++i) {
    points_[i] = points[i];
  }
}

zharov::Polygon::~Polygon()
{
  delete[] points_;
}

zharov::Polygon::Polygon(const Polygon & polygon):
  Shape(),
  size_(polygon.size_),
  pos_(polygon.pos_),
  points_(new point_t[polygon.size_])
{
  for (size_t i = 0; i < size_; ++i) {
    points_[i] = polygon.points_[i];
  }
}

zharov::Polygon & zharov::Polygon::operator=(const Polygon & polygon)
{
  if (this != &polygon) {
    delete[] points_;
  }
  pos_ = polygon.pos_;
  size_ = polygon.size_;
  points_ = new point_t[size_];
  for (size_t i = 0; i < size_; ++i) {
    points_[i] = polygon.points_[i];
  }
}

zharov::Polygon::Polygon(Polygon && polygon):
  Shape(),
  size_(polygon.size_),
  pos_(polygon.pos_),
  points_(polygon.points_)
{
  polygon.points_ = nullptr;
}

zharov::Polygon & zharov::Polygon::operator=(Polygon && polygon)
{
  if (this != &polygon) {
    delete[] points_;
  }
  pos_ = polygon.pos_;
  size_ = polygon.size_;
  points_ = polygon.points_;
  polygon.points_ = nullptr;
}

double zharov::Polygon::getArea() const
{
  return getAreaUni(points_, size_);
}

zharov::rectangle_t zharov::Polygon::getFrameRect() const
{
  return getFrameRectUni(points_, size_);
}

void zharov::Polygon::move(double dx, double dy)
{
  for (size_t i = 0; i < size_; ++i) {
    points_[i].x += dx;
    points_[i].y += dy;
  }
  pos_.x += dx;
  pos_.y += dy;
}

void zharov::Polygon::move(point_t p)
{
  move(p.x - pos_.x, p.y - pos_.y);
}

zharov::point_t zharov::getCentroid(point_t * points, size_t size)
{
  double area = 0.0, cx = 0.0, cy = 0.0;
  for (size_t i = 0; i < size; ++i) {
    size_t j = (i + 1) % size;
    double cross = points[i].x * points[j].y - points[j].x * points[i].y;
    area += cross;
    cx += (points[i].x + points[j].x) * cross;
    cy += (points[i].y + points[j].y) * cross;
  }
  area /= 2.0;
  return {cx / (std::abs(area) * 6.0), cy / (std::abs(area) * 6.0)};
}

void zharov::Polygon::scale(double k)
{
  for (size_t i = 0; i < size_; ++i) {
    points_[i].x = pos_.x + (points_[i].x - pos_.x) * k;
    points_[i].y = pos_.y + (points_[i].y - pos_.y) * k;
  }
}

zharov::Concave::Concave(point_t * points, point_t pos):
  Shape(),
  points_(new point_t[4] {points[0], points[1], points[2], points[3]}),
  pos_(pos)
{}

zharov::Concave::Concave(point_t p1, point_t p2, point_t p3, point_t pos):
  Shape(),
  points_(new point_t[4] {p1, p2, p3, pos}),
  pos_(pos)
{}

zharov::Concave::~Concave()
{
  delete[] points_;
}

zharov::Concave::Concave(const Concave & concave):
  Shape(),
  pos_(concave.pos_),
  points_(new point_t[4])
{
  for (size_t i = 0; i < 4; ++i) {
    points_[i] = concave.points_[i];
  }
}

zharov::Concave & zharov::Concave::operator=(const Concave & concave)
{
  if (this != &concave) {
    delete[] points_;
  }
  pos_ = concave.pos_;
  points_ = new point_t[4];
  for (size_t i = 0; i < 4; ++i) {
    points_[i] = concave.points_[i];
  }
}

zharov::Concave::Concave(Concave && concave):
  Shape(),
  pos_(concave.pos_),
  points_(concave.points_)
{
  concave.points_ = nullptr;
}

zharov::Concave & zharov::Concave::operator=(Concave && concave)
{
  if (this != &concave) {
    delete[] points_;
  }
  pos_ = concave.pos_;
  points_ = concave.points_;
  concave.points_ = nullptr;
}

double zharov::Concave::getArea() const
{
  return getAreaUni(points_, 4);
}

double zharov::getAreaUni(point_t * points, size_t size)
{
  double area = 0.0;
  for (size_t i = 0; i < size; ++i) {
    size_t j = (i + 1) % size;
    area += points[i].x * points[j].y - points[j].x * points[i].y;
  }
  return std::abs(area) / 2.0;
}

zharov::rectangle_t zharov::getFrameRectUni(point_t * points, size_t size)
{
  rectangle_t frame;
  double min_x = points[0].x;
  double max_x = points[0].x;
  double min_y = points[0].y;
  double max_y = points[0].y;
  for (size_t i = 1; i < size; ++i) {
    min_x = std::min(min_x, points[i].x);
    max_x = std::max(max_x, points[i].x);
    min_y = std::min(min_y, points[i].y);
    max_y = std::max(max_y, points[i].y);
  }
  frame.width = max_x - min_x;
  frame.height = max_y - min_y;
  frame.pos = {min_x + frame.width / 2, min_y + frame.height / 2};
  return frame;
}

zharov::rectangle_t zharov::Concave::getFrameRect() const
{
  return getFrameRectUni(points_, 4);
}

void zharov::scaleByPoint(Shape * shapes[], size_t size, point_t p, double k)
{
  for (size_t i = 0; i < size; ++i) {
    point_t pos = shapes[i]->getFrameRect().pos;
    shapes[i]->move(p);
    double dx = k * (pos.x - shapes[i]->getFrameRect().pos.x);
    double dy = k * (pos.y - shapes[i]->getFrameRect().pos.y);
    shapes[i]->scale(k);
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
  for (size_t i = 1; i < size; ++i) {
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
    std::cout << "Shape №"<< i + 1 << "\n";
    std::cout << "  Area: " << shapes[i]->getArea() << "\n";
    std::cout << "  Frame:\n";
    std::cout << "    Width: " << frame.width << "\n";
    std::cout << "    Hight: " << frame.height << "\n";
    std::cout << "    Center: (" << frame.pos.x << ", " << frame.pos.y << ")\n";
  }
  rectangle_t frame_all = getFrameRectAll(shapes, size);
  std::cout << "All shapes:\n";
  std::cout << "  Area: " << getAreaAll(shapes, size) << "\n";
  std::cout << "  Frame:\n";
  std::cout << "    Width: " << frame_all.width << "\n";
  std::cout << "    Hight: " << frame_all.height << "\n";
  std::cout << "    Center: (" << frame_all.pos.x << ", " << frame_all.pos.y << ")\n";
}

int main()
{
  double k = 0.0;
  zharov::point_t p;
  std::cout << "Ente p.x, p.y, k: ";
  std::cin >> p.x >> p.y >> k;
  if (!std::cin || k < 0) {
    std::cerr << "Bad enter\n";
    return 1;
  }

  zharov::Shape * shapes[3] = {};
  zharov::point_t * points_polygon = new zharov::point_t[3] {
    {0, 0}, {4, 0}, {0, 3}};
  try {
    shapes[0] = new zharov::Rectangle(5, 7, {0,0});
    shapes[1] = new zharov::Rectangle(4, 4, {3,8});
    shapes[2] = new zharov::Polygon(points_polygon, 3);
  } catch (...) {}
  zharov::printInfo(shapes, 3);
  zharov::scaleByPoint(shapes, 3, {p.x, p.y}, k);
  std::cout << "\n";
  zharov::printInfo(shapes, 3);

  delete shapes[0];
  delete shapes[1];
  delete shapes[2];
  delete[] points_polygon;
}
