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

#include "core/Window.h"
#include "core/AppBase.h"
#include "core/IFusion.h"
#include "core/Logger.h"

using namespace ilixi;

IDirectFB* Window::_dfb = NULL;
IDirectFBDisplayLayer* Window::_layer = NULL;
IDirectFBEventBuffer* Window::_buffer = NULL;
DFBDisplayLayerConfig Window::_layerConfig;
const DFBSurfacePixelFormat Window::_pixelFormat = DSPF_ARGB;
const bool Window::_doubleBuffered = true;
int Window::_windowCount = 0;
Window::windowList Window::_windowList;

Window* Window::_activeWindow = NULL;
pthread_mutex_t Window::_windowMutex = PTHREAD_MUTEX_INITIALIZER;

Window::Window() :
  _window(NULL), _windowSurface(NULL), _windowUIManager(NULL)
{
  _windowUIManager = new UIManager(this);
}

Window::~Window()
{
  delete _windowUIManager;
  releaseWindow();
  if (_windowCount == 0)
    releaseDFB();
}

IDirectFB*
Window::DFBInterface()
{
  return _dfb;
}

IDirectFBSurface*
Window::DFBSurface() const
{
  return _windowSurface;
}

void
Window::flipSurface(const DFBRegion& region)
{
  DFBResult ret = _windowSurface->Flip(_windowSurface, &region, DSFLIP_BLIT);
  if (ret)
    ILOG_ERROR("DFB Flip Error: %d", ret);
}

UIManager* const
Window::windowUIManager() const
{
  return _windowUIManager;
}

DFBSurfacePixelFormat
Window::pixelFormat()
{
  return _pixelFormat;
}

bool
Window::doubleBuffered()
{
  return _doubleBuffered;
}

void
Window::showWindow(TransitionStyle style, int value1, int value2)
{
  pthread_mutex_lock(&_windowMutex);

  // detach parent window's event buffer
  if (_parentWindow)
    _parentWindow->_window->DetachEventBuffer(_window, _buffer);

  if (_activeWindow)
    {
      _activeWindow->_windowUIManager->setGrabbedWidget(NULL);
      _activeWindow->_windowUIManager->setExposedWidget(NULL,
          PointerEvent(PointerMotion, 0, 0));
    }
  _activeWindow = this;
  _activeWindow->_windowUIManager->selectNext();

  // make visible
  _window->RaiseToTop(_window);

  if (style == None)
    {
      _window->SetOpacity(_window, 255);
    }
  else if (style == Fade)
    {
      for (int i = 0; i <= 255; i += 10)
        {
          _window->SetOpacity(_window, i);
          usleep(1000);
        }
    }
  else if (style == Slide)
    {
      _window->SetOpacity(_window, 255);
      if (value1 < value2)
        {
          for (int i = value1; i <= value2; i += 5)
            {
              _window->MoveTo(_window, 0, i);
              usleep(1000);
            }
          _window->MoveTo(_window, 0, value2);
        }
      else if (value1 > value2)
        {
          for (int i = value1; i >= value2; i -= 5)
            {
              _window->MoveTo(_window, 0, i);
              usleep(1000);
            }
          _window->MoveTo(_window, 0, value2);
        }
    }

  // attach event buffer
  _window->AttachEventBuffer(_window, _buffer);
  _window->RequestFocus(_window);
  _buffer->Reset(_buffer);

  pthread_mutex_unlock(&_windowMutex);
}

void
Window::hideWindow(TransitionStyle style, int value1, int value2)
{
  pthread_mutex_lock(&_windowMutex);

  // detach event buffer
  _window->DetachEventBuffer(_window, _buffer);
  _buffer->Reset(_buffer);

  if (style == None)
    {
      _window->SetOpacity(_window, 0);
    }
  else if (style == Fade)
    {
      for (int i = 255; i >= 0; i -= 10)
        {
          _window->SetOpacity(_window, i);
          usleep(1000);
        }
    }
  else if (style == Slide)
    {
      if (value1 < value2)
        {
          for (int i = value1; i <= value2; i += 5)
            {
              _window->MoveTo(_window, 0, i);
              usleep(1000);
            }
          _window->MoveTo(_window, 0, value2);
        }
      else if (value1 > value2)
        {
          for (int i = value1; i >= value2; i -= 5)
            {
              _window->MoveTo(_window, 0, i);
              usleep(1000);
            }
          _window->MoveTo(_window, 0, value2);
        }
      _window->SetOpacity(_window, 0);
    }

  _window->LowerToBottom(_window);

  // attach parent window's event buffer
  if (_parentWindow)
    {
      _activeWindow = _parentWindow;
      _parentWindow->_window->AttachEventBuffer(_window, _buffer);
      _parentWindow->_window->RequestFocus(_parentWindow->_window);
    }

  _windowUIManager->setExposedWidget(0, PointerEvent(PointerMotion, 0, 0));
  pthread_mutex_unlock(&_windowMutex);
}

void
Window::closeWindow()
{
  pthread_mutex_lock(&_windowMutex);

  _window->Close(_window);

  // attach parent window's event buffer
  if (_parentWindow)
    {
      _activeWindow = _parentWindow;
      _parentWindow->_window->AttachEventBuffer(_window, _buffer);
      _window->RequestFocus(_window);
    }

  pthread_mutex_unlock(&_windowMutex);
}

void
Window::moveWindowTo(int x, int y)
{
  _window->MoveTo(_window, x, y);
}

void
Window::resizeWindow(int width, int height)
{
  _window->Resize(_window, width, height);
}

void
Window::setBounds(int x, int y, int width, int height)
{
  _window->SetBounds(_window, x, y, width, height);
}

void
Window::setRotation(int rotation)
{
  _window->SetRotation(_window, rotation);
}

void
Window::initDFB(int argc, char **argv)
{
  ILOG_DEBUG( "Initialising DirectFB interface...");
  if (!_dfb)
    {
      DFBCHECK(DirectFBInit(&argc, &argv));
      DirectFBCreate(&_dfb);
      DFBCHECK(_dfb->GetDisplayLayer(_dfb, DLID_PRIMARY, &_layer));
      _layer->GetConfiguration(_layer, &_layerConfig);
      DFBCHECK(_dfb->CreateEventBuffer(_dfb, &_buffer));
      ILOG_DEBUG("DirectFB interface is ready.");
    }
}

void
Window::initDFBWindow(int x, int y, int w, int h, bool dialog)
{
  if (!_dfb)
    {
      ILOG_FATAL("DirectFB interface is not initialised!");
      exit( EXIT_FAILURE);
    }

  if (_window && dialog)
    {
      _window->SetBounds(_window, x, y, w, h);
      _window->ResizeSurface(_window, w, h);
      _window->GetSurface(_window, &_windowSurface);
      _windowDesc.posx = x;
      _windowDesc.posy = y;
      _windowDesc.width = w;
      _windowDesc.height = h;
      _windowUIManager->reset();
      ILOG_DEBUG("DirectFB window interface is modified!");
      return;
    }
  else if (_window)
    {
      ILOG_DEBUG("DirectFB window interface is already initialised!");
      _windowUIManager->reset();
      return;
    }
  else
    {
      ILOG_DEBUG( "Initialising DirectFB window interfaces...");

      // modify window description
      _windowDesc.flags = (DFBWindowDescriptionFlags)(
          DWDESC_POSX | DWDESC_POSY | DWDESC_WIDTH | DWDESC_HEIGHT
              | DWDESC_CAPS | DWDESC_SURFACE_CAPS | DWDESC_PIXELFORMAT
              | DWDESC_OPTIONS);
      _windowDesc.posx = x;
      _windowDesc.posy = y < 0 ? AppBase::appInstance->getStatusBarHeight() : y;
      _windowDesc.width = w ? w : _layerConfig.width;
      _windowDesc.height = h ? h : _layerConfig.height
          - AppBase::appInstance->getStatusBarHeight();
      _windowDesc.pixelformat = _pixelFormat;
      if (_doubleBuffered)
        _windowDesc.caps = (DFBWindowCapabilities)(
            DWCAPS_DOUBLEBUFFER | DWCAPS_ALPHACHANNEL);
      else
        _windowDesc.caps = (DFBWindowCapabilities)(DWCAPS_ALPHACHANNEL);
      _windowDesc.surface_caps = (DFBSurfaceCapabilities)(
          DSCAPS_PREMULTIPLIED | DSCAPS_FLIPPING);
      _windowDesc.options = (DFBWindowOptions)(DWOP_ALPHACHANNEL | DWOP_SCALE);

      // Create window and set visibility
      DFBCHECK(_layer->CreateWindow(_layer, &_windowDesc, &_window));
      _parentWindow = _activeWindow;
      _window->GetSurface(_window, &_windowSurface);
      _window->GetID(_window, &_DFBwindowID);
      if (dialog)
        _window->SetStackingClass(_window, DWSC_UPPER);
      pthread_mutex_lock(&_windowMutex);
      _windowCount++;
      _windowID = _windowList.size();
      _windowList.push_back(this);
      pthread_mutex_unlock(&_windowMutex);
      _windowUIManager->reset();
      ILOG_DEBUG( "DirectFB window interfaces are ready.");
    }
}

void
Window::releaseWindow()
{
  if (_window)
    {
      ILOG_DEBUG( "Releasing DirectFB window interfaces...");
      _windowSurface->Release(_windowSurface);
      _windowSurface = NULL;
      _window->Close(_window);
      _window->Destroy(_window);
      _window->Release(_window);
      _window = NULL;
      pthread_mutex_lock(&_windowMutex);
      _windowCount--;
      for (windowListIterator it = _windowList.begin(); it != _windowList.end(); ++it)
        {
          if (((Window*) *it)->_windowID == _windowID)
            {
              _windowList.erase(it);
              break;
            }
        }
      pthread_mutex_unlock(&_windowMutex);
      ILOG_DEBUG( "DirectFB window interfaces are released.");
    }
}

bool
Window::handleWindowEvent(const DFBWindowEvent& event)
{
}

void
Window::cfc()
{
  _windowSurface->Clear(_windowSurface, 0, 0, 0, 0);
  _windowSurface->Flip(_windowSurface, NULL, DSFLIP_FLUSH);
  _windowSurface->Clear(_windowSurface, 0, 0, 0, 0);
}

void
Window::releaseDFB()
{
  if (_dfb)
    {
      ILOG_DEBUG("Releasing DirectFB interfaces...");
      _buffer->Release(_buffer);
      ILOG_DEBUG("Releasing layer...");
      _layer->Release(_layer);
      _dfb->Release(_dfb);
      _dfb = NULL;
      _activeWindow = NULL;
      pthread_mutex_destroy(&_windowMutex);
      ILOG_DEBUG("DirectFB interface is released.");
    }
}

