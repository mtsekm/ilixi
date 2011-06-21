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

#include "WindowWidget.h"
#include "core/Logger.h"

using namespace ilixi;

WindowWidget::WindowWidget(Widget* parent) :
  Window(), Frame(parent)
{
  pthread_mutex_init(&_updates._listLock, NULL);
  sem_init(&_updates._updateReady, 0, 0);
  sem_init(&_updates._paintReady, 0, 1);
  _surfaceDesc = WindowDescription;
  setUIManager(_windowUIManager);
  setMargin(5);
}

WindowWidget::~WindowWidget()
{
  pthread_mutex_destroy(&_updates._listLock);
  sem_destroy(&_updates._updateReady);
  sem_destroy(&_updates._paintReady);
}

void
WindowWidget::doLayout()
{
  update();
}

void
WindowWidget::paint(const Rectangle& rect)
{
  if (visible())
    {
      int ready;
      sem_getvalue(&_updates._updateReady, &ready);
      if (!ready)
        {
          pthread_mutex_lock(&_updates._listLock);
          _updates._updateQueue.push_back(rect);
          pthread_mutex_unlock(&_updates._listLock);
        }
      else
        {
          sem_wait(&_updates._updateReady);
          updateSurface();
          Rectangle intersect = _frameGeometry.intersected(
              _updates._updateRegion);
          sem_post(&_updates._paintReady);
          if (intersect.isValid())
            {
              if (_backgroundFilled)
                {
                  surface()->clear(intersect);
                  compose(intersect);
                }
              else
                surface()->clear(intersect);

              paintChildren(intersect);
              surface()->flip(intersect);
            }
        }
    }
}

void
WindowWidget::repaint(const Rectangle& rect)
{
  sem_wait(&_updates._paintReady);
  _updates._updateRegion = rect;
  ILOG_DEBUG("Repainting Rect(%d, %d, %d, %d)...", rect.x(),
      rect.y(), rect.width(), rect.height());
  sem_post(&_updates._updateReady);
  paint(_updates._updateRegion);
}

bool
WindowWidget::handleWindowEvent(const DFBWindowEvent& event)
{
  Widget* target = this;
  if (_windowUIManager->grabbedWidget())
    target = _windowUIManager->grabbedWidget();

  switch (event.type)
    {
  case DWET_MOTION:
    return target->consumePointerEvent(
        PointerEvent(PointerMotion, event.x, event.y));

  case DWET_BUTTONDOWN:
    return target->consumePointerEvent(
        PointerEvent(PointerButtonDown, event.x, event.y, 0,
            (PointerButton) event.button, (PointerButtonMask) event.buttons));

  case DWET_BUTTONUP:
    return target->consumePointerEvent(
        PointerEvent(PointerButtonUp, event.x, event.y, 0,
            (PointerButton) event.button, (PointerButtonMask) event.buttons));

  case DWET_WHEEL:
    return target->consumePointerEvent(
        PointerEvent(PointerWheel, event.x, event.y, event.step));

  case DWET_KEYUP:
    if (_windowUIManager->focusedWidget())
      return _windowUIManager->focusedWidget()->consumeKeyEvent(
          KeyEvent(KeyUpEvent, event.key_symbol, event.modifiers, event.locks));
    return false;

  case DWET_KEYDOWN:
    if (_windowUIManager->focusedWidget())
      return _windowUIManager->focusedWidget()->consumeKeyEvent(
          KeyEvent(KeyDownEvent, event.key_symbol, event.modifiers, event.locks));
    return false;

  default:
    return false;
    }
}

void
WindowWidget::updateWindow()
{
  if (!_window)
    return;

  if (_updates._updateQueue.size())
    {
      pthread_mutex_lock(&_updates._listLock);
      Rectangle updateTemp;
      updateTemp = _updates._updateQueue[0];
      if (_updates._updateQueue.size() > 1)
        for (int i = 1; i < _updates._updateQueue.size(); i++)
          updateTemp = updateTemp.united(_updates._updateQueue[i]);
      _updates._updateQueue.clear();
      pthread_mutex_unlock(&_updates._listLock);

      if (!updateTemp.isNull())
        {
          sem_wait(&_updates._paintReady);
          _updates._updateRegion = updateTemp;
          ILOG_DEBUG("Updating Rect(%d, %d, %d, %d)...", updateTemp.x(),
              updateTemp.y(), updateTemp.width(), updateTemp.height());
          sem_post(&_updates._updateReady);
          paint(_updates._updateRegion);
        }
    }
}
