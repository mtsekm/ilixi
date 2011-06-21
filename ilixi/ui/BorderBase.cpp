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

#include "ui/BorderBase.h"
#include "core/Logger.h"

using namespace ilixi;

BorderBase::BorderBase(Widget* parent) :
  Widget(parent), _borderStyle(NoBorder), _borderWidth(0)
{
}

BorderBase::~BorderBase()
{
}

WidgetBorderStyle
BorderBase::borderStyle() const
{
  return _borderStyle;
}

int
BorderBase::borderWidth() const
{
  if (_borderStyle == NoBorder)
    return 0;
  else if (_borderStyle == StyledBorder)
    return designer()->hint(BorderWidth);
  else
    return _borderWidth;
}

int
BorderBase::borderHorizontalOffset() const
{
  if (_borderStyle == NoBorder)
    return 0;
  else if (_borderStyle == StyledBorder)
    return std::max(designer()->hint(BorderWidth),
        designer()->hint(FrameBorderRadius));
  else
    return _borderWidth;
}

void
BorderBase::setBorderStyle(WidgetBorderStyle style)
{
  _borderStyle = style;
}

void
BorderBase::setBorderWidth(int borderWidth)
{
  _borderWidth = borderWidth;
}
