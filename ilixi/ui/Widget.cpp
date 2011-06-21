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

#include "ui/Widget.h"
#include "core/UIManager.h"
#include "core/Window.h"
#include <algorithm>
#include "core/Logger.h"

using namespace ilixi;

Designer* Widget::_designer = 0;

//*********************************************************************
// WidgetThread
//*********************************************************************
WidgetThread::WidgetThread(Widget* parent) :
  Thread(), _parent(parent), _delay(1000)
{
}

WidgetThread::~WidgetThread()
{
}

int
WidgetThread::run()
{
  usleep(_delay);
  return 1;
}

//*********************************************************************
// Widget
//*********************************************************************

Widget::Widget(Widget* parent) :
  _state(DefaultState), _surfaceDesc(DefaultDescription),
      _inputMethod(NoInput), _parent(parent), _surface(NULL), _uiManager(NULL),
      _horizontalResizeConstraint(NoConstraint),
      _verticalResizeConstraint(NoConstraint)
{
  sigGeometryUpdated.connect(sigc::mem_fun(this, &Widget::updateFrameGeometry));
}

Widget::~Widget()
{
  if (_uiManager)
    _uiManager->removeWidget(this);
  for (WidgetListIterator it = _children.begin(); it != _children.end(); ++it)
    delete *it;
  delete _surface;
}

int
Widget::x() const
{
  return _surfaceGeometry.x();
}

int
Widget::y() const
{
  return _surfaceGeometry.y();
}

int
Widget::absX() const
{
  if (_surfaceDesc & HasOwnSurface)
    return _surfaceGeometry.x() + _frameGeometry.x();
  else if (_parent)
    {
      if (_parent->_surfaceDesc & HasOwnSurface)
        return _surfaceGeometry.x() + _parent->_surfaceGeometry.x()
            + _parent->_frameGeometry.x();
      else
        return _surfaceGeometry.x() + _parent->_frameGeometry.x();
    }
  return _surfaceGeometry.x();
}

int
Widget::absY() const
{
  if (_surfaceDesc & HasOwnSurface)
    return _surfaceGeometry.y() + _frameGeometry.y();
  else if (_parent)
    {
      if (_parent->_surfaceDesc & HasOwnSurface)
        return _surfaceGeometry.y() + _parent->_surfaceGeometry.y()
            + _parent->_frameGeometry.y();
      else
        return _surfaceGeometry.y() + _parent->_frameGeometry.y();
    }
  return _surfaceGeometry.y();
}

Point
Widget::position() const
{
  return _surfaceGeometry.topLeft();
}

int
Widget::height() const
{
  return _surfaceGeometry.height();
}

int
Widget::width() const
{
  return _surfaceGeometry.width();
}

Size
Widget::size() const
{
  return _surfaceGeometry.size();
}

Rectangle
Widget::frameGeometry() const
{
  return _frameGeometry;
}

Rectangle
Widget::surfaceGeometry() const
{
  return _surfaceGeometry;
}

int
Widget::minHeight() const
{
  return _minSize.height();
}

int
Widget::minWidth() const
{
  return _minSize.width();
}

Size
Widget::minimumSize() const
{
  return _minSize;
}

int
Widget::maxHeight() const
{
  return _maxSize.height();
}

int
Widget::maxWidth() const
{
  return _maxSize.width();
}

Size
Widget::maximumSize() const
{
  return _maxSize;
}

int
Widget::heightForWidth(int width) const
{
  return -1;
}

Size
Widget::preferredSize() const
{
  return Size();
}

bool
Widget::enabled() const
{
  if (_parent)
    return !(_state & DisabledState) && _parent->enabled();
  return !(_state & DisabledState);
}

bool
Widget::visible() const
{
  if (_parent)
    return !(_state & InvisibleState) && _parent->visible();
  return !(_state & InvisibleState);
}

bool
Widget::hasFocus() const
{
  return _state & FocusedState;
}

bool
Widget::exposed() const
{
  return _state & ExposedState;
}

bool
Widget::pressed() const
{
  return _state & PressedState;
}

WidgetState
Widget::state() const
{
  return _state;
}

bool
Widget::acceptsPointerInput() const
{
  return (_inputMethod & PointerInput) && visible() && enabled();
}

bool
Widget::acceptsKeyInput() const
{
  return (_inputMethod & KeyInput) && visible() && enabled();
}

WidgetInputMethod
Widget::inputMethod() const
{
  return _inputMethod;
}

WidgetResizeConstraint
Widget::hConstraint() const
{
  return _horizontalResizeConstraint;
}

WidgetResizeConstraint
Widget::vConstraint() const
{
  return _verticalResizeConstraint;
}

Designer* const
Widget::designer()
{
  return _designer;
}

Widget*
Widget::parent() const
{
  return _parent;
}

Surface*
Widget::surface() const
{
  return _surface;
}

UIManager*
Widget::uiManager() const
{
  return _uiManager;
}

void
Widget::setY(int y)
{
  if (y != _surfaceGeometry.y())
    {
      _surfaceGeometry.setY(y);
      setSurfaceGeometryModified();
    }
}

void
Widget::setX(int x)
{
  if (x != _surfaceGeometry.x())
    {
      _surfaceGeometry.setX(x);
      setSurfaceGeometryModified();
    }
}

void
Widget::setWidth(int width)
{
  if (width != _surfaceGeometry.width())
    {
      if (width < _minSize.width())
        width = _minSize.width();
      else if (_maxSize.width() > 0 && width > _maxSize.width())
        width = _maxSize.width();
      _surfaceGeometry.setWidth(width);
      setSurfaceGeometryModified();
    }
}

void
Widget::setHeight(int height)
{
  if (height != _surfaceGeometry.height())
    {
      if (height < _minSize.height())
        height = _minSize.height();
      else if (_maxSize.height() > 0 && height > _maxSize.height())
        height = _maxSize.height();
      _surfaceGeometry.setHeight(height);
      setSurfaceGeometryModified();
    }
}

void
Widget::moveTo(int x, int y)
{
  if (x != _surfaceGeometry.x() || y != _surfaceGeometry.y())
    {
      _surfaceGeometry.moveTo(x, y);
      setSurfaceGeometryModified();
    }
}

void
Widget::setSize(int width, int height)
{
  setWidth(width);
  setHeight(height);
}

void
Widget::setSize(const Size &size)
{
  setWidth(size.width());
  setHeight(size.height());
}

void
Widget::setGeometry(int x, int y, int width, int height)
{
  moveTo(x, y);
  setWidth(width);
  setHeight(height);
}

void
Widget::setMinimumSize(const Size &size)
{
  _minSize = size;
}

void
Widget::setMinimumSize(int minWidth, int minHeight)
{
  _minSize.setWidth(minWidth);
  _minSize.setHeight(minHeight);
}

void
Widget::setMaximumSize(const Size &size)
{
  _maxSize = size;
}

void
Widget::setMaximumSize(int maxWidth, int maxHeight)
{
  _maxSize.setWidth(maxWidth);
  _maxSize.setHeight(maxHeight);
}

void
Widget::setHConstraint(WidgetResizeConstraint constraint)
{
  _horizontalResizeConstraint = constraint;
}

void
Widget::setVConstraint(WidgetResizeConstraint constraint)
{
  _verticalResizeConstraint = constraint;
}

void
Widget::setConstraints(WidgetResizeConstraint horizontal,
    WidgetResizeConstraint vertical)
{
  _horizontalResizeConstraint = horizontal;
  _verticalResizeConstraint = vertical;
}

void
Widget::addChild(Widget* child)
{
  if (!child)
    return;

  child->_parent = this;
  if (_uiManager)
    child->setUIManager(_uiManager);
  _children.push_back(child);
}

void
Widget::removeChild(Widget* child)
{
  if (!child)
    return;
  WidgetListIterator it = std::find(_children.begin(), _children.end(), child);
  if (child == *it)
    {
      delete *it;
      _children.erase(it);
    }
}

void
Widget::raiseChildToFront(Widget* child)
{
  if (!child)
    return;
  if (_children.size() == 1)
    return;
  WidgetListIterator it = std::find(_children.begin(), _children.end(), child);
  if (child == *it)
    std::iter_swap(it, _children.begin());
}

void
Widget::lowerChildToBottom(Widget* child)
{
  if (!child)
    return;
  if (_children.size() == 1)
    return;
  WidgetListIterator it = std::find(_children.begin(), _children.end(), child);
  if (child == *it)
    std::iter_swap(it, _children.end());
}

void
Widget::raiseChild(Widget* child)
{
  if (!child)
    return;
  if (_children.size() == 1)
    return;
  WidgetListIterator it = std::find(_children.begin(), _children.end(), child);
  if (child == *it && it != _children.begin())
    std::iter_swap(it, --it);
}

void
Widget::lowerChild(Widget* child)
{
  if (!child)
    return;
  if (_children.size() == 1)
    return;
  WidgetListIterator it = std::find(_children.begin(), _children.end(), child);
  if (child == *it && it != _children.end())
    std::iter_swap(it, ++it);
}

void
Widget::paintChildren(const Rectangle& rect)
{
  for (WidgetListIterator it = _children.begin(); it != _children.end(); ++it)
    ((Widget*) *it)->paint(rect);
}

void
Widget::setSurfaceGeometryModified()
{
  _surfaceDesc = (SurfaceDescription) (_surfaceDesc | SurfaceModified);
}

void
Widget::setEnabled()
{
  if ((_state & DisabledState))
    {
      _state = (WidgetState) (_state & ~DisabledState);
      update();
    }
}

void
Widget::setDisabled()
{
  if (!(_state & DisabledState))
    {
      _state = (WidgetState) (_state | DisabledState);
      update();
    }
}

void
Widget::setVisible(bool visible)
{
  if (visible && _state & InvisibleState)
    {
      _state = (WidgetState) (_state & ~InvisibleState);
      doLayout();
    }
  else if (!visible && !(_state & InvisibleState))
    {
      _state = (WidgetState) (_state | InvisibleState);
      doLayout();
    }
}

void
Widget::setFocus()
{
  _uiManager->setFocusedWidget(this);
}

void
Widget::setInputMethod(WidgetInputMethod method)
{
  _inputMethod = method;
}

void
Widget::setPointerTracking(bool pointerTracking)
{
  if (pointerTracking)
    _inputMethod = (WidgetInputMethod) (_inputMethod | PointerTracking);
  else
    _inputMethod = (WidgetInputMethod) (_inputMethod & ~PointerTracking);
}

void
Widget::paint(const Rectangle& rect)
{
  if (visible())
    {
      updateSurface();
      Rectangle intersect = _frameGeometry.intersected(rect);
      if (intersect.isValid())
        {
          compose(mapToSurface(intersect));
          paintChildren(intersect);
        }
    }
}

void
Widget::repaint()
{
  if (_surface && _parent && !(_state & InvisibleState))
    _parent->repaint(
        Rectangle(_frameGeometry.x(), _frameGeometry.y(),
            _surfaceGeometry.width(), _surfaceGeometry.height()));
}

void
Widget::repaint(const Rectangle& rect)
{
  if (_surface && _parent && !(_state & InvisibleState))
    {
      if (_surfaceDesc & HasOwnSurface)
        {
          _surface->clear(
              mapToSurface(rect.x(), rect.y(), rect.width(), rect.height()));
          _parent->repaint(_frameGeometry.intersected(rect));
        }
      else
        _parent->repaint(rect);
    }
}

void
Widget::update()
{
  if (_surface && _parent && !(_state & InvisibleState))
    _parent->update(_frameGeometry);
}

void
Widget::update(const Rectangle& rect)
{
  if (_surface && _parent && !(_state & InvisibleState))
    {
      if (_surfaceDesc & HasOwnSurface)
        {
          _surface->clear(
              mapToSurface(rect.x(), rect.y(), rect.width(), rect.height()));
          _parent->update(_frameGeometry.intersected(rect));
        }
      else
        _parent->update(rect);
    }
  else
    paint(rect);
}

void
Widget::doLayout()
{
  if (_parent)
    _parent->doLayout();
}

Rectangle
Widget::mapToSurface(const Rectangle& rect) const
{
  return mapToSurface(rect.x(), rect.y(), rect.width(), rect.height());
}

Rectangle
Widget::mapToSurface(int x, int y, int width, int height) const
{
  if (_surfaceDesc & HasOwnSurface)
    return Rectangle(x - (_surfaceGeometry.x() + _frameGeometry.x()),
        y - (_surfaceGeometry.y() + _frameGeometry.y()), width, height);
  else
    return Rectangle(x - _frameGeometry.x(), y - _frameGeometry.y(), width,
        height);
}

Point
Widget::mapToSurface(const Point& point) const
{
  if (_surfaceDesc & HasOwnSurface)
    return Point(point.x() - (_surfaceGeometry.x() + _frameGeometry.x()),
        point.y() - (_surfaceGeometry.y() + _frameGeometry.y()));
  else
    return Point(point.x() - _frameGeometry.x(), point.y() - _frameGeometry.y());
}

Rectangle
Widget::mapFromSurface(const Rectangle& rect) const
{
  return Rectangle(_frameGeometry.x() + rect.x(),
      _frameGeometry.y() + rect.y(), rect.width(), rect.height());
}

Rectangle
Widget::mapFromSurface(int x, int y, int width, int height) const
{
  return Rectangle(_frameGeometry.x() + x, _frameGeometry.y() + y, width,
      height);
}

Point
Widget::mapFromSurface(const Point& point) const
{
  return Point(_frameGeometry.x() + point.x(), _frameGeometry.y() + point.y());
}

void
Widget::updateSurface()
{
  if (!_surface && (_surfaceDesc & InitialiseSurface))
    {
      delete _surface;
      _surface = new Surface();
      bool ret = false;
      if (_surfaceDesc & HasOwnSurface)
        ret = _surface->createDFBSurface(width(), height());
      else if (_surfaceDesc & RootWindow)
        ret = _surface->createDFBSubSurface(_surfaceGeometry,
            _uiManager->root()->DFBSurface());
      else if (_parent)
        ret = _surface->createDFBSubSurface(_surfaceGeometry,
            _parent->surface()->DFBSurface());
      if (ret)
        _surfaceDesc = (SurfaceDescription) (_surfaceDesc & ~InitialiseSurface);
    }

  if (_surfaceDesc & SurfaceModified)
    sigGeometryUpdated();
}

void
Widget::updateFrameGeometry()
{
  if (!(_surfaceDesc & HasOwnSurface))
    _frameGeometry.moveTo(absX(), absY());

  if (_surfaceDesc & SharedSize)
    {
      _frameGeometry.setWidth(width());
      _frameGeometry.setHeight(height());
    }

  if (_surface)
    _surface->setGeometry(_surfaceGeometry);

  _surfaceDesc = (SurfaceDescription) (_surfaceDesc & ~SurfaceModified);
}

void
Widget::flip(const Rectangle& rect)
{
  if (_surface)
    _surface->flip(rect);
}

void
Widget::blit(Widget* widget, const Rectangle& clip, int x, int y)
{
  if (_surface)
    widget->surface()->blit(_surface, clip, x, y);
}

void
Widget::keyDownEvent(const KeyEvent& keyEvent)
{
}

void
Widget::keyUpEvent(const KeyEvent& keyEvent)
{
}

void
Widget::pointerButtonDownEvent(const PointerEvent& mouseEvent)
{
}

void
Widget::pointerButtonUpEvent(const PointerEvent& mouseEvent)
{
}

void
Widget::pointerMotionEvent(const PointerEvent& mouseEvent)
{
}

void
Widget::pointerWheelEvent(const PointerEvent& mouseEvent)
{
}

void
Widget::focusInEvent()
{
}

void
Widget::focusOutEvent()
{
}

void
Widget::enterEvent(const PointerEvent& mouseEvent)
{
}

void
Widget::leaveEvent(const PointerEvent& mouseEvent)
{
}

bool
Widget::consumePointerEvent(const PointerEvent& pointerEvent)
{
  if (_uiManager->grabbedWidget() == this || _frameGeometry.contains(
      pointerEvent.x, pointerEvent.y, true))
    {
      // check children first.
      if (_children.size())
        {
          // priority is given to most recent child.
          for (WidgetListReverseIterator it = _children.rbegin(); it
              != _children.rend(); ++it)
            if (((Widget*) *it)->acceptsPointerInput()
                && ((Widget*) *it)->consumePointerEvent(pointerEvent))
              return true;

        }
      if (pointerEvent.eventType == PointerButtonDown)
        {
          _state = (WidgetState) (_state | PressedState);
          _uiManager->setFocusedWidget(this);
          _uiManager->setGrabbedWidget(this);
          pointerButtonDownEvent(pointerEvent);
        }
      else if (pointerEvent.eventType == PointerButtonUp)
        {
          _state = (WidgetState) (_state & ~PressedState);
          _uiManager->setGrabbedWidget(NULL);
          pointerButtonUpEvent(pointerEvent);
        }
      else if (pointerEvent.eventType == PointerWheel)
        {
          _uiManager->setFocusedWidget(this);
          pointerWheelEvent(pointerEvent);
        }
      else if (pointerEvent.eventType == PointerMotion)
        {
          if (_inputMethod & PointerTracking)
            pointerMotionEvent(pointerEvent);
          _uiManager->setExposedWidget(this, pointerEvent);
        }
      return true;
    }
  return false;
}

bool
Widget::consumeKeyEvent(const KeyEvent& keyEvent)
{
  if (_inputMethod & KeyInput)
    {
      if (keyEvent.eventType == KeyUpEvent)
        keyUpEvent(keyEvent);
      else
        keyDownEvent(keyEvent);
      return true;
    }
  return false;
}

void
Widget::setUIManager(UIManager* uiManager)
{
  _surfaceDesc = (SurfaceDescription) (_surfaceDesc | InitialiseSurface);
  _uiManager = uiManager;
  for (WidgetListIterator it = _children.begin(); it != _children.end(); ++it)
    {
      (*it)->setUIManager(uiManager);
      _uiManager->addWidget(*it);
    }
}

void
Widget::invalidateSurface()
{
  _surfaceDesc = (SurfaceDescription) (_surfaceDesc & ~InitialiseSurface);
  delete _surface;
  _surface = NULL;
  for (WidgetListIterator it = _children.begin(); it != _children.end(); ++it)
    (*it)->invalidateSurface();
}
