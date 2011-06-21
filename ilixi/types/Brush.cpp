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

#include "types/Brush.h"

using namespace ilixi;

Brush::Brush() :
  _mode(SolidColorMode), _color(1, 1, 1), _gradient(), _changed(true)
{
}

Brush::Brush(const Brush& brush) :
  _mode(brush.mode()), _color(brush.getColor()),
      _gradient(brush.getGradient()), _changed(true)
{
}

Brush::Brush(const Color& color) :
  _mode(SolidColorMode), _color(color), _gradient(), _changed(true)
{
}

Brush::~Brush()
{
}

Brush::BrushMode
Brush::mode() const
{
  return _mode;
}

Color
Brush::getColor() const
{
  return _color;
}

Gradient
Brush::getGradient() const
{
  return _gradient;
}

void
Brush::setBrushMode(BrushMode mode)
{
  _mode = mode;
  _changed = true;
}

void
Brush::setColor(const Color& color)
{
  _color = color;
  _mode = SolidColorMode;
  _changed = true;
}

void
Brush::setGradient(const Gradient& gradient)
{
  _gradient = gradient;
  _mode = GradientMode;
  _changed = true;
}

Brush&
Brush::operator=(const Brush &brush)
{
  if (this != &brush)
    {
      _mode = brush.mode();
      _color = brush.getColor();
      _gradient = brush.getGradient();
      _changed = true;
    }
  return *this;
}

void
Brush::applyBrush(cairo_t* context)
{
  switch (_mode)
    {
  case GradientMode:
    if (_gradient.getType() != Gradient::None)
      cairo_set_source(context, _gradient.getCairoGradient());
    break;
  case SolidColorMode:
    cairo_set_source_rgba(context, _color.red(), _color.green(), _color.blue(),
        _color.alpha());
    break;
  case TextureMode:
    break;
  default:
    break;
    }
  _changed = false;
}
