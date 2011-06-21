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

#include "ui/Icon.h"
#include "graphics/Painter.h"
#include "core/Logger.h"

using namespace ilixi;

Icon::Icon(std::string path, Widget* parent) :
  Widget(parent), _border(false), _fillBackground(false), _ownImage(true)
{
  _image = new Image(path, 48, 48);
  setConstraints(FixedConstraint, FixedConstraint);
}

Icon::Icon(Image* image, Widget* parent) :
  Widget(parent), _border(false), _fillBackground(false), _ownImage(false)
{
  _image = image;
  setConstraints(FixedConstraint, FixedConstraint);
}

Icon::~Icon()
{
  if (_ownImage)
    delete _image;
}

Size
Icon::preferredSize() const
{
  if (_border)
    return Size(50, 50);
  return Size(48, 48);
}

bool
Icon::border() const
{
  return _border;
}

bool
Icon::fillBackground() const
{
  return _fillBackground;
}

Image*
Icon::image() const
{
  return _image;
}

void
Icon::setBorder(bool border)
{
  _border = border;
}

void
Icon::setFillBackground(bool fill)
{
  _fillBackground = fill;
}

void
Icon::compose(const Rectangle& rect)
{
  Painter painter(this);
  painter.begin(rect);
  designer()->drawIcon(&painter, this);
  painter.end();
}
