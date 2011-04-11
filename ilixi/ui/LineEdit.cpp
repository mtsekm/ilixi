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

#include "ui/LineEdit.h"
#include "graphics/Painter.h"
#include "core/UIManager.h"
#include "core/Logger.h"

using namespace ilixi;

LineEdit::LineEdit(std::string text, Widget* parent) :
  TextLayout(text, parent), _readOnly(false), _maxLength(0)
{
  setInputMethod(OSKInputTracking);
  setConstraints(MinimumConstraint, FixedConstraint);
  setSingleParagraph(true);
}

LineEdit::~LineEdit()
{
}

Size
LineEdit::preferredSize() const
{
  Size s = textExtents();
  return Size(s.width() + 2 * std::max(designer()->hint(BorderWidth),
      designer()->hint(TextInputFrameRadius)), s.height() + 2
      * designer()->hint(BorderWidth));
}

bool
LineEdit::readOnly() const
{
  return _readOnly;
}

int
LineEdit::maxLength() const
{
  return _maxLength;
}

void
LineEdit::clear()
{
  _text = "";
  pango_layout_set_text(_layout, _text.c_str(), _text.length());
  update();
}

void
LineEdit::setReadOnly(bool readOnly)
{
  _readOnly = readOnly;
}

void
LineEdit::setText(const std::string &text)
{
  if (text != _text)
    {
      _text = text;
      pango_layout_set_text(_layout, _text.c_str(), _text.length());
      sigTextChanged(_text);
      update();
    }
}

void
LineEdit::pointerButtonDownEvent(const PointerEvent& mouseEvent)
{
  uiManager()->setOSKWidget(this);
}

void
LineEdit::compose(const Rectangle& rect)
{
  Painter p(this);
  p.begin(rect);
  designer()->drawLineEdit(&p, this, Rectangle());
  p.end();
}

void
LineEdit::updateTextLayoutGeometry()
{
  Size s = textExtents();
  int border = std::max(designer()->hint(TextInputFrameRadius),
      designer()->hint(BorderWidth));
  int yBorder = 2 * designer()->hint(BorderWidth);
  setTextGeometry(border, (height() - s.height()) / 2, width() - 2 * border,
      height() - yBorder);
}

Font*
LineEdit::defaultFont() const
{
  return designer()->defaultFont(InputFont);
}
