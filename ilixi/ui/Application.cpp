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

#include "ui/Application.h"
#include "graphics/Painter.h"
#include "graphics/TDesigner.h"
#include "core/Logger.h"

using namespace ilixi;

Application::Application(int argc, char* argv[]) :
  AppBase(argc, argv), WindowWidget(), _fullscreen(false)
{
  _surfaceDesc = WindowDescription;
  initDFB(NULL, NULL);
  setUIManager(_windowUIManager);
  setDesigner("TDesigner", "stylesheet.xml");
  //  _designer = getDesigner();
  setTitle("Untitled");
  setBackgroundFilled(false);

  setMargins(5, 5, 5, 5);
  setCanvasPosition(Point(0, 0));
  setBorderStyle(NoBorder);
  setAppMode(Ready);
  callMaestro(Notification, Ready);
  ILOG_NOTICE("%d is ready!", getpid());
}

Application::~Application()
{
  delete _designer;
}

void
Application::onHide()
{
}

void
Application::onShow()
{
}

void
Application::quit()
{
  setAppMode(Terminated);
}

void
Application::exec()
{
  ILOG_DEBUG( "%s is starting...", title().c_str());
  callMaestro(ModeRequest, Visible);

  // enter event loop
  bool quit = false;
  DFBWindowEvent event;

  while (!quit)
    {

      if (appMode() == Terminated)
        quit = true;

      if (!_activeWindow)
        {
          usleep(1000);
          continue;
        }

      _buffer->WaitForEventWithTimeout(_buffer, 0, 50);
      while (_buffer->GetEvent(_buffer, DFB_EVENT(&event)) == DFB_OK)
        {
          // Ignore events outside active window.
          if (_activeWindow->_DFBwindowID != event.window_id)
            continue;

          // handle Close event.
          if (_activeWindow == this && event.type == DWET_CLOSE)
            {
              ILOG_DEBUG("QUITTING");
              quit = true;
              break;
            }

          // handle Leave event.
          else if (event.type == DWET_LEAVE)
            {
              _activeWindow->_windowUIManager->setExposedWidget(NULL,
                  PointerEvent(PointerMotion, event.x, event.y));
              _activeWindow->_windowUIManager->setGrabbedWidget(NULL);
              continue;
            }

          // handle TAB release.
          else if (event.key_symbol == DIKS_TAB && event.type == DWET_KEYUP)
            {
              if (event.modifiers == DIMM_SHIFT)
                _activeWindow->_windowUIManager->selectPrevious();
              else
                _activeWindow->_windowUIManager->selectNext();
              continue;
            }

          else
            _activeWindow->handleWindowEvent(event);
        }

      // Paint windows which have updates pending
      for (windowListIterator it = _windowList.begin(); it != _windowList.end(); ++it)
        ((Window*) *it)->updateWindow();
    }

  ILOG_DEBUG( "Stopping...");
  callMaestro(Notification, Terminated);
}

void
Application::show(Window::TransitionStyle style, int value1, int value2)
{
  if (appMode() != Visible)
    {
      initDFBWindow();
      setSize(_windowDesc.width, _windowDesc.height);
      setUIManager(_windowUIManager);
      if (_backgroundFilled && _windowSurface)
        cfc();
      setVisible(true);
      paint(Rectangle(0, 0, width(), height()));
      showWindow(style, value1, value2);
      setAppMode(Visible);
      callMaestro(Notification, Visible);
      onShow();
    }
}

void
Application::hide(Window::TransitionStyle style, int value1, int value2)
{
  if (appMode() != Hidden)
    {
      setVisible(false);
      onHide();
      hideWindow(style, value1, value2);
      setAppMode(Hidden);
      callMaestro(Notification, Hidden);
    }
}

void
Application::setDesigner(const char* designer, const char* styleSheet)
{
  if (_designer)
    return;
  if (!strcmp(designer, "TDesigner"))
    _designer = new TDesigner(styleSheet);
  else
    _designer = new Designer(styleSheet);
}

void
Application::setFullScreen(bool fullscreen)
{
  _fullscreen = fullscreen;
}

void
Application::setBackgroundImage(std::string imagePath)
{
  // TODO Background image.
}

void
Application::compose(const Rectangle& rect)
{
  Painter painter(this);
  painter.begin(rect);
  designer()->drawAppFrame(&painter, this);
  painter.end();
}

ReactionResult
Application::reactorCB(ReactorMessage *msg, void *ctx)
{
  switch (msg->type)
    {
  case SwitchMode:
    if (msg->senderFusionID == 1)
      {
        ILOG_INFO("Received SwitchMode message from Maestro.");
        if (msg->mode == Visible)
          {
            show();
            return RS_OK;
          }
        else if (msg->mode == Hidden)
          {
            if (_activeWindow->_windowUIManager->oskWidget())
              {
                callMaestro(OSKEvent, Hidden);
              }
            hide();
            return RS_OK;
          }
        else if (msg->mode == Terminated)
          {
            if (_activeWindow->_windowUIManager->oskWidget())
              callMaestro(OSKEvent, Hidden);
            quit();
            return RS_OK;
          }
        else
          ILOG_ERROR("Message mode is not supported!");
      }
    else
      {
        ILOG_ERROR("Sender is not authorised!");
        return RS_REMOVE;
      }
    break;

  case OSKEvent:
    if (msg->senderFusionID == 1)
      _activeWindow->_windowUIManager->setOSKWidgetText(getOSKText());
    else
      {
        ILOG_ERROR("Sender is not authorised!");
        return RS_REMOVE;
      }
    break;

  default:
    ILOG_ERROR("Message type is not supported!");
    }
  return RS_DROP;
}
