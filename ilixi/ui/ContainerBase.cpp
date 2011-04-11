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

#include "ui/ContainerBase.h"
#include "core/UIManager.h"
#include <algorithm>
#include "core/Logger.h"

using namespace ilixi;

ContainerBase::ContainerBase(Widget* parent) :
  Widget(parent), _layout(NULL)
{
  setLayout(new LayoutBase());
  setInputMethod(PointerInputOnly);
  sigGeometryUpdated.connect(sigc::mem_fun(this,
      &ContainerBase::updateLayoutGeometry));
}

ContainerBase::~ContainerBase()
{
}

int
ContainerBase::heightForWidth(int width) const
{
  return _layout->heightForWidth(width);
}

Size
ContainerBase::preferredSize() const
{
  return _layout->preferredSize();
}

Rectangle
ContainerBase::childrenRect() const
{
}

unsigned int
ContainerBase::spacing() const
{
  return _layout->spacing();
}

void
ContainerBase::addWidget(Widget* widget)
{
  _layout->addWidget(widget);
}

LayoutBase*
ContainerBase::layout()
{
  return _layout;
}

WidgetResizeConstraint
ContainerBase::hConstraint() const
{
  return _layout->hConstraint();
}

WidgetResizeConstraint
ContainerBase::vConstraint() const
{
  return _layout->vConstraint();
}

void
ContainerBase::setLayout(LayoutBase* layout)
{
  if (layout)
    {
      removeChild(_layout);
      _layout = layout;
      addChild(_layout);
      raiseChildToFront(_layout);
    }
}

void
ContainerBase::setSpacing(unsigned int spacing)
{
  _layout->setSpacing(spacing);
}

void
ContainerBase::doLayout()
{
  if (_layout)
    _layout->tile();
  if (parent())
    parent()->doLayout();
}

void
ContainerBase::updateLayoutGeometry()
{
  _layout->setGeometry(0, 0, width(), height());
}

bool
ContainerBase::consumePointerEvent(const PointerEvent& pointerEvent)
{
  if (_inputMethod & PointerInput)
    {
      if (_frameGeometry.contains(pointerEvent.x, pointerEvent.y, true))
        {
          // priority is given to most recent child.
          for (WidgetListReverseIterator it = _children.rbegin(); it
              != _children.rend(); ++it)
            {
              // event is consumed by child.
              if (((Widget*) *it)->acceptsPointerInput()
                  && ((Widget*) *it)->consumePointerEvent(pointerEvent))
                return true;
            }
          if (uiManager())
            uiManager()->setExposedWidget(NULL, pointerEvent);
        }
    }
  return false;
}

