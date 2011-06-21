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

#include "ui/ToolButton.h"
#include "graphics/Painter.h"

using namespace ilixi;

ToolButton::ToolButton(std::string text, Widget* parent) :
  Button(text, parent), _toolButtonStyle(IconBeforeText)
{
  setConstraints(FixedConstraint, FixedConstraint);
  setSingleParagraph(true);
}

ToolButton::~ToolButton()
{
}

Size
ToolButton::preferredSize() const
{
  Size defaultSize = designer()->sizeHint(ButtonSizeHint);

  if (text().empty() && !hasIcon())
    return defaultSize;

  int hBorder = std::max(designer()->hint(BorderWidth),
      designer()->hint(ButtonRadius));
  int vBorder = designer()->hint(BorderWidth);

  int w = 2 * hBorder;
  int h = 2 * vBorder;

  if (_checkable)
    {
      if ((_toolButtonStyle == IconBelowText) || (_toolButtonStyle
          == IconAboveText))
        h += designer()->hint(ButtonCheckedIndicatorWidth) + designer()->hint(
            ButtonOffset);
      else
        w = hBorder + vBorder + designer()->hint(ButtonCheckedIndicatorWidth)
            + designer()->hint(ButtonOffset);
    }

  if (_toolButtonStyle == TextOnly)
    {
      Size s = textExtents();
      if (s.isValid())
        return Size(w + s.width(), h + s.height());
      return defaultSize;
    }

  // calculate image size
  int imgW = 0;
  int imgH = 0;
  if (hasIcon())
    {
      imgW = icon()->width();
      imgH = icon()->height() + 2; // 1px for button down movement
    }

  if (_toolButtonStyle == IconOnly)
    {
      if (imgW)
        return Size(w + imgW, h + imgH);
      return defaultSize;
    }
  else
    {
      Size s = textExtents();
      if (s.isValid() && imgW)
        {
          if (_toolButtonStyle == IconBeforeText)
            {
              w += imgW + designer()->hint(ButtonOffset) + s.width();
              h += std::max(s.height(), imgH);
              return Size(w, h);
            }
          else
            {
              // IconBelowText or IconAboveText
              w += std::max(imgW, s.width());
              h += imgH + s.height() + designer()->hint(ButtonOffset);
              return Size(w, h);
            }
        }
      else if (imgW)
        return Size(w + imgW, h + imgH);
      else
        return Size(w + s.width(), h + s.height());
    }
}

ToolButton::ToolButtonStyle
ToolButton::getToolButtonStyle() const
{
  return _toolButtonStyle;
}

void
ToolButton::setToolButtonStyle(ToolButtonStyle style)
{
  _toolButtonStyle = style;
}

void
ToolButton::compose(const Rectangle& rect)
{
  Painter p(this);
  p.begin(rect);
  designer()->drawToolButton(&p, this);
  p.end();
}

void
ToolButton::updateTextLayoutGeometry()
{
  int textHeight = textExtents().height();
  int iconW = 0;
  int iconH = 0;
  int hBorder = std::max(designer()->hint(BorderWidth),
      designer()->hint(ButtonRadius));
  int vBorder = designer()->hint(BorderWidth);
  int wUsed = 2 * hBorder;
  int x = hBorder;

  if (_checkable)
    {
      if ((_toolButtonStyle == TextOnly) || (_toolButtonStyle == IconOnly)
          || (_toolButtonStyle == IconBeforeText))
        {
          x = vBorder + designer()->hint(ButtonCheckedIndicatorWidth)
              + designer()->hint(ButtonOffset);
          wUsed = x + hBorder;
        }
    }

  if (hasIcon())
    {
      iconW = icon()->width();
      iconH = icon()->height() + 1;
    }

  if (_toolButtonStyle == TextOnly)
    {
      setTextGeometry(x, (height() - textHeight) / 2, width() - wUsed,
          textHeight);
      return;
    }

  else if (_toolButtonStyle == IconOnly)
    {
      _iconPosition.moveTo(x + (width() - (iconW + wUsed)) / 2,
          (height() - iconH) / 2 + 1);
      setTextGeometry(0, 0, 0, 0);
      return;
    }

  else if (_toolButtonStyle == IconBeforeText)
    {
      if (iconW)
        {
          _iconPosition.moveTo(x, vBorder + 1);
          x += iconW + designer()->hint(ButtonOffset);
          wUsed += iconW + designer()->hint(ButtonOffset);
        }
      setTextGeometry(x, (height() - textHeight) / 2, width() - wUsed,
          textHeight);
    }
  else if (_toolButtonStyle == IconBelowText)
    {
      setTextGeometry(x, vBorder, width() - wUsed, textHeight);
      if (iconW)
        {
          _iconPosition.moveTo((width() - iconW) / 2,
              vBorder + textHeight + 1 + designer()->hint(ButtonOffset));
        }
    }
  else //  IconAboveText
    {
      int y = vBorder;
      if (iconW)
        {
          _iconPosition.moveTo((width() - iconW) / 2, y + 1);
          y += iconH + designer()->hint(ButtonOffset) + 1;
        }
      setTextGeometry(hBorder, y, width() - wUsed, textHeight);
    }
}
