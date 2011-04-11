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

#include "core/UIManager.h"
#include "core/AppBase.h"
#include "ui/Widget.h"
#include "ui/TextLayout.h"
#include "core/Logger.h"

using namespace ilixi;

UIManager::UIManager(Window* root) :
  _rootWindow(root), _focusedWidget(0), _exposedWidget(0), _grabbedWidget(0),
      _oskWidget(0)
{
}

UIManager::~UIManager()
{
}

Window*
UIManager::root()
{
  return _rootWindow;
}

Widget*
UIManager::exposedWidget() const
{
  return _exposedWidget;
}

Widget*
UIManager::focusedWidget() const
{
  return _focusedWidget;
}

Widget*
UIManager::grabbedWidget() const
{
  return _grabbedWidget;
}

Widget*
UIManager::oskWidget() const
{
  return _oskWidget;
}

void
UIManager::reset()
{
  widgets.clear();
  _focusedWidget = 0;
  _exposedWidget = 0;
}

void
UIManager::addWidget(Widget* widget)
{
  if (!widget)
    return;
  widgets.push_back(widget);
  setFocusedWidget(widget);
}

void
UIManager::removeWidget(Widget* widget)
{
  if (!widget)
    return;

  for (widgetListIterator it = widgets.begin(); it != widgets.end(); ++it)
    {
      if ((Widget*) *it == widget)
        {
          widgets.erase(it);
          break;
        }
    }
}

bool
UIManager::setExposedWidget(Widget* widget, const PointerEvent& pointerEvent)
{
  if (widget == _exposedWidget)
    return false;

  if (_exposedWidget)
    {
      _exposedWidget->_state = (WidgetState) (_exposedWidget->_state
          & ~ExposedState);
      _exposedWidget->_state = (WidgetState) (_exposedWidget->_state
          & ~PressedState);
      _exposedWidget->leaveEvent(pointerEvent);
    }

  _exposedWidget = widget;
  if (_exposedWidget)
    {
      _exposedWidget->_state = (WidgetState) (_exposedWidget->_state
          | ExposedState);
      _exposedWidget->enterEvent(pointerEvent);
    }

  return true;
}

bool
UIManager::setFocusedWidget(Widget* widget)
{
  if (widget == _focusedWidget || widget == NULL
      || !(widget->acceptsKeyInput()))
    return false;

  if (_focusedWidget)
    {
      _focusedWidget->_state = (WidgetState) (_focusedWidget->_state
          & ~FocusedState);
      _focusedWidget->update();
      _focusedWidget->focusOutEvent();
      if (_oskWidget)
        {
          AppBase::appInstance->callMaestro(OSKEvent, Hidden);
          _oskWidget = NULL;
        }
    }

  _focusedWidget = widget;
  _focusedWidget->_state
      = (WidgetState) (_focusedWidget->_state | FocusedState);
  _focusedWidget->update();
  _focusedWidget->focusInEvent();
  setOSKWidget(widget);
  return true;
}

bool
UIManager::setGrabbedWidget(Widget* widget)
{
  _grabbedWidget = widget;
  return true;
}

bool
UIManager::setOSKWidget(Widget* widget)
{
  if (widget == _oskWidget || widget == NULL || !(widget->inputMethod()
      & OSKInput))
    return false;
  TextLayout* tw = dynamic_cast<TextLayout*> (_focusedWidget);
  if (tw)
    {
      _oskWidget = tw;
      AppBase::appInstance->setOSKText(tw->text());
      AppBase::appInstance->callMaestro(OSKEvent, Visible);
    }
  return true;
}

void
UIManager::selectNext(bool found, bool iter)
{
  for (widgetListIterator it = widgets.begin(); it != widgets.end(); ++it)
    {
      if (found && setFocusedWidget(*it))
        return;

      if (*it == _focusedWidget)
        found = true;
    }

  if (iter)
    return;

  selectNext(true, true);
}

void
UIManager::selectPrevious(bool found, bool iter)
{
  for (widgetListReverseIterator it = widgets.rbegin(); it != widgets.rend(); ++it)
    {
      if (found && setFocusedWidget(*it))
        return;

      if (*it == _focusedWidget)
        found = true;
    }

  if (iter)
    return;

  selectPrevious(true, true);
}

void
UIManager::setOSKWidgetText(char* text)
{
  if (_oskWidget)
    {
      TextLayout* tw = dynamic_cast<TextLayout*> (_focusedWidget);
      if (tw)
        {
          tw->setText(text);
          _oskWidget = NULL;
        }
    }
}
