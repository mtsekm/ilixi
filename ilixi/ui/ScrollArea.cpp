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

#include "ui/ScrollArea.h"
#include "graphics/Painter.h"
//#include "widgets/HBoxLayout.h"

#include "core/Logger.h"

using namespace ilixi;

ScrollArea::ScrollArea(Widget* parent) :
  Frame(parent), _widgetOffsetX(0), _widgetOffsetY(0), _hSliderMode(Auto),
      _vSliderMode(Auto)
{
  setMargin(Margin(0));
  setBorderWidth(0);
  setBorderStyle(NoBorder);
  //  setLayout(new HBoxLayout());
  setBackgroundFilled(false);

  _hSlider = new ScrollBar(this);
  _hSlider->setVisible(false);
  _hSlider->sigValueChanged.connect(sigc::mem_fun(this,
      &ScrollArea::slideHorizontal));
  addChild(_hSlider);

  _vSlider = new ScrollBar(this);
  _vSlider->setVisible(false);
  _vSlider->setOrientation(Vertical);
  _vSlider->setInverted(true);
  _vSlider->sigValueChanged.connect(sigc::mem_fun(this,
      &ScrollArea::slideVertical));
  addChild(_vSlider);
}

ScrollArea::~ScrollArea()
{
}

ScrollArea::SliderMode
ScrollArea::getHSliderMode() const
{
  return _hSliderMode;
}

ScrollArea::SliderMode
ScrollArea::getVSliderMode() const
{
  return _vSliderMode;
}

WidgetResizeConstraint
ScrollArea::hConstraint() const
{
  return NoConstraint;
}

WidgetResizeConstraint
ScrollArea::vConstraint() const
{
  return NoConstraint;
}

void
ScrollArea::setHSliderMode(SliderMode hMode)
{
  _hSliderMode = hMode;
  update();
}

void
ScrollArea::setVSliderMode(SliderMode vMode)
{
  _vSliderMode = vMode;
  update();
}

void
ScrollArea::setLayout(LayoutBase* layout)
{
  if (layout)
    {
      removeChild(_layout);
      _layout = layout;
      _layout->_surfaceDesc = BlitDescription;
      addChild(_layout);
      raiseChildToFront(_layout);
    }
}

void
ScrollArea::paint(const Rectangle& rect, bool forceRepaint)
{
  if (visible())
    {
      updateSurface();
      Rectangle intersect = getIntersectionForPaint(rect, forceRepaint);
      if (intersect.isValid())
        {
          compose(mapToSurface(intersect));
          _hSlider->paint(intersect);
          _vSlider->paint(intersect);

          if (rect.intersects(_layout->_frameGeometry))
            {
              _layout->paint(intersect);
              _layout->blit(this, Rectangle(-_widgetOffsetX, -_widgetOffsetY,
                  canvasWidth(), canvasHeight()), canvasX(), canvasY());
            }
        }
    }
}

void
ScrollArea::slideHorizontal(int value)
{
  _widgetOffsetX = -value;
  blitLayout();
}

void
ScrollArea::slideVertical(int value)
{
  _widgetOffsetY = -value;
  blitLayout();
}

int
ScrollArea::canvasHeight() const
{
  int ch = height() - (_canvasTopLeft.y() + _margin.vSum()) - 2 * borderWidth();
  if (_hSlider->visible())
    ch -= _hSlider->height() + 1;
  return ch;
}

int
ScrollArea::canvasWidth() const
{
  int cw = width() - (_canvasTopLeft.x() + _margin.hSum()) - 2
      * borderHorizontalOffset();
  if (_vSlider->visible())
    cw -= _vSlider->width() + 1;
  return cw;
}

void
ScrollArea::updateLayoutGeometry()
{
  _layoutSize = _layout->preferredSize();
  ILOG_DEBUG("LS: %d, %d", _layoutSize.width(), _layoutSize.height());

  // Set horizontal scroll bar visibility
  if (_hSliderMode == AlwaysVisible)
    _hSlider->setVisible(true);
  else if (_hSliderMode == Auto && _layoutSize.width() > canvasWidth())
    _hSlider->setVisible(true);
  else
    _hSlider->setVisible(false);

  // Set vertical scroll bar visibility
  if (_vSliderMode == AlwaysVisible)
    _vSlider->setVisible(true);
  else if (_vSliderMode == Auto && _layoutSize.height() > canvasHeight())
    _vSlider->setVisible(true);
  else
    _vSlider->setVisible(false);

  // Set Horizontal ScrollBar geometry
  int sbSize = designer()->hint(ScrollBarHeight);
  int sbOffset = sbSize + 2;
  if (_hSlider->visible())
    {
      if (_vSlider->visible())
        _hSlider->setGeometry(0, height() - sbSize, width() - sbOffset, sbSize);
      else
        _hSlider->setGeometry(0, height() - sbSize, width(), sbSize);
      _frameHeight = height() - _hSlider->height() - 1;
    }
  else
    _frameHeight = height();

  // Set Vertical ScrollBar geometry
  if (_vSlider->visible())
    {
      if (_hSlider->visible())
        _vSlider->setGeometry(width() - sbSize, 0, sbSize, height() - sbOffset);
      else
        _vSlider->setGeometry(width() - sbSize, 0, sbSize, height());
      _frameWidth = width() - _vSlider->width() - 1;
    }
  else
    _frameWidth = width();

  if (_hSlider->visible())
    {
      int max = _layoutSize.width() - canvasWidth();
      _hSlider->setMaximum(max);
      _hSlider->setPageStep(max / 10);
      ILOG_DEBUG("HMAX: %d", max / 10);
    }

  if (_vSlider->visible())
    {
      int max = _layoutSize.height() - canvasHeight();
      _vSlider->setMaximum(max);
      _vSlider->setPageStep(max / 10);
      ILOG_DEBUG("VMAX: %d", max / 10);
    }

  // Set Layout's Frame Geometry (FG)
  // FG is used as a clip rectangle and specifies layout's visible region.
  _layout->_frameGeometry.moveTo(absX() + canvasX(), absY() + canvasY());
  _layout->_frameGeometry.setSize(canvasWidth(), canvasHeight());

  // Set layout's Surface Geometry.
  _layout->moveTo(_widgetOffsetX, _widgetOffsetY);
  if (_layoutSize.width() < canvasWidth())
    _layout->setWidth(canvasWidth());
  else
    _layout->setWidth(_layoutSize.width());

  if (_layoutSize.height() < canvasHeight())
    _layout->setHeight(canvasHeight());
  else
    _layout->setHeight(_layoutSize.height());
  //  _layout->updateFrameGeometry();
  //  _layout->paint();
}

void
ScrollArea::compose(const Rectangle& rect)
{
  Painter p(this);
  p.begin(rect);
  designer()->drawFrame(&p, this, 0, 0, _frameWidth, _frameHeight,
      _backgroundFilled);
  p.end();
}

void
ScrollArea::blitLayout()
{
  _layout->moveTo(_widgetOffsetX, _widgetOffsetY);
  _layout->updateFrameGeometry();
  //  LOG_DEBUG("L %d, %d - %d %d", _layout->_surfaceGeometry.x(), _layout->_surfaceGeometry.y(), _widgetOffsetX, _widgetOffsetY);
  surface()->clear(Rectangle(canvasX(), canvasY(), canvasWidth(),
      canvasHeight()));

  if (_backgroundFilled)
    compose(Rectangle(canvasX(), canvasY(), canvasWidth(), canvasHeight()));

  _layout->blit(this, Rectangle(-_widgetOffsetX, -_widgetOffsetY,
      canvasWidth(), canvasHeight()), canvasX(), canvasY());
  flip(Rectangle(canvasX(), canvasY(), canvasWidth(), canvasHeight()));
}
