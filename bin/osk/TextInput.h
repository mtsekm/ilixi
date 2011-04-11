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

#ifndef ILIXI_TEXTINPUT_H_
#define ILIXI_TEXTINPUT_H_

#include "ui/LineEdit.h"
#include "core/Thread.h"

namespace ilixi
{
  class TextInput;

  class TextInputThread : public Thread
  {
  public:
    TextInputThread(TextInput* parent);

    ~TextInputThread();

    int
    run();

  private:
    TextInput* _parent;
  };

  class TextInput : public LineEdit
  {
    friend class TextInputThread;
  public:
    TextInput(std::string text, Widget* parent = 0);

    virtual
    ~TextInput();

    int
    cursorIndex() const;

    virtual void
    clear();

    virtual void
    setText(const std::string &text);

    sigc::signal<void, int, int> sigCursorMoved;
    sigc::signal<void> sigSelectionChanged;

  protected:
    virtual void
    pointerButtonDownEvent(const PointerEvent& mouseEvent);

    virtual void
    pointerButtonUpEvent(const PointerEvent& mouseEvent);

    virtual void
    pointerMotionEvent(const PointerEvent& mouseEvent);

    virtual void
    keyUpEvent(const KeyEvent& keyEvent);

    virtual void
    focusOutEvent();

    /*!
     * Paints TextInput on its surface using current designer.
     * @param rect Clipped rectangle to paint.
     */
    void
    compose(const Rectangle& rect);

    virtual void
    updateTextLayoutGeometry();

  private:
    bool _selecting;
    int _cursorIndex;
    int _selectedIndex;

    TextInputThread _thread;

    Rectangle _cursor;
    Rectangle _selection;

    void
    updateCursorPosition();

    void
    drawCursor(bool show);
  };

}

#endif /* ILIXI_TEXTINPUT_H_ */
