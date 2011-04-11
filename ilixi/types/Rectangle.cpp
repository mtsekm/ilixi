/*
 Copyright 2010, 2011 Tarik Sekmen.

 All Rights Reserved.

 Written by Tarik Sekmen <tarik@ilixi.org>.

 ilixi is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 ilixi is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "types/Rectangle.h"
#include "core/Utils.h"

using namespace ilixi;

Rectangle::Rectangle()
{
  _size.setHeight(0);
  _size.setWidth(0);
}

Rectangle::Rectangle(const Rectangle& rectangle) :
  _topLeft(rectangle._topLeft), _size(rectangle._size)
{
}

Rectangle::Rectangle(const Point &topLeft, const Size &size) :
  _topLeft(topLeft), _size(size)
{
}

Rectangle::Rectangle(const Point &topLeft, const Point &bottomRight) :
  _topLeft(topLeft)
{
  _size.setHeight(bottomRight.y() - _topLeft.y());
  _size.setWidth(bottomRight.x() - _topLeft.x());
}

Rectangle::Rectangle(int x, int y, int width, int height)
{
  _topLeft = Point(x, y);
  _size = Size(width, height);
}

Rectangle::~Rectangle()
{
}

int
Rectangle::x() const
{
  return _topLeft.x();
}

int
Rectangle::y() const
{
  return _topLeft.y();
}

int
Rectangle::width() const
{
  return _size.width();
}

int
Rectangle::height() const
{
  return _size.height();
}

Size
Rectangle::size() const
{
  return _size;
}

int
Rectangle::bottom() const
{
  return (y() + height());
}

Point
Rectangle::center() const
{
  if (size().isValid())
    {
      Point p;
      p.setX(x() + width() / 2);
      p.setY(y() + height() / 2);
      return p;
    }
  return _topLeft;
}

Point
Rectangle::bottomLeft() const
{
  Point p;
  p.setX(left());
  p.setY(bottom());
  return p;
}

Point
Rectangle::bottomRight() const
{
  Point p;
  p.setX(right());
  p.setY(bottom());
  return p;
}

Point
Rectangle::topLeft() const
{
  return _topLeft;
}

Point
Rectangle::topRight() const
{
  Point p;
  p.setX(right());
  p.setY(top());
  return p;
}

int
Rectangle::left() const
{
  return x();
}

int
Rectangle::right() const
{
  return (x() + width());
}

int
Rectangle::top() const
{
  return y();
}

bool
Rectangle::isNull() const
{
  if (_size.width() == 0 && _size.height() == 0)
    return true;
  return false;
}

bool
Rectangle::isEmpty() const
{
  if (left() > right() || top() > bottom())
    return true;
  return false;
}

bool
Rectangle::isValid() const
{
  if (left() < right() && top() < bottom())
    return true;
  return false;
}

bool
Rectangle::contains(const Point &point, bool edge) const
{
  return contains(point.x(), point.y(), edge);
}

bool
Rectangle::contains(int px, int py, bool edge) const
{
  if (edge)
    {
      if ((px >= left() && px <= right()) && (py >= top() && py <= bottom()))
        return true;
    }
  else
    {
      if ((px > left() && px < right()) && (py > top() && py < bottom()))
        return true;
    }
  return false;
}

bool
Rectangle::contains(const Rectangle &rect, bool edge) const
{
  if (edge)
    {
      if ((rect.left() >= left() && rect.width() <= width()) && (rect.top()
          >= top() && rect.height() <= height()))
        return true;
    }
  else
    {
      if ((rect.left() > left() && rect.width() < width()) && (rect.top()
          > top() && rect.height() < height()))
        return true;
    }
  return false;
}

bool
Rectangle::intersects(const Rectangle &r) const
{
  return !(left() > r.right() || right() < r.left() || top() > r.bottom()
      || bottom() < r.top());
}

Rectangle
Rectangle::intersected(const Rectangle &r) const
{
  if (intersects(r))
    {
      return Rectangle(Point(max(left(), r.left()), max(top(), r.top())),
          Point(min(right(), r.right()), min(bottom(), r.bottom())));
    }
  return Rectangle(0, 0, 0, 0);
}

Rectangle
Rectangle::united(const Rectangle &r) const
{
  return Rectangle(Point(min(left(), r.left()), min(top(), r.top())), Point(
      max(right(), r.right()), max(bottom(), r.bottom())));
}

void
Rectangle::moveTo(int x, int y)
{
  _topLeft.moveTo(x, y);
}

void
Rectangle::moveTo(const Point &point)
{
  _topLeft = point;
}

void
Rectangle::translate(int x, int y)
{
  _topLeft.translate(x, y);
}

void
Rectangle::setBottom(int y)
{
  _size.setHeight(y - top());
}

void
Rectangle::setLeft(int x)
{
  _topLeft.setX(x);
}

void
Rectangle::setRight(int x)
{
  _size.setWidth(x - left());
}

void
Rectangle::setTop(int y)
{
  _topLeft.setY(y);
}

void
Rectangle::setHeight(int height)
{
  _size.setHeight(height);
}

void
Rectangle::setWidth(int width)
{
  _size.setWidth(width);
}

void
Rectangle::setX(int x)
{
  _topLeft.setX(x);
}

void
Rectangle::setY(int y)
{
  _topLeft.setY(y);
}

void
Rectangle::setTopLeft(const Point& point)
{
  _topLeft = point;
}

void
Rectangle::setSize(int width, int height)
{
  _size.setWidth(width);
  _size.setHeight(height);
}

void
Rectangle::setSize(const Size &size)
{
  _size = size;
}

Rectangle&
Rectangle::operator=(const Rectangle &rectangle)
{
  if (this != &rectangle)
    {
      _topLeft = rectangle._topLeft;
      _size = rectangle._size;
    }
  return *this;
}

bool
Rectangle::operator==(const Rectangle &rectangle) const
{
  return ((_topLeft == rectangle._topLeft) && (_size == rectangle._size));
}

bool
Rectangle::operator!=(const Rectangle &rectangle) const
{
  return !(*this == rectangle);
}
