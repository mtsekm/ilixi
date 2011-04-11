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

#ifndef ILIXI_COMBOBOX_H_
#define ILIXI_COMBOBOX_H_

#include "ui/TextLayout.h"
#include <vector>

namespace ilixi
{
  class Dialog;
  class RadioButton;
  class ComboBox : public TextLayout
  {
  public:
    ComboBox(const std::string& title, Widget* parent = 0);

    ComboBox(const std::string& title, const std::vector<std::string>& items,
        Widget* parent = 0);

    virtual
    ~ComboBox();

    virtual Size
    preferredSize() const;

    void
    clear();

    unsigned int
    selectedIndex() const;

    std::string
    selectedItem() const;

    std::string
    item(unsigned int index) const;

    void
    addItem(const std::string& item);

    void
    removeItem(const std::string& item);

    void
    setItems(const std::vector<std::string>& items);

    void
    setSelected(unsigned int index);

  protected:
    bool _alternateRows;
    unsigned int _selectedIndex;

    std::vector<RadioButton*> _items;

    virtual void
    keyUpEvent(const KeyEvent& keyEvent);

    virtual void
    pointerButtonUpEvent(const PointerEvent& mouseEvent);

    virtual void
    pointerWheelEvent(const PointerEvent& event);

    virtual void
    compose(const Rectangle& rect);

  private:
    Dialog* _dialog;

    void
    updateSelected(int index);

    virtual void
    updateTextLayoutGeometry();

    virtual Font*
    defaultFont() const;
  };
}

#endif /* ILIXI_COMBOBOX_H_ */
