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

#include "ui/VBoxLayout.h"
#include "core/Logger.h"

using namespace ilixi;

VBoxLayout::VBoxLayout(Widget* parent) :
  LayoutBase(parent)
{
  //  setConstraints(NoConstraint, MinimumConstraint);
}

VBoxLayout::~VBoxLayout()
{
}

int
VBoxLayout::heightForWidth(int width) const
{
  int h = 0; // total height
  int hCurrent = 0; // height for current widget
  int hMax = 0; // current widget's maximum height.
  int hMin = 0; // current widget's minimum height.
  bool checkGrow; // set to true, if vertical growing is supported for widget.
  bool checkShrink; // set to true, if vertical shrinking is supported for widget.
  Widget* currentWidget;
  for (WidgetList::const_iterator it = _children.begin(); it != _children.end(); ++it)
    {
      currentWidget = ((Widget*) *it);
      if (currentWidget->visible() && currentWidget->vConstraint()
          != IgnoredConstraint)
        {
          hMin = currentWidget->minHeight();
          hMax = currentWidget->maxHeight();
          Size pref = currentWidget->preferredSize();
          hCurrent = pref.height();

          if (currentWidget->vConstraint() & GrowPolicy)
            checkGrow = true;
          else
            checkGrow = false;

          if (currentWidget->vConstraint() & ShrinkPolicy)
            checkShrink = true;
          else
            checkShrink = false;

          // check for minimum and maximum height.
          if (hMin > hCurrent)
            {
              // Widget can not shrink since its minimum height is reached.
              hCurrent = hMin;
              // Widget can not grow vertically unless widget have grow policy.
              if (!checkGrow)
                {
                  h += hCurrent + spacing();
                  continue;
                }
            }
          else if (hMax > 0 && hMax < hCurrent)
            {
              // Widget can not grow since its maximum height is reached.
              hCurrent = hMax;
              // Widget can not shrink vertically unless it has shrink policy.
              if (!checkShrink)
                {

                  h += hCurrent + spacing();
                  continue;
                }
            }

          // check for vertical and horizontal resizing
          if (width == pref.width())
            {
              // Width of layout is same as preferred width so there is no need to progress further.
              h += hCurrent + spacing();
              continue;
            }
          // Widget can grow vertically and shrink horizontally
          else if (checkGrow && width < pref.width()
              && currentWidget->hConstraint() & ShrinkPolicy)
            {
              int h4w = currentWidget->heightForWidth(width);
              if (h4w)
                hCurrent = h4w;
            }
          // Widget can shrink vertically and grow horizontally
          else if (checkShrink && width > pref.width()
              && currentWidget->hConstraint() & GrowPolicy)
            {
              int h4w = currentWidget->heightForWidth(width);
              if (h4w)
                hCurrent = h4w;
            }

          // Integrate height here
          h += hCurrent + spacing();
        }
    }
  if (h)
    return h - spacing();
  return -1;
}

Size
VBoxLayout::preferredSize() const
{
  int w = 0; // max width.

  // find max width. and create an ActiveWidgetList
  ActiveWidgetList wList;
  for (WidgetList::const_iterator it = _children.begin(); it != _children.end(); ++it)
    {
      if (((Widget*) *it)->visible() && ((Widget*) *it)->vConstraint()
          != IgnoredConstraint)
        {
          ActiveWidget aw;
          aw.widget = ((Widget*) *it);
          aw.size = aw.widget->preferredSize();

          // check minimum width constraint
          if (aw.size.width() < aw.widget->minWidth())
            aw.size.setWidth(aw.widget->minWidth());
          else if (aw.widget->maxWidth() > 0 && aw.size.width()
              > aw.widget->maxWidth())
            aw.size.setWidth(aw.widget->maxWidth());

          // Is maximum width?
          if (aw.size.width() > w)
            w = aw.size.width();

          wList.push_back(aw);
        }
    }

  int h = 0; // total height.
  int wCurrent = 0; // width for current widget
  int hCurrent = 0; // height for current widget
  int hMax = 0; // current widget's maximum height.
  int hMin = 0; // current widget's minimum height.
  bool checkGrow; // set to true, if vertical growing is supported for widget.
  bool checkShrink; // set to true, if vertical shrinking is supported for widget.

  // Calculate total height for active layout contents.
  for (ActiveWidgetListIterator it = wList.begin(); it != wList.end(); ++it)
    {
      Widget* currentWidget = ((ActiveWidget) *it).widget;
      hMin = currentWidget->minHeight();
      hMax = currentWidget->maxHeight();
      hCurrent = ((ActiveWidget) *it).size.height();
      wCurrent = ((ActiveWidget) *it).size.width();
      checkGrow = false;
      checkShrink = false;

      if (currentWidget->vConstraint() & GrowPolicy)
        checkGrow = true;
      else
        checkGrow = false;

      if (currentWidget->vConstraint() & ShrinkPolicy)
        checkShrink = true;
      else
        checkShrink = false;

      // check for minimum and maximum height.
      if (hMin > hCurrent)
        {
          // Widget can not shrink since its minimum height is reached.
          hCurrent = hMin;
          // Widget can not grow vertically unless widget have grow policy.
          if (!checkGrow)
            {
              h += hCurrent + spacing();
              continue;
            }
        }
      else if (hMax > 0 && hMax < hCurrent)
        {
          // Widget can not grow since its maximum height is reached.
          hCurrent = hMax;
          // Widget can not shrink vertically unless it has shrink policy.
          if (!checkShrink)
            {
              h += hCurrent + spacing();
              continue;
            }
        }

      // check for vertical and horizontal resizing
      if (w == wCurrent)
        {
          // Width of layout is same as preferred width so there is no need to progress further.
          h += hCurrent + spacing();
          continue;
        }
      else if (checkGrow && w < wCurrent && currentWidget->hConstraint()
          & ShrinkPolicy)
        {
          // Widget can grow vertically and shrink horizontally
          int h4w = currentWidget->heightForWidth(w);
          if (h4w)
            hCurrent = h4w;
        }
      else if (checkShrink && w > wCurrent && currentWidget->hConstraint()
          & GrowPolicy)
        {
          // Widget can shrink vertically and grow horizontally
          int h4w = currentWidget->heightForWidth(w);
          if (h4w)
            hCurrent = h4w;
        }

      // Integrate height here
      h += hCurrent + spacing();
    }
  if (h)
    h -= spacing();
  return Size(w, h);
}

void
VBoxLayout::updateActiveWidgetList()
{
  _numExpanding = 0;
  _activeWidgetList.clear();
  Widget* widget;
  bool wUpdated;
  for (WidgetListIterator it = _children.begin(); it != _children.end(); ++it)
    {
      widget = (*it);
      if ((*it)->visible() && (*it)->vConstraint() != IgnoredConstraint)
        {
          ActiveWidget aw;
          aw.widget = (*it);
          aw.size = (*it)->preferredSize();

          wUpdated = true;
          // Update width using layout's width if widget permits it.
          if (aw.size.width() > width() && (*it)->hConstraint() & ShrinkPolicy)
            aw.size.setWidth(width());
          else if (aw.size.width() < width() && (*it)->hConstraint()
              & GrowPolicy)
            aw.size.setWidth(width());
          else
            wUpdated = false;

          // Update height using new Width if the widget permits it.
          if (wUpdated)
            {
              int h4w = (*it)->heightForWidth(aw.size.width());
              if (h4w > 0)
                {
                  if (h4w > aw.size.height() && (*it)->vConstraint()
                      & GrowPolicy)
                    aw.size.setHeight(h4w);
                  else if (h4w < aw.size.height() && (*it)->vConstraint()
                      & ShrinkPolicy)
                    aw.size.setHeight(h4w);
                }
            }

          if (widget->vConstraint() & ExpandPolicy)
            ++_numExpanding;

          _activeWidgetList.push_back(aw);
        }
    }
}

void
VBoxLayout::tile()
{
  if (_layoutModified)
    {
      updateActiveWidgetList();
      if (_activeWidgetList.size() > 0)
        {
          ActiveWidgetList w = _activeWidgetList;
          int availableSpace = height() - ((w.size() - 1) * spacing());
          int average = availableSpace / w.size();
          ILOG_DEBUG("Layout(w,h) = (%d, %d)", width(), height());
          ILOG_DEBUG("Count: %d\t availableSpace: %d\t average: %d", w.size(), availableSpace, average);

          //***********************************************************
          // FixedConstraint
          // Remove only fixed from temporary list (ignore min.size and max.size), decrement available size and
          // recalculate average if list is not empty.
          //***********************************************************
          int spaceUsed = 0;
          ActiveWidgetListIterator it = w.begin();
          while (it != w.end())
            {
              if (((ActiveWidget) *it).widget->vConstraint() == FixedConstraint
                  && ((ActiveWidget) *it).widget->minHeight() < 0
                  && ((ActiveWidget) *it).widget->maxHeight() < 0)
                {
                  spaceUsed += ((ActiveWidget) *it).size.height();
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
          //                       MaximumSize
          //***********************************************************
          spaceUsed = 0;
          it = w.begin();
          while (it != w.end())
            {
              if (average > ((ActiveWidget) *it).widget->maxHeight()
                  && ((ActiveWidget) *it).widget->maxHeight() > 0)
                {
                  spaceUsed += ((ActiveWidget) *it).widget->maxHeight();
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
          //                      MinimumSize
          //***********************************************************
          spaceUsed = 0;
          it = w.begin();
          while (it != w.end())
            {
              if (average < ((ActiveWidget) *it).widget->minHeight())
                {
                  spaceUsed += ((ActiveWidget) *it).widget->minHeight();
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
              ILOG_DEBUG("Count@minSize: %d\t availableSpace: %d\t average: %d", w.size(), availableSpace, average);
            }

          //***********************************************************
          //                     ShrinkPolicy
          //***********************************************************
          spaceUsed = 0;
          it = w.begin();
          while (it != w.end())
            {
              if (average < ((ActiveWidget) *it).size.height()
                  && !(((ActiveWidget) *it).widget->vConstraint()
                      & ShrinkPolicy))
                {
                  spaceUsed += ((ActiveWidget) *it).size.height();
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
              ILOG_DEBUG("Count@ShrinkPolicy: %d\t availableSpace: %d\t average: %d", w.size(), availableSpace, average);
            }

          //***********************************************************
          //                     GrowPolicy
          //***********************************************************
          spaceUsed = 0;
          it = w.begin();
          while (it != w.end())
            {
              if (average > ((ActiveWidget) *it).size.height()
                  && !(((ActiveWidget) *it).widget->vConstraint() & GrowPolicy))
                {
                  spaceUsed += ((ActiveWidget) *it).widget->height();
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
              ILOG_DEBUG("Count@GrowPolicy: %d\t availableSpace: %d\t average: %d", w.size(), availableSpace, average);
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
                  if (!(((ActiveWidget) *it).widget->vConstraint()
                      & ExpandPolicy))
                    {
                      if (((ActiveWidget) *it).widget->minHeight() > 0
                          && average > ((ActiveWidget) *it).widget->minHeight())
                        {
                          expandSpace += average
                              - ((ActiveWidget) *it).widget->minHeight();
                          ((ActiveWidget) *it).size.setHeight(
                              ((ActiveWidget) *it).widget->minHeight());
                        }
                      else if (average > ((ActiveWidget) *it).size.height())
                        expandSpace += average
                            - ((ActiveWidget) *it).size.height();
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

          int currentY = 0;
          Widget* currentWidget;
          for (ActiveWidgetListIterator it = _activeWidgetList.begin(), end =
              _activeWidgetList.end(); it != end; ++it)
            {

              currentWidget = ((ActiveWidget) *it).widget;
              // Set height
              if (_numExpanding)
                {
                  if (currentWidget->vConstraint() & ExpandPolicy)
                    {
                      if (useArtifact)
                        {
                          currentWidget->setHeight(average + expandAverage
                              + artifact);
                          useArtifact = false;
                        }
                      else
                        currentWidget->setHeight(average + expandAverage);
                    }
                  else if (currentWidget->vConstraint() & ShrinkPolicy
                      && average < ((ActiveWidget) *it).size.height())
                    currentWidget->setHeight(average);
                  else
                    currentWidget->setHeight(((ActiveWidget) *it).size.height());
                }
              else
                {
                  if (currentWidget->minHeight() > 0
                      || currentWidget->maxHeight() > 0)
                    currentWidget->setHeight(average);
                  else if ((currentWidget->vConstraint() & ShrinkPolicy)
                      && ((ActiveWidget) *it).size.height() > average)
                    currentWidget->setHeight(average);
                  else if ((currentWidget->vConstraint() & GrowPolicy)
                      && ((ActiveWidget) *it).size.height() < average)
                    {
                      if (useArtifact)
                        {
                          currentWidget->setHeight(average + artifact);
                          useArtifact = false;
                        }
                      else
                        currentWidget->setHeight(average);
                    }
                  else
                    currentWidget->setHeight(((ActiveWidget) *it).size.height());
                }
              currentWidget->setWidth(((ActiveWidget) *it).size.width());
              currentWidget->moveTo(0, currentY);
              currentY += currentWidget->height() + spacing();

              //LOG_DEBUG("w: %d, h: %d", currentWidget->width(), currentWidget->height());
            }
          //LOG_DEBUG("Current_Y: %d", currentY);
          ILOG_DEBUG("Tiling finished!");
        }
      _layoutModified = false;
    }
}
