#ifndef POLYGON_HPP
#define POLYGON_HPP
#include <istream>
#include <vector>

namespace zakozhurnikova
{
  struct Point
  {
    int x;
    int y;
    bool operator==(const Point& rhs) const;
    bool operator<(const Point& rhs) const;
    bool operator<=(const Point& rhs) const;
    bool operator>=(const Point& rhs) const;
  };
  std::istream& operator>>(std::istream& in, Point& point);

  struct Polygon
  {
    std::vector< Point > points;
    double getArea() const;
  };
  std::istream& operator>>(std::istream& in, Polygon& polygon);
  bool equalPolygons(const Polygon& lhs, const Polygon& rhs);
}

#endif
