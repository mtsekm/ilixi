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

#include "ui/DirectionalButton.h"
#include "graphics/Painter.h"

using namespace ilixi;

DirectionalButton::DirectionalButton(DirectionalButtonType type,
    Direction direction, Widget* parent) :
  Widget(parent), _type(type), _direction(direction), _doClick(false)
{
  setInputMethod(PointerInputOnly);
}

DirectionalButton::~DirectionalButton()
{
}

Direction
DirectionalButton::direction() const
{
  return _direction;
}

DirectionalButton::DirectionalButtonType
DirectionalButton::type() const
{
  return _type;
}

void
DirectionalButton::setDirection(Direction direction)
{
  _direction = direction;
}

void
DirectionalButton::setType(DirectionalButtonType type)
{
  _type = type;
}

void
DirectionalButton::pointerButtonDownEvent(const PointerEvent& mouseEvent)
{
  _doClick = true;
  update();
  sigPressed();
}

void
DirectionalButton::pointerButtonUpEvent(const PointerEvent& mouseEvent)
{
  if (_doClick)
    {
      update();
      sigClicked();
    }
  sigReleased();
  _doClick = false;
}

void
DirectionalButton::enterEvent(const PointerEvent& mouseEvent)
{
  update();
}

void
DirectionalButton::leaveEvent(const PointerEvent& mouseEvent)
{
  update();
}

void
DirectionalButton::compose(const Rectangle& rect)
{
  Painter p(this);
  p.begin(rect);
  designer()->drawDirectionalButton(&p, this);
  p.end();
}
