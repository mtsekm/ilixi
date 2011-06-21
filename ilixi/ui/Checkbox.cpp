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

#include "ui/Checkbox.h"
#include "graphics/Painter.h"
#include "core/Logger.h"

using namespace ilixi;

Checkbox::Checkbox(std::string text, Widget* parent) :
  Button(text, parent), _tristate(false), _checkboxState(Unchecked)
{
  setCheckable(true);
  setConstraints(NoConstraint, MinimumConstraint);
}

Checkbox::~Checkbox()
{
}

int
Checkbox::heightForWidth(int width) const
{
  int w = designer()->hint(CheckBoxOffset) + designer()->hint(CheckBoxWidth);
  int h = designer()->hint(CheckBoxHeight);

  if (hasIcon())
    {
      w += designer()->hint(CheckBoxOffset) + icon()->width();
      h = std::max(icon()->height(), h);
    }

  return std::max(textHeightForWidth(width - w), h);
}

Size
Checkbox::preferredSize() const
{
  int w = designer()->hint(CheckBoxOffset) + designer()->hint(CheckBoxWidth);
  int h = designer()->hint(CheckBoxHeight);

  if (hasIcon())
    {
      w += designer()->hint(CheckBoxOffset) + icon()->width();
      h = std::max(icon()->height(), h);
    }

  if (!text().empty())
    {
      Size s = textExtents();
      w += designer()->hint(CheckBoxOffset) + s.width();
      h = std::max(s.height(), h);
    }
  return Size(w, h);
}

Checkbox::CheckState
Checkbox::checkState() const
{
  return _checkboxState;
}

void
Checkbox::toggleState()
{
  if (_checkable)
    {
      _checked = !_checked;
      if (_tristate)
        {
          if (_checkboxState == Checked)
            _checkboxState = Partial;
          else if (_checkboxState == Partial)
            _checkboxState = Unchecked;
          else
            _checkboxState = Checked;
        }
      else
        {
          if (_checkboxState == Unchecked)
            _checkboxState = Checked;
          else
            _checkboxState = Unchecked;
        }
      sigCheckStateChanged(_checkboxState);
      update();
    }
}

void
Checkbox::setTriState(bool triState)
{
  _tristate = triState;
}

void
Checkbox::compose(const Rectangle& rect)
{
  Painter p(this);
  p.begin(rect);
  designer()->drawCheckBox(&p, this);
  p.end();
}

void
Checkbox::updateTextLayoutGeometry()
{
  int x = 2 * designer()->hint(CheckBoxOffset)
      + designer()->hint(CheckBoxWidth);

  if (hasIcon())
    {
      _iconPosition.moveTo(x, (height() - icon()->height()) / 2);
      x += designer()->hint(CheckBoxOffset) + icon()->width();
    }

  int th = textHeightForWidth(width() - x);
  int y = (height() - th) / 2;

  setTextGeometry(x, y, width() - x, height());
}
