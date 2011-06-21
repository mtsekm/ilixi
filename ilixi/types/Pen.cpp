/*
 Copyright 2010, 2011 Tarik Sekmen.

 All Rights Reserved.

 Written by Tarik Sekmen <tarik@ilixi.org>.

 This file is part of ilixi.

 ilixi is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 ilixi is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with ilixi.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "types/Pen.h"

using namespace ilixi;

Pen::Pen() :
  _mode(SolidColorMode), _color(0, 0, 0), _gradient(), _lineWidth(2),
      _lineJoinStyle(MitterJoin), _lineCapStyle(ButtCap), _dashCount(0),
      _dashOffset(0), _dashPattern(0), _changed(true)
{
}

Pen::Pen(const Pen& pen) :
  _mode(SolidColorMode), _color(pen._color), _gradient(pen._gradient),
      _lineWidth(pen._lineWidth), _lineJoinStyle(pen._lineJoinStyle),
      _lineCapStyle(pen._lineCapStyle), _dashCount(0), _dashOffset(0),
      _dashPattern(0), _changed(true)
{
  if (pen.getDashPattern())
    setDash(pen.getDashPattern(), pen.getDashCount(), pen.getDashOffset());
}

Pen::Pen(const Color& color) :
  _mode(SolidColorMode), _color(color), _gradient(), _lineWidth(2),
      _lineJoinStyle(MitterJoin), _lineCapStyle(ButtCap), _dashCount(0),
      _dashOffset(0), _dashPattern(0), _changed(true)
{
}

Pen::~Pen()
{
  delete[] _dashPattern;
}

Color
Pen::getColor() const
{
  return _color;
}

Gradient
Pen::getGradient() const
{
  return _gradient;
}

Pen::LineCapStyle
Pen::getLineCapStyle() const
{
  return _lineCapStyle;
}

Pen::LineJoinStyle
Pen::getLineJoinStyle() const
{
  return _lineJoinStyle;
}

double
Pen::getLineWidth() const
{
  return _lineWidth;
}

int
Pen::getDashCount() const
{
  return _dashCount;
}

double
Pen::getDashOffset() const
{
  return _dashOffset;
}

double*
Pen::getDashPattern() const
{
  return _dashPattern;
}

void
Pen::setColor(const Color& color)
{
  _color = color;
  _mode = SolidColorMode;
  _changed = true;
}

void
Pen::clearDashPattern()
{
  _dashCount = 0;
  _changed = true;
}

void
Pen::setDash(const double* dashes, unsigned int dashCount, double offset)
{
  delete[] _dashPattern;
  _dashOffset = offset;
  _dashCount = dashCount;
  _dashPattern = new double[dashCount];
  for (unsigned int i = 0; i < dashCount; ++i)
    _dashPattern[i] = dashes[i];
  _changed = true;
}

void
Pen::setLineCapStyle(LineCapStyle lineCapStyle)
{
  _lineCapStyle = lineCapStyle;
  _changed = true;
}

void
Pen::setLineJoinStyle(LineJoinStyle lineJoinStyle)
{
  _lineJoinStyle = lineJoinStyle;
  _changed = true;
}

void
Pen::setLineWidth(double width)
{
  /// FIXME clipping is problematic if width is 1
  _lineWidth = width;
  _changed = true;
}

void
Pen::setGradient(const Gradient& gradient)
{
  _gradient = gradient;
  _mode = GradientMode;
  _changed = true;
}

void
Pen::setPenMode(PenMode mode)
{
  _mode = mode;
  _changed = true;
}

Pen&
Pen::operator=(const Pen& pen)
{
  if (this != &pen)
    {
      _color = pen._color;
      _lineCapStyle = pen._lineCapStyle;
      _lineJoinStyle = pen._lineJoinStyle;
      _lineWidth = pen._lineWidth;
      if (pen._dashPattern)
        setDash(pen._dashPattern, pen._dashCount, pen._dashOffset);
      else
        clearDashPattern();
    }
  return *this;
}

void
Pen::applyPen(cairo_t* context)
{
  if (_dashPattern)
    cairo_set_dash(context, _dashPattern, _dashCount, _dashOffset);
  else
    cairo_set_dash(context, 0, 0, 0);

  if (_mode == GradientMode && _gradient.getType() != Gradient::None)
    cairo_set_source(context, _gradient.getCairoGradient());
  else
    cairo_set_source_rgba(context, _color.red(), _color.green(), _color.blue(),
        _color.alpha());

  cairo_set_line_width(context, _lineWidth);
  cairo_set_line_join(context, (cairo_line_join_t) _lineJoinStyle);
  cairo_set_line_cap(context, (cairo_line_cap_t) _lineCapStyle);
  _changed = false;
}
