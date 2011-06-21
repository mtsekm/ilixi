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

#include "Palette.h"

using namespace ilixi;

Palette::Palette()
{
}

Palette::~Palette()
{
}

ColorGroup&
Palette::getGroup(WidgetState state)
{
  if (state & DisabledState)
    return _disabled;
  else if (state & PressedState)
    return _pressed;
  else if (state & ExposedState)
    return _exposed;
  else
    return _default;
}

ColorGroup::ColorGroup() :
  _base(1, 1, 1), _baseAlt(0.9, 0.9, 0.9), _backgroundTop(0.43, 0.43, 0.43),
      _backgroundMid(0, 0, 0), _backgroundBottom(0.28, 0.28, 0.28),
      _borderTop(0.3, 0.3, 0.3), _borderMid(0, 0, 0), _borderBottom(0, 0, 0),
      _text(1, 1, 1)
{
}

ColorGroup::ColorGroup(Color base, Color baseAlt, Color backgroundTop,
    Color backgrounMid, Color backgrounBottom, Color borderTop,
    Color borderMid, Color borderBottom, Color text) :
  _base(base), _baseAlt(baseAlt), _backgroundTop(backgroundTop),
      _backgroundMid(backgrounMid), _backgroundBottom(backgrounBottom),
      _borderTop(borderTop), _borderMid(borderMid),
      _borderBottom(borderBottom), _text(text)
{
}

void
ColorGroup::setColors(Color base, Color baseAlt, Color backgroundTop,
    Color backgrounMid, Color backgrounBottom, Color borderTop,
    Color borderMid, Color borderBottom, Color text)
{
  _base = base;
  _baseAlt = baseAlt;
  _backgroundTop = backgroundTop;
  _backgroundMid = backgrounMid;
  _backgroundBottom = backgrounBottom;
  _borderTop = borderTop;
  _borderMid = borderMid;
  _borderBottom = borderBottom;
  _text = text;
}
