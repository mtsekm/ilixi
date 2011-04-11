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

#include "ui/Label.h"
#include "graphics/Painter.h"
#include "types/FontMetrics.h"
#include "core/Logger.h"

using namespace ilixi;

Label::Label(std::string text, Widget* parent) :
  BorderBase(parent), TextLayout(text, parent), _margin(0), _vertical(
      AlignVCenter)
{
  setConstraints(NoConstraint, MinimumConstraint);
}

Label::~Label()
{
}

int
Label::heightForWidth(int width) const
{
  return textHeightForWidth(width - 2 * (borderHorizontalOffset() + _margin))
      + 2 * (borderWidth() + _margin);
}

Size
Label::preferredSize() const
{
  Size s = textExtents();
  return Size(s.width() + 2 * (borderHorizontalOffset() + _margin), s.height()
      + 2 * (borderWidth() + _margin));
}

int
Label::margin() const
{
  return _margin;
}

VerticalAlignment
Label::verticalAlignment() const
{
  return _vertical;
}

void
Label::setMargin(int margin)
{
  _margin = margin;
}

void
Label::setVerticalAlignment(VerticalAlignment vAlign)
{
  _vertical = vAlign;
}

void
Label::compose(const Rectangle& rect)
{
  Painter p(this);
  p.begin(rect);
  designer()->drawFrame(&p, this);
  designer()->drawLabel(&p, this);
  p.end();
}

void
Label::updateTextLayoutGeometry()
{
  if (_vertical == AlignVTop)
    setTextGeometry(_margin + borderHorizontalOffset(),
        _margin + borderWidth(), width() - 2 * borderHorizontalOffset() - 2
            * _margin, height() - 2 * borderWidth() - 2 * _margin);
  else
    {
      int h = textHeightForWidth(width() - 2 * (borderHorizontalOffset()
          + _margin));
      int y = 0;
      if (_vertical == AlignVCenter)
        y = (height() - h) / 2;
      else
        y = height() - h;

      setTextGeometry(_margin + borderHorizontalOffset(), _margin
          + borderWidth() + y, width() - 2 * (borderHorizontalOffset()
          + _margin), height() - 2 * (borderWidth() + _margin));
    }
}
