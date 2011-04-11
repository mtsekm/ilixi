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

#include "ui/PushButton.h"
#include "graphics/Painter.h"

using namespace ilixi;

PushButton::PushButton(std::string text, Widget* parent) :
  Button(text, parent)
{
  setConstraints(MinimumConstraint, FixedConstraint);
  setSingleParagraph(true);
}

PushButton::~PushButton()
{
}

Size
PushButton::preferredSize() const
{
  if (text().empty() && !hasIcon())
    return designer()->sizeHint(ButtonSizeHint);

  int border = std::max(designer()->hint(BorderWidth), designer()->hint(
      ButtonRadius));

  int w = 2 * border;
  int h = 0;

  if (hasIcon())
    {
      w += icon()->width() + designer()->hint(ButtonOffset);
      h += icon()->height() + 2; // 2px for button pressed state
    }

  if (!text().empty())
    {
      Size s = textExtents();
      w += s.width();
      h = std::max(s.height(), h) + 2 * designer()->hint(BorderWidth);
    }
  return Size(w, h);
}

void
PushButton::compose(const Rectangle& rect)
{
  Painter p(this);
  p.begin(rect);
  designer()->drawPushButton(&p, this);
  p.end();
}

void
PushButton::updateTextLayoutGeometry()
{
  int border = std::max(designer()->hint(BorderWidth), designer()->hint(
      ButtonRadius));
  int x = border;
  int textHeight = textExtents().height();
  int y = (height() - textHeight) / 2;
  int w = 2 * border;

  if (hasIcon())
    {
      _iconPosition.moveTo(x, (height() - icon()->height()) / 2);
      x += icon()->width() + designer()->hint(ButtonOffset);
      w = x + border;
    }

  setTextGeometry(x, y, width() - w, textHeight);
}
