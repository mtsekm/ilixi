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

#ifndef ILIXI_LINEEDIT_H_
#define ILIXI_LINEEDIT_H_

#include "ui/TextLayout.h"
#include "core/Thread.h"

namespace ilixi
{

  class LineEdit : public TextLayout
  {
  public:
    LineEdit(std::string text, Widget* parent = 0);

    virtual
    ~LineEdit();

    virtual Size
    preferredSize() const;

    bool
    readOnly() const;

    int
    maxLength() const;

    virtual void
    clear();

    void
    setReadOnly(bool readOnly);

    virtual void
    setText(const std::string &text);

    sigc::signal<void, std::string> sigTextChanged;

  protected:
    virtual void
    pointerButtonDownEvent(const PointerEvent& mouseEvent);

    /*!
     * Paints line edit widget on its surface using current designer.
     * @param rect Clipped rectangle to paint.
     */
    virtual void
    compose(const Rectangle& rect);

    virtual void
    updateTextLayoutGeometry();

  private:
    bool _readOnly;
    unsigned int _maxLength;

    virtual Font*
    defaultFont() const;
  };

}

#endif /* ILIXI_LINEEDIT_H_ */
