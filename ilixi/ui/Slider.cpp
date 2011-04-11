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

#include "ui/Slider.h"
#include "graphics/Painter.h"
#include "core/Logger.h"

using namespace ilixi;

Slider::Slider(Widget* parent) :
  SliderBase(parent)
{
}

Slider::~Slider()
{
}

Size
Slider::preferredSize() const
{
  Size s = designer()->sizeHint(SliderSizeHint);
  if (orientation() == Horizontal)
    return s;
  s.transpose();
  return s;
}

void
Slider::compose(const Rectangle& rect)
{
  Painter p(this);
  p.begin(rect);
  designer()->drawSlider(&p, this);
  p.end();
}

void
Slider::updateFrameGeometry()
{
  _indicator.setSize(designer()->hint(SliderHeight), designer()->hint(
      SliderHeight));

  if (_orientation == Vertical)
    {
      int x = (width() - _indicator.width()) / 2;
      _frameGeometry.moveTo(absX() + x, absY());
      _frameGeometry.setSize(_indicator.width(), height());

      _indicatorRegion.moveTo(x, 0);
      _indicatorRegion.setSize(_indicator.width(), _frameGeometry.height());

      _sRect.moveTo(x, _indicator.height() / 2);
      _sRect.setSize(_indicatorRegion.width(), _indicatorRegion.height()
          - _indicator.height());
    }
  else
    {
      int y = (height() - _indicator.height()) / 2;
      _frameGeometry.moveTo(absX(), absY() + y);
      _frameGeometry.setSize(width(), _indicator.height());

      _indicatorRegion.moveTo(0, y);
      _indicatorRegion.setSize(_frameGeometry.width(), _indicator.height());

      _sRect.moveTo(_indicator.width() / 2, y);
      _sRect.setSize(_indicatorRegion.width() - _indicator.width(),
          _indicatorRegion.height());
    }
  updateIndicatorPosition();
}
