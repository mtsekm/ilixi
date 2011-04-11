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

#include "types/Color.h"

using namespace ilixi;

Color::Color() :
  _red(0), _green(0), _blue(0), _alpha(1.0)
{
}

Color::Color(const Color& color) :
  _red(color.red()), _green(color.green()), _blue(color.blue()), _alpha(
      color.alpha())
{

}

Color::Color(double r, double g, double b, double a) :
  _red(r), _green(g), _blue(b), _alpha(a)
{
}

Color::~Color()
{
}

double
Color::alpha() const
{
  return _alpha;
}

double
Color::blue() const
{
  return _blue;
}

double
Color::green() const
{
  return _green;
}

double
Color::red() const
{
  return _red;
}

void
Color::setAlpha(double a)
{
  _alpha = a;
  if (_alpha > 1)
    _alpha = 1;
  else if (_alpha < 0)
    _alpha = 0;
}

void
Color::setBlue(double b)
{
  _blue = b;
  if (_blue > 1)
    _blue = 1;
  else if (_blue < 0)
    _blue = 0;
}

void
Color::setGreen(double g)
{
  _green = g;
  if (_green > 1)
    _green = 1;
  else if (_green < 0)
    _green = 0;
}

void
Color::setRed(double r)
{
  _red = r;
  if (_red > 1)
    _red = 1;
  else if (_red < 0)
    _red = 0;
}

Color&
Color::operator=(const Color & color)
{
  if (this != &color)
    {
      _red = color.red();
      _green = color.green();
      _blue = color.blue();
      _alpha = color.alpha();
    }
  return *this;
}

Color&
Color::operator-(const double i)
{
  setRed(_red - i);
  setGreen(_green - i);
  setBlue(_blue - i);
  return *this;
}

Color&
Color::operator+(const double i)
{
  setRed(_red + i);
  setGreen(_green + i);
  setBlue(_blue + i);
  return *this;
}

bool
Color::operator==(const Color & color) const
{
  return ((red() == color.red()) && (green() == color.green()) && (blue()
      == color.blue()) && (alpha() == color.alpha()));
}

bool
Color::operator!=(const Color & color) const
{
  return !(*this == color);
}
