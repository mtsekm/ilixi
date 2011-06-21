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

#include "ui/ComboBox.h"
#include "graphics/Painter.h"
#include "ui/Dialog.h"
#include "ui/VBoxLayout.h"
#include "ui/RadioButton.h"
#include "ui/ScrollArea.h"
#include <sigc++/bind.h>
#include <algorithm>
#include "core/Logger.h"

using namespace ilixi;

ComboBox::ComboBox(const std::string& title, Widget* parent) :
  TextLayout("", parent), _alternateRows(true), _selectedIndex(0)
{
  setInputMethod(KeyAndPointerInput);
  setConstraints(MinimumConstraint, FixedConstraint);

  _dialog = new Dialog(title, Dialog::CancelButtonOption);
  _dialog->setLayout(new VBoxLayout());

  _scrollArea = new ScrollArea();
  _scrollArea->setLayout(new VBoxLayout());
  _dialog->addWidget(_scrollArea);

  _dialog->sigFinished.connect(sigc::mem_fun(this, &ComboBox::updateSelected));
}

ComboBox::ComboBox(const std::string& title,
    const std::vector<std::string>& items, Widget* parent) :
  TextLayout("", parent), _alternateRows(true), _selectedIndex(0)
{
  setInputMethod(KeyAndPointerInput);
  setConstraints(MinimumConstraint, FixedConstraint);
  _dialog = new Dialog(title, Dialog::CancelButtonOption);
  _dialog->sigFinished.connect(sigc::mem_fun(this, &ComboBox::updateSelected));
  setItems(items);
  setSelected(_selectedIndex);
}

ComboBox::~ComboBox()
{
  delete _dialog;
}

Size
ComboBox::preferredSize() const
{
  Size s = textExtents();
  return Size(
      s.width() + 2 * std::max(designer()->hint(BorderWidth),
          designer()->hint(ComboBoxRadius)) + designer()->hint(
          ComboBoxButtonWidth), s.height() + 2 * designer()->hint(BorderWidth));
}

void
ComboBox::clear()
{
  _items.clear();
}

unsigned int
ComboBox::selectedIndex() const
{
  return _selectedIndex;
}

std::string
ComboBox::selectedItem() const
{
  return _items.at(_selectedIndex)->text();
}

std::string
ComboBox::item(unsigned int index) const
{
  if (index < _items.size())
    return _items.at(index)->text();
  return "";
}

void
ComboBox::addItem(const std::string& item)
{
  _selectedIndex = _items.size();
  setText(item);
  RadioButton* rb = new RadioButton(item);
  _items.push_back(rb);
  _scrollArea->addWidget(rb);
  rb->toggleState();
  rb->sigClicked.connect(
      sigc::bind<int>(sigc::mem_fun(_dialog, &Dialog::finish), _selectedIndex));
}

void
ComboBox::setItems(const std::vector<std::string>& items)
{
  _dialog->setLayout(new VBoxLayout());
  for (unsigned int i = 0; i < items.size(); ++i)
    {
      _items.push_back(new RadioButton(items[i]));
      _scrollArea->addWidget(_items.back());
    }
}

void
ComboBox::setSelected(unsigned int index)
{
  if (index < _items.size())
    {
      _selectedIndex = index;
      _items[index]->toggleState();
      setText(_items.at(_selectedIndex)->text());
    }
}

void
ComboBox::keyUpEvent(const KeyEvent& keyEvent)
{
  if (keyEvent.keySymbol == DIKS_CURSOR_UP)
    setSelected(_selectedIndex - 1);
  else if (keyEvent.keySymbol == DIKS_CURSOR_DOWN)
    setSelected(_selectedIndex + 1);
}

void
ComboBox::pointerButtonUpEvent(const PointerEvent& mouseEvent)
{
  if (_items.size())
    {
      update();
      _dialog->execute();
    }
}

void
ComboBox::pointerWheelEvent(const PointerEvent& event)
{
  if (event.wheelStep < 0)
    setSelected(_selectedIndex + 1);
  else
    setSelected(_selectedIndex - 1);
}

void
ComboBox::compose(const Rectangle& rect)
{
  Painter p(this);
  p.begin(rect);
  designer()->drawComboBox(&p, this);
  p.end();
}

void
ComboBox::updateSelected(int index)
{
  if (index != _selectedIndex)
    {
      _selectedIndex = index;
      setText(item(_selectedIndex));
    }
}

void
ComboBox::updateTextLayoutGeometry()
{
  int border = std::max(designer()->hint(BorderWidth),
      designer()->hint(ComboBoxRadius));
  int x = border;
  int y = designer()->hint(BorderWidth);

  setTextGeometry(x, y, width() - x - designer()->hint(ComboBoxButtonWidth),
      height() - 2 * designer()->hint(BorderWidth));
}

Font*
ComboBox::defaultFont() const
{
  return designer()->defaultFont(InputFont);
}
