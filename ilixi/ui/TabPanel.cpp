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

#include "ui/TabPanel.h"
#include "graphics/Painter.h"
#include "core/Logger.h"
#include <sigc++/sigc++.h>

using namespace ilixi;

TabPanelButton::TabPanelButton(std::string label, Widget* parent) :
  Button(label, parent)
{
  setInputMethod(PointerInputOnly);
  setCheckable(true);
}

TabPanelButton::~TabPanelButton()
{
}

Size
TabPanelButton::preferredSize() const
{
  if (text().empty() && !hasIcon())
    return designer()->sizeHint(ButtonSizeHint);

  int w = 2 * (std::max(designer()->hint(BorderWidth), designer()->hint(
      FrameBorderRadius)) + designer()->hint(ButtonOffset));

  if (hasIcon())
    w += icon()->width() + designer()->hint(ButtonOffset);

  if (!text().empty())
    w += textExtents().width();

  return Size(w, designer()->hint(TabPanelButtonHeight));
}

void
TabPanelButton::toggleState()
{
}

void
TabPanelButton::compose(const Rectangle& rect)
{
  Painter painter(this);
  painter.begin(rect);
  designer()->drawTabPanelButton(&painter, this);
  painter.end();
}

void
TabPanelButton::updateTextLayoutGeometry()
{
  int border = std::max(designer()->hint(BorderWidth), designer()->hint(
      FrameBorderRadius));
  int x = border + designer()->hint(ButtonOffset);
  int y = designer()->hint(BorderWidth) + 3;

  if (hasIcon())
    {
      _iconPosition.moveTo(x, y);
      x += icon()->width() + designer()->hint(ButtonOffset);
    }

  setTextGeometry(x, y, width(), 0);
}

//********************************************************************************

TabPanel::TabPanel(Widget* parent) :
  BorderBase(parent), _currentIndex(-1), _margin(5)
{
  setBorderStyle(StyledBorder);
  setBorderWidth(1);
  setConstraints(NoConstraint, MinimumConstraint);
  setInputMethod(PointerInputOnly);
  sigGeometryUpdated.connect(sigc::mem_fun(this,
      &TabPanel::updateChildrenFrameGeometry));
}

TabPanel::~TabPanel()
{
}

int
TabPanel::heightForWidth(int width) const
{
  int used = 2 * (borderHorizontalOffset() + _margin);
  int h = -1;
  int h4w;
  for (unsigned int i = 0; i < _pages.size(); i++)
    {
      h4w = _pages.at(i).widget->heightForWidth(width - used);
      if (h4w > h)
        h = h4w;
    }

  if (h > 0)
    h += designer()->hint(TabPanelButtonHeight) + 2 * _margin;
  return h;
}

Size
TabPanel::preferredSize() const
{
  int w = 0, h = 0;

  // calculate max. size of pages.
  for (unsigned int i = 0; i < _pages.size(); i++)
    {
      Size wS = _pages[i].widget->preferredSize();
      if (wS.width() > w)
        w = wS.width();
      if (wS.height() > h)
        h = wS.height();
    }
  return Size(w + 2 * (borderHorizontalOffset() + _margin), h
      + designer()->hint(TabPanelButtonHeight) + 2 * (borderWidth() + _margin));
}

int
TabPanel::count() const
{
  return _pages.size();
}

void
TabPanel::clear()
{
  _pages.clear();
}

int
TabPanel::currentIndex() const
{
  return _currentIndex;
}

Widget*
TabPanel::currentWidget() const
{
  return _pages.at(_currentIndex).widget;
}

int
TabPanel::indexOf(Widget* widget) const
{
  for (unsigned int i = 0; i < _pages.size(); i++)
    {
      if (_pages.at(i).widget == widget)
        return i;
    }
  return -1;
}

int
TabPanel::margin() const
{
  return _margin;
}

Image*
TabPanel::pageIcon(int index) const
{
  return _pages.at(index).button->icon();
}

bool
TabPanel::pageEnabled(int index) const
{
  return _pages.at(index).widget->enabled();
}

std::string
TabPanel::pageLabel(int index) const
{
  return _pages.at(index).button->text();
}

void
TabPanel::addPage(Widget* widget, std::string label, std::string iconPath)
{
  if (!widget)
    return;

  if (_currentIndex != -1)
    {
      _pages.at(_currentIndex).widget->setVisible(false);
      _pages.at(_currentIndex).button->setChecked(false);
    }

  _currentIndex++;
  TabPage page;
  page.widget = widget;
  page.button = new TabPanelButton(label, this);
  page.button->setChecked(true);
  page.button->sigClicked.connect(sigc::bind<int>(sigc::mem_fun(this,
      &TabPanel::setCurrentPage), _currentIndex));
  if (!iconPath.empty())
    page.button->setIcon(iconPath, Size(designer()->hint(TabPanelButtonHeight)
        - 6, designer()->hint(TabPanelButtonHeight) - 6));

  _pages.push_back(page);

  addChild(page.button);
  addChild(widget);
  setSurfaceGeometryModified();
}

void
TabPanel::insertPage(int index, Widget* widget, std::string label,
    std::string iconPath)
{
  //  widget->setParent(this);
  //  TabPage page;
  //  page.widget = widget;
  //  page.button = new TabPanelButton(label, this);
  //  if (!iconPath.empty())
  //    page.button->setIcon(iconPath, Size(designer()->hint(TabPanelButtonHeight)
  //        - 6, designer()->hint(TabPanelButtonHeight) - 6));
  //
  //  if (_currentIndex != -1)
  //    _pages.at(_currentIndex).widget->setVisible(false);
  //
  //  if (index < _pages.size())
  //    {
  //      _currentIndex = index;
  //      PageList::iterator it = _pages.begin() + index;
  //      _pages.insert(it, page);
  //    }
  //  else
  //    {
  //      _pages.push_back(page);
  //      _currentIndex = _pages.size() - 1;
  //    }
  //
  //  addChild(page.button);
  //  setSurfaceGeometryChanged();
  //  update();
}

void
TabPanel::removePage(int index)
{
  if (index < _pages.size())
    {
      PageList::iterator it = _pages.begin() + index;
      removeChild(it->button);
      _pages.erase(it);
      setSurfaceGeometryModified();
      update();
    }
}

void
TabPanel::setPageIcon(int index, std::string iconPath)
{
  _pages.at(index).button->setIcon(iconPath, Size(designer()->hint(
      TabPanelButtonHeight) - 6, designer()->hint(TabPanelButtonHeight) - 6));
}

void
TabPanel::setCurrentPage(int index)
{
  if (index == _currentIndex)
    return;

  if (index >= 0 && index < _pages.size())
    {
      // hide previous page.
      if (_currentIndex != -1)
        {
          _pages.at(_currentIndex).widget->setVisible(false);
          _pages.at(_currentIndex).button->setChecked(false);
        }

      // show current page.
      _currentIndex = index;
      _pages.at(_currentIndex).widget->setVisible(true);
      _pages.at(_currentIndex).button->setChecked(true);
      update();
    }
}

void
TabPanel::setMargin(int margin)
{
  _margin = margin;
}

void
TabPanel::setPageLabel(int index, std::string label)
{
  _pages.at(index).button->setText(label);
}

void
TabPanel::setPageEnabled(int index, bool enabled)
{
  if (enabled)
    _pages.at(index).widget->setEnabled();
  else
    _pages.at(index).widget->setDisabled();
}

void
TabPanel::doLayout()
{
}

void
TabPanel::compose(const Rectangle& rect)
{
  Painter painter(this);
  painter.begin(rect);
  designer()->drawTabPanel(&painter, this);
  painter.end();
}

void
TabPanel::updateChildrenFrameGeometry()
{
  int buttonX = 0;
  int buttonW = 0;
  int buttonH = designer()->hint(TabPanelButtonHeight);
  int pageX = borderHorizontalOffset() + _margin;
  int pageY = borderWidth() + buttonH + _margin;
  int pageWidth = width() - 2 * (borderHorizontalOffset() + _margin);
  int pageHeight = height() - buttonH - 2 * (borderWidth() + _margin);

  for (unsigned int i = 0; i < _pages.size(); i++)
    {
      // set button
      buttonW = _pages[i].button->preferredSize().width();
      _pages[i].button->setGeometry(buttonX, 0, buttonW, buttonH);
      buttonX += buttonW;

      // set page
      _pages[i].widget->moveTo(pageX, pageY);

      _pages[i].widgetSize = _pages[i].widget->preferredSize();

      if (_pages[i].widgetSize.width() < pageWidth
          && _pages[i].widget->hConstraint() & GrowPolicy)
        _pages[i].widget->setWidth(pageWidth);
      else if (_pages[i].widgetSize.width() > pageWidth
          && _pages[i].widget->hConstraint() & ShrinkPolicy)
        _pages[i].widget->setWidth(pageWidth);
      else
        _pages[i].widget->setWidth(_pages[i].widgetSize.width());

      if (_pages[i].widgetSize.height() < pageHeight
          && _pages[i].widget->vConstraint() & GrowPolicy)
        _pages[i].widget->setHeight(pageHeight);
      else if (_pages[i].widgetSize.height() > pageHeight
          && _pages[i].widget->vConstraint() & ShrinkPolicy)
        _pages[i].widget->setHeight(pageHeight);
      else
        _pages[i].widget->setHeight(_pages[i].widgetSize.height());
    }
}

void
TabPanel::paintChildren(const Rectangle& rect)
{
  for (unsigned int i = 0; i < _pages.size(); i++)
    {
      _pages[i].button->paint(rect);
      if (i == _currentIndex)
        _pages[i].widget->paint(rect);
    }
}
