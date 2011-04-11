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

#include "ui/ScrollBar.h"
#include "ui/DirectionalButton.h"
#include "graphics/Painter.h"
//#include "core/UIManager.h"
//#include "core/Logger.h"
#include <sigc++/bind.h>

using namespace ilixi;

//*********************************************************************
// ScrollBarThread
//*********************************************************************
ScrollBarThread::ScrollBarThread(ScrollBar* parent) :
  Thread(), _parent(parent), _amount(0)
{
}

ScrollBarThread::~ScrollBarThread()
{
}

int
ScrollBarThread::run()
{
  _parent->setValue(_parent->value() + _amount);
  int value = _parent->value();
  usleep(200000);
  pthread_testcancel();
  while (value > _parent->minimum() && value < _parent->maximum())
    {
      pthread_testcancel();
      _parent->setValue(value + _amount);
      value = _parent->value();
      usleep(200000);
    }
  return 1;
}

void
ScrollBarThread::setSlideAmount(int amount)
{
  _amount = amount;
}

//*********************************************************************
// ScrollBar
//*********************************************************************
ScrollBar::ScrollBar(Widget* parent) :
  SliderBase(parent), _thread(this)
{
  button1 = new DirectionalButton(DirectionalButton::ArrowButton, Left, this);
  button1->sigPressed.connect(sigc::bind<int>(sigc::mem_fun(this,
      &ScrollBar::startSlide), -1));
  button1->sigReleased.connect(sigc::mem_fun(this, &ScrollBar::stopSlide));
  addChild(button1);

  button2 = new DirectionalButton(DirectionalButton::ArrowButton, Right, this);
  button2->sigPressed.connect(sigc::bind<int>(sigc::mem_fun(this,
      &ScrollBar::startSlide), 1));
  button2->sigReleased.connect(sigc::mem_fun(this, &ScrollBar::stopSlide));
  addChild(button2);

  sigValueChanged.connect(sigc::mem_fun(this, &ScrollBar::disableButtons));
}

ScrollBar::~ScrollBar()
{
  _thread.cancel();
}

Size
ScrollBar::preferredSize() const
{
  Size s = designer()->sizeHint(ScrollBarSizeHint);
  if (orientation() == Horizontal)
    return s;
  s.transpose();
  return s;
}

void
ScrollBar::startSlide(int direction)
{
  _thread.setSlideAmount(_pageStep * direction);
  _thread.start();
}

void
ScrollBar::stopSlide()
{
  _thread.cancel();
}

void
ScrollBar::compose(const Rectangle& rect)
{
  Painter p(this);
  p.begin(rect);
  designer()->drawScrollBar(&p, this);
  p.end();
}

void
ScrollBar::updateFrameGeometry()
{
  Size buttonSize = designer()->sizeHint(ScrollBarButtonSizeHint);
  if (_orientation == Vertical)
    {
      buttonSize.transpose();
      int x = (width() - buttonSize.width()) / 2;
      _frameGeometry.moveTo(absX() + x, absY());
      _frameGeometry.setSize(buttonSize.width(), height());

      _indicatorRegion.moveTo(x, buttonSize.height() - 1);
      _indicatorRegion.setSize(buttonSize.width(), height() - 2
          * buttonSize.height() + 2);

      int indicatorHeight = _indicatorRegion.height() * buttonSize.height()
          / 100;
      if (indicatorHeight < buttonSize.height())
        indicatorHeight = buttonSize.height();
      _indicator.setSize(_indicatorRegion.width(), indicatorHeight);

      _sRect.moveTo(_indicatorRegion.x(), _indicatorRegion.y()
          + _indicator.height() / 2);
      _sRect.setSize(_indicatorRegion.width(), _indicatorRegion.height()
          - _indicator.height());
    }
  else
    {
      int y = (height() - buttonSize.height()) / 2;
      _frameGeometry.moveTo(absX(), absY() + y);
      _frameGeometry.setSize(width(), buttonSize.height());

      _indicatorRegion.moveTo(buttonSize.width() - 1, y);
      _indicatorRegion.setSize(width() - 2 * buttonSize.width() + 2,
          buttonSize.height());

      int indicatorWidth = _indicatorRegion.width() * buttonSize.width() / 100;
      if (indicatorWidth < buttonSize.height())
        indicatorWidth = buttonSize.height();
      _indicator.setSize(indicatorWidth, _indicatorRegion.height());

      _sRect.moveTo(_indicatorRegion.x() + _indicator.width() / 2,
          _indicatorRegion.y());
      _sRect.setSize(_indicatorRegion.width() - _indicator.width(),
          _indicatorRegion.height());
    }
  updateIndicatorPosition();
  updateChildrenGeometry();
}

void
ScrollBar::updateChildrenGeometry()
{
  Size buttonSize = designer()->sizeHint(ScrollBarButtonSizeHint);
  if (_orientation == Horizontal)
    {
      int y = (height() - buttonSize.height()) / 2;
      button1->setSize(buttonSize);
      button2->setSize(buttonSize);
      button1->moveTo(0, y);
      button2->moveTo(width() - buttonSize.width(), y);
      button1->setDirection(Left);
      button2->setDirection(Right);
    }
  else
    {
      buttonSize.transpose();
      int x = (width() - buttonSize.width()) / 2;
      button1->setSize(buttonSize);
      button2->setSize(buttonSize);
      if (_inverted)
        {
          button1->moveTo(x, 0);
          button2->moveTo(x, height() - buttonSize.height());
          button1->setDirection(Up);
          button2->setDirection(Down);
        }
      else
        {
          button1->moveTo(x, height() - buttonSize.height());
          button2->moveTo(x, 0);
          button1->setDirection(Down);
          button2->setDirection(Up);
        }
    }
  disableButtons(_value);
}

void
ScrollBar::disableButtons(int value)
{
  if (value > _minimum && value < _maximum)
    {
      button1->setEnabled();
      button2->setEnabled();
    }
  else
    {
      if (_orientation == Horizontal)
        {
          if (value == _minimum)
            button1->setDisabled();
          else if (value == _maximum)
            button2->setDisabled();
        }
      else
        {
          if (value == _minimum)
            button1->setDisabled();
          else if (value == _maximum)
            button2->setDisabled();
        }
    }
}
