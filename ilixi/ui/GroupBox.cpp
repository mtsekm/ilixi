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

#include "ui/GroupBox.h"
#include "graphics/Painter.h"
#include "types/FontMetrics.h"
#include "core/Logger.h"

using namespace ilixi;

GroupBox::GroupBox(std::string title, Widget* parent) :
  Frame(parent), _title(title)
{
  setMargin(5);
}

GroupBox::~GroupBox()
{
}

int
GroupBox::heightForWidth(int width) const
{
  int usedHorizontalSpace = (_canvasTopLeft.x() + _margin.hSum()) - 2
      * borderHorizontalOffset();
  return _layout->heightForWidth(width - usedHorizontalSpace)
      + _canvasTopLeft.y() + _margin.vSum() + 2 * borderWidth()
      + getTitleTextSize().height();
}

Size
GroupBox::preferredSize() const
{
  ILOG_DEBUG("GROUP PREF");
  Size s = _layout->preferredSize();
  return Size(s.width() + _canvasTopLeft.x() + _margin.hSum() + 2
      * borderHorizontalOffset(), s.height() + _canvasTopLeft.y()
      + _margin.vSum() + 2 * borderWidth() + getTitleTextSize().height());
}

std::string
GroupBox::title() const
{
  return _title;
}

void
GroupBox::setTitle(std::string title)
{
  _title = title;
}

void
GroupBox::compose(const Rectangle& rect)
{
  Painter painter(this);
  painter.begin(rect);
  designer()->drawGroupBox(&painter, this);
  painter.end();
}

int
GroupBox::canvasY() const
{
  return _canvasTopLeft.y() + _margin.top() + borderWidth()
      + getTitleTextSize().height();
}

int
GroupBox::canvasHeight() const
{
  return height() - (_canvasTopLeft.y() + _margin.vSum()
      + getTitleTextSize().height()) - 2 * borderWidth();
}

Size
GroupBox::getTitleTextSize() const
{
  if (!_title.empty())
    {
      return FontMetrics::getSize(designer()->defaultFont(TitleFont), _title);
    }
  return Size(0, 0);
}
