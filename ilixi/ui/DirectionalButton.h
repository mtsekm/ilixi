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

#ifndef ILIXI_DIRBUTTON_H_
#define ILIXI_DIRBUTTON_H_

#include "ui/Widget.h"

namespace ilixi
{

  class DirectionalButton : public Widget
  {
  public:
    enum DirectionalButtonType
    {
      ArrowButton, PlusMinusButton
    };

    DirectionalButton(DirectionalButtonType type, Direction direction,
        Widget* parent = 0);

    virtual
    ~DirectionalButton();

    Direction
    direction() const;

    DirectionalButtonType
    type() const;

    void
    setDirection(Direction direction);

    void
    setType(DirectionalButtonType type);

    sigc::signal<void> sigClicked;

    sigc::signal<void> sigPressed;

    sigc::signal<void> sigReleased;

  protected:
    virtual void
    pointerButtonDownEvent(const PointerEvent& mouseEvent);

    virtual void
    pointerButtonUpEvent(const PointerEvent& mouseEvent);

    virtual void
    enterEvent(const PointerEvent& mouseEvent);

    virtual void
    leaveEvent(const PointerEvent& mouseEvent);

  private:
    DirectionalButtonType _type;
    Direction _direction;
    bool _doClick;

    void
    compose(const Rectangle& rect);

  };

}

#endif /* ILIXI_DIRBUTTON_H_ */
