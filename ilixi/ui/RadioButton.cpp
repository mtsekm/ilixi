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

#include "ui/RadioButton.h"
#include "graphics/Painter.h"
#include "core/Logger.h"

using namespace ilixi;

RadioButton::RadioButton(std::string text, Widget* parent) :
  Button(text, parent), _group(0)
{
  setCheckable(true);
  setConstraints(NoConstraint, MinimumConstraint);
  sigCheckChanged.connect(sigc::mem_fun(this, &RadioButton::informGroup));
}

RadioButton::~RadioButton()
{
}

int
RadioButton::heightForWidth(int width) const
{
  int w = designer()->hint(RadioOffset) + designer()->hint(RadioWidth);
  int h = designer()->hint(RadioHeight);

  if (hasIcon())
    {
      w += designer()->hint(RadioOffset) + icon()->width();
      h = std::max(icon()->height(), h);
    }

  return std::max(textHeightForWidth(width - w), h);
}

Size
RadioButton::preferredSize() const
{
  int w = designer()->hint(RadioOffset) + designer()->hint(RadioWidth);
  int h = designer()->hint(RadioHeight);

  if (hasIcon())
    {
      w += designer()->hint(RadioOffset) + icon()->width();
      h = std::max(icon()->height(), h);
    }

  if (!text().empty())
    {
      Size s = textExtents();
      w += designer()->hint(RadioOffset) + s.width();
      h = std::max(s.height(), h);
    }
  return Size(w, h);
}

void
RadioButton::toggleState()
{
  if (!_checked)
    {
      _checked = true;
      sigCheckChanged(_checked);
    }
  update();
}

void
RadioButton::setGroup(RadioGroup* group)
{
  _group = group;
}

void
RadioButton::informGroup(bool checked)
{
  _group->select(this);
}

void
RadioButton::compose(const Rectangle& rect)
{
  Painter p(this);
  p.begin(rect);
  designer()->drawRadioButton(&p, this);
  p.end();
}

void
RadioButton::updateTextLayoutGeometry()
{
  int x = 2 * designer()->hint(RadioOffset) + designer()->hint(RadioWidth);
  int y = (height() - textExtents().height()) / 2;

  if (hasIcon())
    {
      _iconPosition.moveTo(x, (height() - icon()->height()) / 2);
      x += designer()->hint(RadioOffset) + icon()->width();
    }

  setTextGeometry(x, y, width() - x, height());
}
