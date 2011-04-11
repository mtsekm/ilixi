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

#include "ui/HBoxLayout.h"
#include "core/Logger.h"

using namespace ilixi;

HBoxLayout::HBoxLayout(Widget* parent) :
  LayoutBase(parent)
{
  //  setConstraints(MinimumConstraint, MinimumConstraint);
}

HBoxLayout::~HBoxLayout()
{
}

int
HBoxLayout::heightForWidth(int width) const
{
  int h = 0; // total height
  int hTemp = 0; // height for current widget
  for (WidgetList::const_iterator it = _children.begin(); it != _children.end(); ++it)
    {
      if (((Widget*) *it)->visible() && ((Widget*) *it)->vConstraint()
          != IgnoredConstraint)
        {
          hTemp = ((Widget*) *it)->preferredSize().height();
          int h4w = ((Widget*) *it)->heightForWidth(width);
          // update hTemp if widget permits resizing in vertical axis.
          if (h4w)
            {
              if (h4w > hTemp && ((Widget*) *it)->vConstraint() & ShrinkPolicy)
                hTemp = h4w;

              else if (h4w < hTemp && ((Widget*) *it)->vConstraint()
                  & GrowPolicy)
                hTemp = h4w;
            }
          // Integrate height here
          h += hTemp + spacing();
        }
    }
  return h - spacing();
}

Size
HBoxLayout::preferredSize() const
{
  int w = 0, h = 0;
  // find max height and create an ActiveWidgetList

  Size s;
  for (WidgetList::const_iterator it = _children.begin(); it != _children.end(); ++it)
    {
      if (((Widget*) *it)->visible() && ((Widget*) *it)->vConstraint()
          != IgnoredConstraint)
        {
          s = (*it)->preferredSize();

          // handle min-max width (min has priority)
          if (s.width() < (*it)->minWidth())
            s.setWidth((*it)->minWidth());
          else if ((*it)->maxWidth() > 0 && s.width() > (*it)->maxWidth())
            s.setWidth((*it)->maxWidth());

          // handle min-max height (min has priority)
          if (s.height() < (*it)->minHeight())
            s.setHeight((*it)->minHeight());
          else if ((*it)->maxHeight() > 0 && s.height() > (*it)->maxHeight())
            s.setHeight((*it)->maxHeight());

          // update max. height
          if (s.height() > h)
            h = s.height();

          w += s.width() + spacing();
        }
    }
  return Size(w - spacing(), h);
}

void
HBoxLayout::updateActiveWidgetList()
{
  _numExpanding = 0;
  _activeWidgetList.clear();
  Widget* currentWidget;
  for (WidgetListIterator it = _children.begin(); it != _children.end(); ++it)
    {
      currentWidget = ((Widget*) *it);
      if (currentWidget->visible() && currentWidget->hConstraint()
          != IgnoredConstraint)
        {
          ActiveWidget aw;
          aw.widget = currentWidget;
          aw.size = currentWidget->preferredSize();

          // Handle Resize Policies
          bool wUpdated = true;
          // Update height using layout's height if widget permits it.
          if ((aw.size.height() > height()) && (aw.widget->vConstraint()
              & ShrinkPolicy))
            aw.size.setHeight(height());
          else if ((aw.size.height() < height()) && (aw.widget->vConstraint()
              & GrowPolicy))
            aw.size.setHeight(height());
          else
            wUpdated = false;

          // Update height using new Width if the widget permits it.
          //          if (wUpdated)
          //            {
          //              int h4w = currentWidget->heightForWidth(aw.size.width());
          //              if (h4w)
          //                {
          //                  if (h4w > aw.size.height() && aw.widget->vConstraint()
          //                      & GrowPolicy)
          //                    aw.size.setHeight(h4w);
          //                  else if (h4w < aw.size.height() && aw.widget->vConstraint()
          //                      & ShrinkPolicy)
          //                    aw.size.setHeight(h4w);
          //                }
          //            }
          if (currentWidget->hConstraint() & ExpandPolicy)
            ++_numExpanding;

          _activeWidgetList.push_back(aw);
        }
    }
}
void
HBoxLayout::tile()
{
  updateActiveWidgetList();
  if (_activeWidgetList.size() > 0)
    {
      ActiveWidgetList w = _activeWidgetList;
      int totalSpacing = (w.size() - 1) * spacing();
      int availableSpace = width() - totalSpacing;
      int average = availableSpace / w.size();
      ILOG_DEBUG("Layout(w,h) = (%d, %d)", width(), height());
      ILOG_DEBUG("Count: %d\t availableSpace: %d\t average: %d", w.size(), availableSpace, average);

      //***********************************************************
      // Handle FixedConstraint
      // Remove only fixed from temporary list (ignore min.size and max.size), decrement available size and
      // recalculate average if list is not empty.
      //***********************************************************
      int spaceUsed = 0;
      ActiveWidgetListIterator it = w.begin();
      while (it != w.end())
        {
          if (((ActiveWidget) *it).widget->hConstraint() == FixedConstraint
              && ((ActiveWidget) *it).widget->minWidth() < 0
              && ((ActiveWidget) *it).widget->maxWidth() < 0)
            {
              spaceUsed += ((ActiveWidget) *it).size.width();
              it = w.erase(it);
            }
          else
            ++it;
        }
      if (spaceUsed)
        {
          availableSpace -= spaceUsed;
          if (w.size())
            average = availableSpace / w.size();
          ILOG_DEBUG("Count@fixed: %d\t availableSpace: %d\t average: %d", w.size(), availableSpace, average);
        }

      //***********************************************************
      // Handle MinimumSize
      //***********************************************************
      spaceUsed = 0;
      it = w.begin();
      while (it != w.end())
        {
          if (average < ((ActiveWidget) *it).widget->minWidth())
            {
              spaceUsed += ((ActiveWidget) *it).widget->minWidth();
              it = w.erase(it);
            }
          else
            ++it;
        }
      if (spaceUsed)
        {
          availableSpace -= spaceUsed;
          if (w.size())
            average = availableSpace / w.size();
          ILOG_DEBUG("Count@min: %d\t availableSpace: %d\t average: %d", w.size(), availableSpace, average);
        }

      //***********************************************************
      // Handle MaximumSize
      //***********************************************************
      spaceUsed = 0;
      it = w.begin();
      while (it != w.end())
        {
          if (average > ((ActiveWidget) *it).widget->maxWidth()
              && ((ActiveWidget) *it).widget->maxWidth() > 0)
            {
              spaceUsed += ((ActiveWidget) *it).widget->maxWidth();
              it = w.erase(it);
            }
          else
            ++it;
        }
      if (spaceUsed)
        {
          availableSpace -= spaceUsed;
          if (w.size())
            average = availableSpace / w.size();
          ILOG_DEBUG("Count@max: %d\t availableSpace: %d\t average: %d", w.size(), availableSpace, average);
        }

      //***********************************************************
      //                     ShrinkPolicy
      //***********************************************************
      spaceUsed = 0;
      it = w.begin();
      while (it != w.end())
        {
          if (average < ((ActiveWidget) *it).size.width()
              && !(((ActiveWidget) *it).widget->hConstraint() & ShrinkPolicy))
            {
              spaceUsed += ((ActiveWidget) *it).size.width();
              it = w.erase(it);
            }
          else
            ++it;
        }
      if (spaceUsed)
        {
          availableSpace -= spaceUsed;
          if (w.size())
            average = availableSpace / w.size();
          ILOG_DEBUG("Count@minConstraint: %d\t availableSpace: %d\t average: %d", w.size(), availableSpace, average);
        }

      //***********************************************************
      //                     GrowPolicy
      //***********************************************************
      spaceUsed = 0;
      it = w.begin();
      while (it != w.end())
        {
          if (average > ((ActiveWidget) *it).size.width()
              && !(((ActiveWidget) *it).widget->hConstraint() & GrowPolicy))
            {
              spaceUsed += ((ActiveWidget) *it).widget->width();
              it = w.erase(it);
            }
          else
            ++it;
        }
      if (spaceUsed)
        {
          availableSpace -= spaceUsed;
          if (w.size())
            average = availableSpace / w.size();
          ILOG_DEBUG("Count@maxSize: %d\t availableSpace: %d\t average: %d", w.size(), availableSpace, average);
        }

      //***********************************************************
      // Handle ExpandingConstraint
      //***********************************************************
      int expandAverage = 0;
      if (_numExpanding)
        {
          int expandSpace = 0;
          it = w.begin();
          while (it != w.end())
            {
              if (!(((ActiveWidget) *it).widget->hConstraint() & ExpandPolicy))
                {
                  if (((ActiveWidget) *it).widget->minWidth() > 0 && average
                      > ((ActiveWidget) *it).widget->minWidth())
                    {
                      expandSpace += average
                          - ((ActiveWidget) *it).widget->minWidth();
                      ((ActiveWidget) *it).size.setWidth(
                          ((ActiveWidget) *it).widget->minWidth());
                    }
                  else if (average > ((ActiveWidget) *it).size.width())
                    expandSpace += average - ((ActiveWidget) *it).size.width();
                }
              ++it;
            }
          if (expandSpace)
            {
              expandAverage = expandSpace / _numExpanding;
              ILOG_DEBUG("expandAverage: %d, average: %d", expandAverage, average);
            }
        }
      //***********************************************************
      // Arrange widgets inside layout
      //***********************************************************
      int artifact = availableSpace - average * w.size();
      bool useArtifact = true;
      ILOG_DEBUG("artifact: %d", artifact);

      int currentX = 0;
      Widget* currentWidget;
      for (ActiveWidgetListIterator it = _activeWidgetList.begin(), end =
          _activeWidgetList.end(); it != end; ++it)
        {

          currentWidget = ((ActiveWidget) *it).widget;
          // Set height
          if (_numExpanding)
            {
              if (currentWidget->hConstraint() & ExpandPolicy)
                {
                  if (useArtifact)
                    {
                      currentWidget->setWidth(average + expandAverage
                          + artifact);
                      useArtifact = false;
                    }
                  else
                    currentWidget->setWidth(average + expandAverage);
                }
              else if (currentWidget->hConstraint() & ShrinkPolicy && average
                  < ((ActiveWidget) *it).size.width())
                currentWidget->setWidth(average);
              else
                currentWidget->setWidth(((ActiveWidget) *it).size.width());
            }
          else
            {
              if (currentWidget->minWidth() > 0 || currentWidget->maxWidth()
                  > 0)
                currentWidget->setWidth(average);
              else if ((currentWidget->hConstraint() & ShrinkPolicy)
                  && ((ActiveWidget) *it).size.width() > average)
                currentWidget->setWidth(average);
              else if ((currentWidget->hConstraint() & GrowPolicy)
                  && ((ActiveWidget) *it).size.width() < average)
                {
                  if (useArtifact)
                    {
                      currentWidget->setWidth(average + artifact);
                      useArtifact = false;
                    }
                  else
                    currentWidget->setWidth(average);
                }
              else
                currentWidget->setWidth(((ActiveWidget) *it).size.width());
            }
          currentWidget->setHeight(((ActiveWidget) *it).size.height());

          // FIXME align vcenter?
          int y = (height() - currentWidget->height()) / 2;

          currentWidget->moveTo(currentX, y);
          currentX += currentWidget->width() + spacing();

          ILOG_DEBUG("w: %d, h: %d", currentWidget->width(), currentWidget->height());
        }
      ILOG_DEBUG("Current_Y: %d", currentX);
    }
  _layoutModified = false;
}
