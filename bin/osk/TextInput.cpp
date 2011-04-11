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

#include "TextInput.h"
#include "graphics/Painter.h"

#include "core/Logger.h"

using namespace ilixi;

TextInputThread::TextInputThread(TextInput* parent) :
  Thread(), _parent(parent)
{
}

TextInputThread::~TextInputThread()
{
}

int
TextInputThread::run()
{
  bool on = false;
  _parent->updateCursorPosition();
  while (1)
    {
      pthread_testcancel();
      usleep(100000);
      pthread_testcancel();
      _parent->drawCursor(on = !on);
      usleep(400000);
    }
  return 1;
}

//**********************************************************************

TextInput::TextInput(std::string text, Widget* parent) :
  LineEdit(text, parent), _selecting(false), _cursorIndex(_text.length()),
      _selectedIndex(0), _thread(this)
{
  setInputMethod(KeyAndPointerInputTracking);
}

TextInput::~TextInput()
{
  _thread.cancel();
}

int
TextInput::cursorIndex() const
{
  return _cursorIndex;
}

void
TextInput::clear()
{
  _text = "";
  pango_layout_set_text(_layout, _text.c_str(), _text.length());
  _selection.setSize(0, 0);
  _cursorIndex = 0;
  update();
}

void
TextInput::setText(const std::string &text)
{
  if (text != _text)
    {
      _thread.cancel();
      _text = text;
      pango_layout_set_text(_layout, _text.c_str(), _text.length());
      _cursorIndex = _text.length();
      _selection.setSize(0, 0);
      updateCursorPosition();
      update();
      sigTextChanged(_text);
    }
}

void
TextInput::pointerButtonDownEvent(const PointerEvent& mouseEvent)
{
  // get cursor index
  Point p = mapToSurface(Point(mouseEvent.x, mouseEvent.y));
  p -= _layoutPosition;
  int index = 0;
  bool rc = pango_layout_xy_to_index(_layout, p.x() * PANGO_SCALE, p.y()
      * PANGO_SCALE, &index, NULL);

  // fix last character
  if (index == _text.length() - 1 && !rc)
    ++index;

  if (index != _cursorIndex)
    {
      sigCursorMoved(_cursorIndex, index);
      _cursorIndex = index;

      updateCursorPosition();
      _selecting = true;
      _selectedIndex = _cursorIndex;
      _selection.moveTo(_cursor.x(), _cursor.y());
      _selection.setSize(0, 0);
      update();
    }
}

void
TextInput::pointerButtonUpEvent(const PointerEvent& mouseEvent)
{
  _selecting = false;
}

void
TextInput::pointerMotionEvent(const PointerEvent& mouseEvent)
{
  if (_selecting)
    {
      // get cursor index
      int index = 0;
      Point p = mapToSurface(Point(mouseEvent.x, mouseEvent.y));
      p -= _layoutPosition;
      bool rc = pango_layout_xy_to_index(_layout, p.x() * PANGO_SCALE, p.y()
          * PANGO_SCALE, &index, NULL);

      // fix last character
      if (index == _text.length() - 1 && !rc)
        index++;

      if (index != _cursorIndex)
        {
          sigCursorMoved(_cursorIndex, index);
          _cursorIndex = index;
          updateCursorPosition();
          if (_selectedIndex != _cursorIndex)
            {
              _selection.setRight(_cursor.right());
              _selection.setBottom(_cursor.bottom());
              sigSelectionChanged();
            }
          else
            _selection.setSize(0, 0);
          update();
        }
    }
}

void
TextInput::keyUpEvent(const KeyEvent& keyEvent)
{
  if (readOnly())
    return;

  if (keyEvent.keySymbol >= DIKS_SPACE && keyEvent.keySymbol <= DIKS_TILDE)
    {
      if (maxLength() > 0 && _text.length() == maxLength())
        return;

      if (_selection.isNull())
        {
          _text.insert(_cursorIndex, 1, (char) keyEvent.keySymbol);
          sigCursorMoved(_cursorIndex, ++_cursorIndex);
        }
      else
        {
          int pos1 = std::min(_selectedIndex, _cursorIndex);
          int n1 = abs(_selectedIndex - _cursorIndex);
          _text.replace(pos1, n1, 1, (char) keyEvent.keySymbol);
          _selection.setSize(0, 0);
          sigCursorMoved(_cursorIndex, pos1 + 1);
          _cursorIndex = pos1 + 1;
        }

      pango_layout_set_text(_layout, _text.c_str(), _text.length());
      update();
      return;
    }

  switch (keyEvent.keySymbol)
    {
  case DIKS_CURSOR_LEFT:
  case DIKS_CURSOR_UP:
    if (_cursorIndex)
      {
        sigCursorMoved(_cursorIndex, --_cursorIndex);
        update();
      }
    break;

  case DIKS_CURSOR_RIGHT:
  case DIKS_CURSOR_DOWN:
    if (_cursorIndex < _text.length())
      {
        sigCursorMoved(_cursorIndex, ++_cursorIndex);
        update();
      }
    break;

  case DIKS_PAGE_UP:
  case DIKS_HOME:
    sigCursorMoved(_cursorIndex, 0);
    _cursorIndex = 0;
    update();
    break;

  case DIKS_PAGE_DOWN:
  case DIKS_END:
    sigCursorMoved(_cursorIndex, _text.length());
    _cursorIndex = _text.length();
    update();
    break;

  case DIKS_DELETE:
    if (_cursorIndex < _text.length())
      {
        _text.erase(_cursorIndex, 1);
        pango_layout_set_text(_layout, _text.c_str(), _text.length());
        update();
      }
    break;

  case DIKS_BACKSPACE:
    if (_selection.isNull())
      {
        if (_cursorIndex)
          {
            sigCursorMoved(_cursorIndex, --_cursorIndex);
            _text.erase(_cursorIndex, 1);
          }
      }
    else
      {
        int pos1 = std::min(_selectedIndex, _cursorIndex);
        int n1 = abs(_selectedIndex - _cursorIndex);
        _text.erase(pos1, n1);
        _selection.setSize(0, 0);
        sigSelectionChanged();
        sigCursorMoved(_cursorIndex, pos1);
        _cursorIndex = pos1;
      }
    pango_layout_set_text(_layout, _text.c_str(), _text.length());
    update();
    break;

  default:
    break;
    }
}

void
TextInput::focusOutEvent()
{
  _thread.cancel();
  _selecting = false;
}

void
TextInput::compose(const Rectangle& rect)
{
  _thread.cancel();
  Painter p(this);
  p.begin(rect);
  designer()->drawLineEdit(&p, this, _selection);
  p.end();
  if (!readOnly() && _state & FocusedState)
    _thread.start();
}

void
TextInput::updateTextLayoutGeometry()
{
  Size s = textExtents();
  _cursor.setY((height() - s.height()) / 2);
  _cursor.setSize(2, s.height());
  setTextGeometry(std::max(designer()->hint(TextInputFrameRadius),
      designer()->hint(BorderWidth)), _cursor.y(), width() - 2
      * designer()->hint(TextInputFrameRadius), height() - 2
      * designer()->hint(BorderWidth));
}

void
TextInput::updateCursorPosition()
{
  PangoRectangle weak;
  pango_layout_get_cursor_pos(_layout, _cursorIndex, NULL, &weak);
  _cursor.moveTo(weak.x / PANGO_SCALE + _layoutPosition.x(), weak.y
      + _layoutPosition.y());
  if (_cursor.x() > _layoutPosition.x() + textLayoutWidth())
    {
      _layoutPosition.setX(-_layoutPosition.x());
      _cursor.setX(_layoutPosition.x() + textLayoutWidth());
    }
}

void
TextInput::drawCursor(bool show)
{
  Painter p(this);
  p.begin(_cursor);
  designer()->drawInputCursor(&p, this, _cursor, show, _selection);
  flip(_cursor);
  p.end();
}
