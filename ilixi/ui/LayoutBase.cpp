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

#include "ui/LayoutBase.h"
#include "ui/RadioButton.h"
#include "core/UIManager.h"
#include <algorithm>

#include "core/Logger.h"
using namespace ilixi;

LayoutBase::LayoutBase(Widget* parent) :
  Widget(parent), _layoutModified(false), _spacing(5), _numExpanding(0)
{
  _group = new RadioGroup();
  setInputMethod(PointerInputOnly);
  sigGeometryUpdated.connect(
      sigc::mem_fun(this, &LayoutBase::updateChildrenFrameGeometry));
}

LayoutBase::~LayoutBase()
{
}

int
LayoutBase::heightForWidth(int width) const
{
  return -1;
}

Size
LayoutBase::preferredSize() const
{
  Rectangle r;
  for (WidgetListConstIterator it = _children.begin(); it != _children.end(); ++it)
    {
      if (((Widget*) *it)->visible() && ((Widget*) *it)->hConstraint()
          != IgnoredConstraint && ((Widget*) *it)->vConstraint()
          != IgnoredConstraint)
        {
          Rectangle rTemp;
          rTemp.setTopLeft(((Widget*) *it)->position());
          rTemp.setSize(((Widget*) *it)->preferredSize());
          r.united(rTemp);
        }
    }
  return r.size();
}

Rectangle
LayoutBase::contentsRect()
{
  Rectangle r;
  for (WidgetListConstIterator it = _children.begin(); it != _children.end(); ++it)
    if (((Widget*) *it)->visible() && ((Widget*) *it)->hConstraint()
        != IgnoredConstraint && ((Widget*) *it)->vConstraint()
        != IgnoredConstraint)
      r.united(((Widget*) *it)->surfaceGeometry());
  return r;
}

int
LayoutBase::count()
{
  return _children.size();
}

unsigned int
LayoutBase::spacing() const
{
  return _spacing;
}

void
LayoutBase::setSpacing(unsigned int spacing)
{
  _spacing = spacing;
}

void
LayoutBase::setVisible(bool visible)
{
  if (visible && _state & InvisibleState)
    {
      _state = (WidgetState) (_state & ~InvisibleState);
      if (parent())
        parent()->doLayout();
    }
  else if (!visible && !(_state & InvisibleState))
    {
      _state = (WidgetState) (_state | InvisibleState);
      if (parent())
        parent()->doLayout();
    }
}

void
LayoutBase::addWidget(Widget* widget)
{
  if (!widget)
    return;
  addChild(widget);
  _layoutModified = true;
  RadioButton* radio = dynamic_cast<RadioButton*> (widget);
  if (radio)
    _group->add(radio);
}

void
LayoutBase::tile()
{
  /*
   * You should calculate total available space and divide it among widgets and satisfy all constraints.
   * In general you could start by updating the active widget list and get preferred size for each widget.
   * If width of a widget is changed (e.g. stretched horizontally) preferred heights should be updated
   * using heightForWidth() if provided.
   *
   * Your layout algorithm should satisfy constraints in the following order:
   * 1. Fixed Constraint
   * 2. Minimum Size
   * 3. Maximum Size
   * 4. Minimum Constraint
   * 5. Maximum Constraint
   * 6. Expanding Constraint
   *
   * Finally, you could update geometry of an active widget after taking its GrowPolicy and ShrinkPolicy into account.
   */
  for (WidgetList::const_iterator it = _children.begin(); it != _children.end(); ++it)
    if (((Widget*) *it)->visible()
        && ((Widget*) *it)->surfaceGeometry().isNull())
      ((Widget*) *it)->setSize(((Widget*) *it)->preferredSize());

  _layoutModified = false; // do not forget to set this flag to false!!
}

void
LayoutBase::doLayout()
{
  _layoutModified = true;
  if (parent())
    parent()->doLayout();
}

void
LayoutBase::paint(const Rectangle& rect)
{
  if (visible())
    {
      updateSurface();
      Rectangle intersect = _frameGeometry.intersected(rect);
      if (intersect.isValid())
        {
          if (_layoutModified)
            tile();
          paintChildren(intersect);
        }
    }
}

void
LayoutBase::updateChildrenFrameGeometry()
{
  _layoutModified = true;
  for (WidgetList::const_iterator it = _children.begin(); it != _children.end(); ++it)
    ((Widget*) *it)->sigGeometryUpdated();
}

void
LayoutBase::compose(const Rectangle& rect)
{
}

bool
LayoutBase::consumePointerEvent(const PointerEvent& pointerEvent)
{
  // priority is given to most recent child.
  for (WidgetListReverseIterator it = _children.rbegin(); it
      != _children.rend(); ++it)
    {
      if (((Widget*) *it)->acceptsPointerInput()
          && ((Widget*) *it)->consumePointerEvent(pointerEvent))
        return true;
    }
  return false;
}

void
LayoutBase::updateActiveWidgetList()
{
}

//**************************************************************

Spacer::Spacer(Orientation orientation, Widget* parent) :
  Widget(parent)
{
  setOrientation(orientation);
}

Spacer::~Spacer()
{
}

Size
Spacer::preferredSize() const
{
  return Size(1, 1);
}

void
Spacer::setOrientation(Orientation orientation)
{
  if (orientation == Horizontal)
    setConstraints(ExpandingConstraint, FixedConstraint);
  else
    setConstraints(FixedConstraint, ExpandingConstraint);
}

void
Spacer::paint(const Rectangle& rect)
{
}

void
Spacer::compose(const Rectangle& rect)
{
}
