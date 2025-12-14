#include <iostream>
#include <algorithm>
#include <cstddef>
#include <stdexcept>

namespace vasyakin
{
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

  struct Concave : Shape
  {
    Concave(point_t a, point_t b, point_t c, point_t d);
    double getArea() const override;
    rectangle_t getFrameRect() const override;
    void move(const point_t& p) override;
    void move(double dx, double dy) override;
    void scale(double k) override;
    point_t getCenter() const;
  private:
    point_t a_, b_, c_, d_;
    double triangleArea(point_t p1, point_t p2, point_t p3) const;
  };
  void ScaleByPnt(Shape ** figures, size_t size, const point_t& k, double a);
  double getSumArea(Shape** arr, size_t k);
  rectangle_t getAllFrame(Shape** arr, size_t k);
  void output(Shape** arr, size_t k);
}
void vasyakin::ScaleByPnt(Shape ** figures, size_t size, const point_t& k, double a)
{
  if (!size || figures == nullptr)
  {
    throw std::invalid_argument("Empty size or array");
  }
  for (size_t i = 0; i < size; ++i)
  {
    point_t g = figures[i]->getFrameRect().pos;
    double dx = (k.x - g.x) * (a - 1);
    double dy = (k.y - g.y) * (a - 1);
    figures[i]->move(dx, dy);
    figures[i]->scale(a);
  }
}

double vasyakin::getSumArea(Shape** arr, size_t k)
{
  if (!k || arr == nullptr)
  {
    throw std::invalid_argument("Invalid size or array");
  }
  double final_area = 0.0;
  for (size_t i = 0; i < k; ++i)
  {
    final_area += arr[i]->getArea();
  }
  return final_area;
}

vasyakin::rectangle_t vasyakin::getAllFrame(Shape** arr, size_t k)
{
  if (!k || arr == nullptr)
  {
    throw std::invalid_argument("Invalid size or array");
  }
  if (arr[0] == nullptr)
  {
    throw std::invalid_argument("First element is nullptr");
  }
  rectangle_t AllFrame{};
  rectangle_t fr = arr[0]->getFrameRect();
  double min_x = fr.pos.x - fr.width / 2;
  double max_x = fr.pos.x + fr.width / 2;
  double min_y = fr.pos.y - fr.height / 2;
  double max_y = fr.pos.y + fr.height / 2;
  for (size_t i = 1; i < k; ++i)
  {
    if (arr[i] == nullptr)
    {
      throw std::invalid_argument("Array contains nullptr");
    }
    fr = arr[i]->getFrameRect();
    min_x = std::min(min_x, fr.pos.x - fr.width / 2);
    max_x = std::max(max_x, fr.pos.x + fr.width / 2);
    min_y = std::min(min_y, fr.pos.y - fr.height / 2);
    max_y = std::max(max_y, fr.pos.y + fr.height / 2);
  }
  AllFrame.width = max_x - min_x;
  AllFrame.height = max_y - min_y;
  AllFrame.pos = {(min_x + max_x) / 2, (min_y + max_y) / 2};
  return AllFrame;
}

void vasyakin::output(Shape** arr, size_t k)
{
  if (arr == nullptr)
  {
    throw std::invalid_argument("Invalid arr");
  }
  for (size_t i = 0; i < k; ++i)
  {
    rectangle_t fr = arr[i]->getFrameRect();
    std::cout << "Figure " << i + 1 << ":\n";
    std::cout << "\tArea " << arr[i]->getArea() << '\n';
    std::cout << "\tFrame Rectangle:\n";
    std::cout << "\t\tWidth: " << fr.width << '\n';
    std::cout << "\t\tHeight: " << fr.height << '\n';
    std::cout << "\t\tCenter: x = " << fr.pos.x << " y = " << fr.pos.y << '\n';
  }
  std::cout << "SumArea: " << getSumArea(arr, k) << '\n';
  rectangle_t fr2 = getAllFrame(arr, k);
  std::cout << "AllFrame:\n";
  std::cout << "\tWidth: " << fr2.width << '\n';
  std::cout << "\tHeight: " << fr2.height << '\n';
  std::cout << "\tCenter: x = " << fr2.pos.x << " y = " << fr2.pos.y << '\n';
}

vasyakin::Rectangle::Rectangle(double width, double height, point_t pos) :
  Shape(),
  width_(width),
  height_(height),
  pos_(pos)
{
  if (width <= 0 || height <= 0)
  {
    throw std::invalid_argument("Invalid size");
  }
}

double vasyakin::Rectangle::getArea() const
{
  return width_ * height_;
}

vasyakin::rectangle_t vasyakin::Rectangle::getFrameRect() const
{
  rectangle_t fr_rect;
  fr_rect.width = width_;
  fr_rect.height = height_;
  fr_rect.pos = pos_;
  return fr_rect;
}

void vasyakin::Rectangle::move(const point_t& p)
{
  pos_ = p;
}

void vasyakin::Rectangle::move(double dx, double dy)
{
  pos_.x += dx;
  pos_.y += dy;
}

void vasyakin::Rectangle::scale(double k)
{
  if (k <= 0)
  {
    throw std::invalid_argument("incorrect coefficient");
  }
  width_ *= k;
  height_ *= k;
}

vasyakin::Triangle::Triangle(point_t a, point_t b, point_t c) :
  Shape(),
  a_(a),
  b_(b),
  c_(c)
{}

vasyakin::point_t vasyakin::Triangle::getCenter() const
{
  point_t center;
  center.x = (a_.x + b_.x + c_.x) / 3.0;
  center.y = (a_.y + b_.y + c_.y) / 3.0;
  return center;
}

double vasyakin::Triangle::getArea() const
{
  return 0.5 * std::abs((b_.x - a_.x) * (c_.y - a_.y) - (c_.x - a_.x) * (b_.y - a_.y));
}

vasyakin::rectangle_t vasyakin::Triangle::getFrameRect() const
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

void vasyakin::Triangle::move(const point_t& p)
{
  point_t center = getCenter();
  double dx = p.x - center.x;
  double dy = p.y - center.y;
  move(dx, dy);
}

void vasyakin::Triangle::move(double dx, double dy)
{
  a_.x += dx;
  a_.y += dy;
  b_.x += dx;
  b_.y += dy;
  c_.x += dx;
  c_.y += dy;
}

void vasyakin::Triangle::scale(double k)
{
  if (k <= 0)
  {
    throw std::invalid_argument("incorrect coefficient");
  }
  point_t center = getCenter();
  a_.x = center.x + (a_.x - center.x) * k;
  a_.y = center.y + (a_.y - center.y) * k;
  b_.x = center.x + (b_.x - center.x) * k;
  b_.y = center.y + (b_.y - center.y) * k;
  c_.x = center.x + (c_.x - center.x) * k;
  c_.y = center.y + (c_.y - center.y) * k;
}

vasyakin::point_t vasyakin::Concave::getCenter() const
{
  return d_;
}

vasyakin::Concave::Concave(point_t a, point_t b, point_t c, point_t d) :
  Shape(),
  a_(a),
  b_(b),
  c_(c),
  d_(d)
{}

double vasyakin::Concave::getArea() const
{
  double area1 = triangleArea(a_, b_, d_);
  double area2 = triangleArea(b_, c_, d_);
  return area1 + area2;
}

vasyakin::rectangle_t vasyakin::Concave::getFrameRect() const
{
  rectangle_t fr_rect;
  double min_x = std::min({a_.x, b_.x, c_.x, d_.x});
  double max_x = std::max({a_.x, b_.x, c_.x, d_.x});
  double min_y = std::min({a_.y, b_.y, c_.y, d_.y});
  double max_y = std::max({a_.y, b_.y, c_.y, d_.y});
  fr_rect.pos.x = (min_x + max_x) / 2.0;
  fr_rect.pos.y = (min_y + max_y) / 2.0;
  fr_rect.width = max_x - min_x;
  fr_rect.height = max_y - min_y;
  return fr_rect;
}

void vasyakin::Concave::move(const point_t& p)
{
  double dx = p.x - d_.x;
  double dy = p.y - d_.y;
  move(dx, dy);
}

void vasyakin::Concave::move(double dx, double dy)
{
  a_.x += dx;
  a_.y += dy;
  b_.x += dx;
  b_.y += dy;
  c_.x += dx;
  c_.y += dy;
  d_.x += dx;
  d_.y += dy;
}

void vasyakin::Concave::scale(double k)
{
  if (k <= 0)
  {
    throw std::invalid_argument("incorrect coefficient");
  }
  point_t center = getCenter();
  a_.x = center.x + (a_.x - center.x) * k;
  a_.y = center.y + (a_.y - center.y) * k;
  b_.x = center.x + (b_.x - center.x) * k;
  b_.y = center.y + (b_.y - center.y) * k;
  c_.x = center.x + (c_.x - center.x) * k;
  c_.y = center.y + (c_.y - center.y) * k;
}

double vasyakin::Concave::triangleArea(point_t p1, point_t p2, point_t p3) const
{
  return 0.5 * std::abs((p2.x - p1.x) * (p3.y - p1.y) - (p3.x - p1.x) * (p2.y - p1.y));
}

int main()
{
  vasyakin::Shape* figures[3] = {nullptr, nullptr, nullptr};
  size_t k = 3;
  double l = 0.0;
  vasyakin::point_t a = {0.0, 0.0};
  std::cout << "x, y, scale: ";
  std::cin >> a.x >> a.y >> l;
  if (!std::cin || l <= 0.0)
  {
    std::cerr << "Bad input\n";
    return 1;
  }
  try
  {
    figures[0] = new vasyakin::Rectangle{3.0, 6.0, {6.0, 4.0}};
    figures[1] = new vasyakin::Triangle{{10, 3}, {12, 4}, {8, 9}};
    figures[2] = new vasyakin::Concave{{11, 9}, {4, 7}, {10, 4}, {6, 5}};
    vasyakin::output(figures, k);
    vasyakin::ScaleByPnt(figures, k, a, l);
    vasyakin::output(figures, k);
    for (size_t i = 0; i < k; ++i)
    {
      delete figures[i];
    }
    return 0;
  }
  catch(const std::bad_alloc& e)
  {
    std::cerr << e.what() << '\n';
  }
  catch(const std::invalid_argument& e)
  {
    std::cerr << e.what() << '\n';
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }
  for (size_t i = 0; i < k; ++i)
  {
    delete figures[i];
  }
  return 1;
}
