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

#ifndef ILIXI_PUSHBUTTON_H_
#define ILIXI_PUSHBUTTON_H_

#include "ui/Button.h"

namespace ilixi
{
  //! A push button with text label and icon.
  class PushButton : public Button
  {
  public:
    /*!
     * Constructor
     * @param text
     * @param parent
     */
    PushButton(std::string text, Widget* parent = 0);

    /*!
     * Destructor
     */
    virtual
    ~PushButton();

    virtual Size
    preferredSize() const;

  private:
    /*!
     * Paints PushButton on its surface using current designer.
     * @param rect Clipped rectangle to paint.
     */
    void
    compose(const Rectangle& rect);

    virtual void
    updateTextLayoutGeometry();
  };
}

#endif /* ILIXI_PUSHBUTTON_H_ */
