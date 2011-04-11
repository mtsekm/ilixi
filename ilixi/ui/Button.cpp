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

#include "ui/Button.h"
#include "graphics/Painter.h"
#include "core/Logger.h"

using namespace ilixi;

Button::Button(std::string text, Widget* parent) :
  TextLayout(text, parent), _checkable(false), _checked(false),
      _doClick(false), _icon(0)
{
  setInputMethod(KeyAndPointerInput);
}

Button::~Button()
{
  delete _icon;
}

bool
Button::checkable() const
{
  return _checkable;
}

bool
Button::checked() const
{
  if (_checkable)
    return _checked;
  return false;
}

Size
Button::getIconSize() const
{
  if (_icon)
    return _icon->size();
  return Size();
}

Image*
Button::icon() const
{
  return _icon;
}

Point
Button::iconPosition() const
{
  return _iconPosition;
}

bool
Button::hasIcon() const
{
  if (_icon)
    return true;
  return false;
}

void
Button::click(unsigned int ms)
{
  if (ms)
    {
      _state = (WidgetState) (_state ^ PressedState);
      update();
      usleep(ms * 1000);
      _state = (WidgetState) (_state ^ PressedState);
    }
  sigClicked();
  toggleState();
}

void
Button::setCheckable(bool checkable)
{
  _checkable = checkable;
}

void
Button::setChecked(bool checked)
{
  if (_checkable && checked != _checked)
    {
      _checked = checked;
      // FIXME signal checked here?
      // sigCheckChanged(_checked);
      update();
    }
}

void
Button::setIcon(const std::string& iconPath, const Size& size)
{
  Image* temp = new Image(iconPath, size);
  if (temp->getDFBSurface())
    {
      delete _icon;
      _icon = temp;
    }
  else
    delete temp;
}

void
Button::keyUpEvent(const KeyEvent& keyEvent)
{
  if (keyEvent.keySymbol == DIKS_SPACE)
    click(50);
}

void
Button::pointerButtonDownEvent(const PointerEvent& mouseEvent)
{
  _doClick = true;
  sigPressed();
  update();
}

void
Button::pointerButtonUpEvent(const PointerEvent& mouseEvent)
{
  sigReleased();
  if (_doClick)
    {
      sigClicked();
      toggleState();
      _doClick = false;
    }
}

void
Button::enterEvent(const PointerEvent& mouseEvent)
{
  update();
}

void
Button::leaveEvent(const PointerEvent& mouseEvent)
{
  update();
}

void
Button::toggleState()
{
  if (_checkable)
    {
      _checked = !_checked;
      sigCheckChanged(_checked);
    }
  update();
}

Font*
Button::defaultFont() const
{
  return designer()->defaultFont(ButtonFont);
}
