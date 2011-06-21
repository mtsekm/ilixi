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

#ifndef ILIXI_LABEL_H_
#define ILIXI_LABEL_H_

#include "ui/BorderBase.h"
#include "ui/TextLayout.h"

namespace ilixi
{
  //! Provides a simple text label with optional border
  class Label : public BorderBase, public TextLayout
  {
  public:
    /*!
     * Constructor.
     *
     * @param text Label's text.
     * @param parent Label's parent.
     */
    Label(std::string text, Widget* parent = 0);

    /*!
     * Destructor.
     */
    virtual
    ~Label();

    virtual int
    heightForWidth(int width) const;

    virtual Size
    preferredSize() const;

    /*!
     * Returns the margin around text.
     */
    int
    margin() const;

    VerticalAlignment
    verticalAlignment() const;

    /*!
     * Sets the margin around text.
     *
     * @param margin Margin value in pixels.
     */
    void
    setMargin(int margin);

    void
    setVerticalAlignment(VerticalAlignment vAlign);

  protected:

    //! This property holds the margin around text.
    int _margin;
    //! This property hold the vertical alignment of text inside label widget.
    VerticalAlignment _vertical;

    virtual void
    compose(const Rectangle& rect);

    virtual void
    updateTextLayoutGeometry();
  };
}

#endif /* ILIXI_LABEL_H_ */
