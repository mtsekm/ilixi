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

#include "ui/Line.h"
#include "graphics/Painter.h"

using namespace ilixi;

Line::Line(Orientation orientation, Widget* parent) :
  Widget(parent)
{
  setOrientation(orientation);
}

Line::~Line()
{
}

Size
Line::preferredSize() const
{
  if (_orientation == Horizontal)
    return Size(10, 2);
  return Size(2, 10);
}

Line::Orientation
Line::orientation() const
{
  return _orientation;
}

void
Line::setOrientation(Orientation orientation)
{
  _orientation = orientation;
  if (_orientation == Horizontal)
    setConstraints(MinimumConstraint, FixedConstraint);
  else
    setConstraints(FixedConstraint, MinimumConstraint);
}

void
Line::compose(const Rectangle& rect)
{
  Painter painter(this);
  painter.begin(rect);
  designer()->drawLine(&painter, this);
  painter.end();
}
