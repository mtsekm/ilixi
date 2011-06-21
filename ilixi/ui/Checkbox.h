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

#ifndef ILIXI_CHECKBOX_H_
#define ILIXI_CHECKBOX_H_

#include "ui/Button.h"

namespace ilixi
{
  //! A check box button with a text label and icon.
  class Checkbox : public Button
  {
  public:
    /*!
     *
     */
    enum CheckState
    {
      Checked, //!< Checked
      Partial, //!< Partial
      Unchecked
    //!< Unchecked
    };

    /*!
     * Constructor.
     * @param text
     * @param parent
     */
    Checkbox(std::string text, Widget* parent = 0);

    /*!
     * Destructor.
     */
    virtual
    ~Checkbox();

    int
    heightForWidth(int width) const;

    Size
    preferredSize() const;

    /*!
     * Returns checkbox's check state.
     */
    CheckState
    checkState() const;

    /*!
     * Changes checkbox's CheckState. CheckStateChanged signal is emitted.
     */
    void
    toggleState();

    void
    setTriState(bool triState);

    /*!
     * This signal is emitted when button is released.
     * \sa sigClicked
     */
    sigc::signal<void, CheckState> sigCheckStateChanged;

  private:
    //! This property holds whether checkbox has three states. (Checked, Unchecked, Partial)
    bool _tristate;
    //! This property holds checkbox's current state.
    CheckState _checkboxState;

    /*!
     * Paints Checkbox on its surface using current designer.
     * @param rect Clipped rectangle to paint.
     */
    void
    compose(const Rectangle& rect);

    virtual void
    updateTextLayoutGeometry();
  };
}

#endif /* ILIXI_CHECKBOX_H_ */
