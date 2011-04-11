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

#include "ui/SliderBase.h"

#include "core/Logger.h"

using namespace ilixi;

SliderBase::SliderBase(Widget* parent) :
  Widget(parent), _maximum(100), _minimum(0), _value(0), _step(1),
      _pageStep(10), _orientation(Horizontal), _inverted(false)
{
  _range = _maximum - _minimum;
  setInputMethod(KeyAndPointerInputTracking);
  setConstraints(ExpandingConstraint, FixedConstraint);
}

SliderBase::~SliderBase()
{
}

Rectangle
SliderBase::indicatorGeometry() const
{
  return _indicator;
}

Rectangle
SliderBase::indicatorRegion() const
{
  return _indicatorRegion;
}

bool
SliderBase::inverted() const
{
  return _inverted;
}

int
SliderBase::maximum() const
{
  return _maximum;
}

int
SliderBase::minimum() const
{
  return _minimum;
}

Orientation
SliderBase::orientation() const
{
  return _orientation;
}

int
SliderBase::step() const
{
  return _step;
}

int
SliderBase::pageStep() const
{
  return _pageStep;
}

int
SliderBase::range() const
{
  return _range;
}

int
SliderBase::value() const
{
  return _value;
}

void
SliderBase::setInverted(bool inverted)
{
  if (_inverted != inverted)
    {
      _inverted = inverted;
      setSurfaceGeometryModified();
    }
}

void
SliderBase::setMaximum(int maximum)
{
  if (maximum != _maximum)
    {
      if (maximum <= _minimum)
        _maximum = _minimum;
      else
        _maximum = maximum;

      if (_value > _maximum)
        _value = _maximum;

      _range = _maximum - _minimum;
    }
}

void
SliderBase::setMinimum(int minimum)
{
  if (minimum != _maximum)
    {
      if (minimum >= _maximum)
        _minimum = _maximum;
      else
        _minimum = minimum;

      if (_value < _minimum)
        _value = _minimum;

      _range = _maximum - _minimum;
    }
}

void
SliderBase::setOrientation(Orientation orientation)
{
  if (_orientation != orientation)
    {
      _orientation = orientation;
      if (_orientation == Horizontal)
        setConstraints(ExpandingConstraint, FixedConstraint);
      else
        setConstraints(FixedConstraint, ExpandingConstraint);
      setSurfaceGeometryModified();
    }
}

void
SliderBase::setRange(int minimum, int maximum)
{
  if (minimum >= maximum)
    _minimum = maximum;

  if (maximum <= minimum)
    _maximum = minimum;

  if (_value < _minimum)
    _value = _minimum;
  else if (_value > _maximum)
    _value = _maximum;

  _range = _maximum - _minimum;
}

void
SliderBase::setStep(int step)
{
  _step = step;
}

void
SliderBase::setPageStep(int pageStep)
{
  _pageStep = pageStep;
}

void
SliderBase::setValue(int value)
{
  if (value != _value)
    {
      if (value < _minimum)
        _value = _minimum;
      else if (value > _maximum)
        _value = _maximum;
      else
        _value = value;

      updateIndicatorPosition();
      //      LOG_DEBUG("Value: %d", _value);
      sigValueChanged(_value);
      update();
    }
}

void
SliderBase::addAmount(int amount)
{
  setValue(_value + amount);
}

void
SliderBase::subAmount(int amount)
{
  setValue(_value - amount);
}

void
SliderBase::updateIndicatorPosition()
{
  if (_orientation == Horizontal)
    _indicator.moveTo(_indicatorRegion.x() + _sRect.width() * _value / _range,
        _indicatorRegion.y());
  else if (_inverted)
    _indicator.moveTo(_indicatorRegion.x(), _indicatorRegion.y()
        + _sRect.height() * _value / _range);
  else
    _indicator.moveTo(_indicatorRegion.x(), _indicatorRegion.y()
        + _sRect.height() * (_range - _value) / _range);
}

void
SliderBase::keyUpEvent(const KeyEvent& keyEvent)
{
  switch (keyEvent.keySymbol)
    {
  case DIKS_CURSOR_LEFT:
  case DIKS_CURSOR_UP:
    if (_orientation == Vertical && !_inverted)
      addAmount(_step);
    else
      subAmount(_step);
    break;
  case DIKS_CURSOR_RIGHT:
  case DIKS_CURSOR_DOWN:
    if (_orientation == Vertical && !_inverted)
      subAmount(_step);
    else
      addAmount(_step);
    break;
  case DIKS_PAGE_UP:
    if (_orientation == Vertical && !_inverted)
      addAmount(_pageStep);
    else
      subAmount(_pageStep);
    break;
  case DIKS_PAGE_DOWN:
    if (_orientation == Vertical && !_inverted)
      subAmount(_pageStep);
    else
      addAmount(_pageStep);
    break;
  case DIKS_HOME:
    setValue(_minimum);
    break;
  case DIKS_END:
    setValue(_maximum);
    break;
  default:
    break;
    }
}

void
SliderBase::pointerButtonDownEvent(const PointerEvent& event)
{
  Point mouseLocal = mapToSurface(Point(event.x, event.y));
  if (_indicator.contains(mouseLocal.x(), mouseLocal.y(), true))
    {
      sigPressed();
      int cursor;
      if (_orientation == Horizontal)
        {
          if (mouseLocal.x() < _sRect.x())
            cursor = 0;
          else if (mouseLocal.x() > _sRect.right())
            cursor = _sRect.width();
          else
            cursor = mouseLocal.x() - _sRect.x();
          int value = _range * cursor / (_sRect.width() + .0);
          if (value != _value)
            setValue(value);
          else
            update();
        }
      else if (_inverted) // Vertical orientation & inverted
        {
          if (mouseLocal.y() < _sRect.y())
            cursor = 0;
          else if (mouseLocal.y() > _sRect.bottom())
            cursor = _sRect.height();
          else
            cursor = mouseLocal.y() - _sRect.y();
          int value = _range * cursor / (_sRect.height() + .0);
          if (value != _value)
            setValue(value);
          else
            update();
        }
      else // Vertical orientation
        {
          if (mouseLocal.y() < _sRect.y())
            cursor = _sRect.height();
          else if (mouseLocal.y() > _sRect.bottom())
            cursor = 0;
          else
            cursor = _sRect.bottom() - mouseLocal.y();
          int value = _range * cursor / (_sRect.height() + .0);
          if (value != _value)
            setValue(value);
          else
            update();
        }
    }
  else if (_orientation == Horizontal)
    {
      if (mouseLocal.x() > _indicator.x())
        addAmount(_pageStep);
      else
        subAmount(_pageStep);
    }
  else if (_inverted)
    {
      if (mouseLocal.y() > _indicator.y())
        addAmount(_pageStep);
      else
        subAmount(_pageStep);
    }
  else
    {
      if (mouseLocal.y() > _indicator.y())
        subAmount(_pageStep);
      else
        addAmount(_pageStep);
    }
}

void
SliderBase::pointerButtonUpEvent(const PointerEvent& event)
{
  sigReleased();
  update();
}

void
SliderBase::pointerMotionEvent(const PointerEvent& event)
{
  if (pressed())
    {
      Point mouseLocal = mapToSurface(Point(event.x, event.y));
      int cursor;
      if (_orientation == Horizontal)
        {
          if (mouseLocal.x() < _sRect.x())
            cursor = 0;
          else if (mouseLocal.x() > _sRect.right())
            cursor = _sRect.width();
          else
            cursor = mouseLocal.x() - _sRect.x();
          setValue(_range * cursor / (_sRect.width() + .0));
        }
      else if (_inverted)
        {
          if (mouseLocal.y() < _sRect.y())
            cursor = 0;
          else if (mouseLocal.y() > _sRect.bottom())
            cursor = _sRect.height();
          else
            cursor = mouseLocal.y() - _sRect.y();
          setValue(_range * cursor / (_sRect.height() + .0));
        }
      else
        {
          if (mouseLocal.y() < _sRect.y())
            cursor = _sRect.height();
          else if (mouseLocal.y() > _sRect.bottom())
            cursor = 0;
          else
            cursor = _sRect.bottom() - mouseLocal.y();
          setValue(_range * cursor / (_sRect.height() + .0));
        }
    }
}

void
SliderBase::pointerWheelEvent(const PointerEvent& event)
{
  if (_orientation == Vertical)
    {
      if (_inverted)
        setValue(_value - event.wheelStep * _pageStep);
      else
        setValue(_value + event.wheelStep * _pageStep);
    }
  else
    setValue(_value - event.wheelStep * _pageStep);
}

void
SliderBase::enterEvent(const PointerEvent& mouseEvent)
{
  update();
}

void
SliderBase::leaveEvent(const PointerEvent& mouseEvent)
{
  update();
}
