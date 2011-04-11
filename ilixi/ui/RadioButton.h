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

#ifndef ILIXI_RADIOBUTTON_H_
#define ILIXI_RADIOBUTTON_H_

#include "ui/Button.h"
#include "types/RadioGroup.h"

namespace ilixi
{
  //! A radio button with text label and icon.
  class RadioButton : public Button
  {
  public:
    /*!
     * Constructor
     * @param text
     * @param parent
     */
    RadioButton(std::string text, Widget* parent = 0);

    /*!
     * Destructor
     */
    ~RadioButton();

    int
    heightForWidth(int width) const;

    Size
    preferredSize() const;

    /*!
     * Changes radio button's checked state.
     */
    virtual void
    toggleState();

    void
    setGroup(RadioGroup* group);

  private:
    RadioGroup* _group;

    void
    informGroup(bool checked);

    /*!
     * Paints RadioButton on its surface using current designer.
     * @param rect Clipped rectangle to paint.
     */
    void
    compose(const Rectangle& rect);

    virtual void
    updateTextLayoutGeometry();
  };
}

#endif /* ILIXI_RADIOBUTTON_H_ */
