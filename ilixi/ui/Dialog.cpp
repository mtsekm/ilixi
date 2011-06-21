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

#include "ui/Dialog.h"
#include "ui/ToolButton.h"
#include "graphics/Painter.h"
#include "types/FontMetrics.h"
#include <sigc++/bind.h>
#include "core/Logger.h"

using namespace ilixi;

Dialog::Dialog(std::string title, ButtonOption option, Widget* parent) :
  //Window(), Frame(parent),
      WindowWidget(), _buttonLayout(NULL), _result(-1)
{
  _surfaceDesc = WindowDescription;
  setMargin(5);
  setTitle(title);
  setUIManager(_windowUIManager);
  setButtonLayoutOption(option);
  sigGeometryUpdated.connect(
      sigc::mem_fun(this, &Dialog::updateButtonLayoutGeometry));
}

Dialog::~Dialog()
{
}

int
Dialog::heightForWidth(int width) const
{
  int usedHorizontalSpace = (_canvasTopLeft.x() + _margin.hSum()) - 2
      * borderHorizontalOffset();
  return _layout->heightForWidth(width - usedHorizontalSpace)
      + _canvasTopLeft.y() + _margin.vSum() + 2 * borderWidth()
      + _titleSize.height() + _buttonLayoutSize.height();
}

Size
Dialog::preferredSize() const
{
  Size layoutSize = _layout->preferredSize();

  int layoutWidth = layoutSize.width() + _canvasTopLeft.x() + _margin.hSum();
  int w = std::max(layoutWidth, _titleSize.width());
  w = std::max(w, _buttonLayoutSize.width());

  int hButtonLayout = _buttonLayoutSize.height() + 2 * spacing();

  return Size(
      w + 2 * borderHorizontalOffset(),
      layoutSize.height() + _canvasTopLeft.y() + _margin.vSum()
          + _titleSize.height() + hButtonLayout + 2 * borderWidth());
}

void
Dialog::accept()
{
  _result = 1;
  sigAccepted();
  sigFinished(_result);
  closeDialog();
}

void
Dialog::reject()
{
  _result = 0;
  sigRejected();
  sigFinished(_result);
  closeDialog();
}

void
Dialog::finish(int result)
{
  _result = result;
  sigFinished(_result);
  closeDialog();
}

int
Dialog::result() const
{
  return _result;
}

void
Dialog::execute()
{
  if (_window)
    _window->UngrabPointer(_window);
  ILOG_DEBUG("EXECUTE!");
  Size pref = preferredSize();
  int w = pref.width();
  int h = pref.height();
  if (w > (_layerConfig.width - 15))
    w = _layerConfig.width - 15;

  if (h > (_layerConfig.height - 15))
    h = _layerConfig.height - 15;

  int x = (_layerConfig.width - w) / 2.0;
  int y = (_layerConfig.height - h) / 2.0;

  setSize(w, h);

  initDFBWindow(x, y, width(), height(), true);

  setUIManager(_windowUIManager);
  paint(Rectangle(0, 0, w, h));

  _window->GrabPointer(_window);
  showWindow(Fade);
}

std::string
Dialog::title() const
{
  return _title;
}

Size
Dialog::buttonLayoutSize() const
{
  return _buttonLayoutSize;
}

Size
Dialog::titleSize() const
{
  return _titleSize;
}

void
Dialog::setButtonLayoutOption(ButtonOption option)
{
  if (_buttonLayout)
    removeChild(_buttonLayout);

  if (option == NoButtonOption)
    return;

  _buttonLayout = new HBoxLayout();
  _buttonLayout->addWidget(new Spacer());

  ToolButton* button;
  if (option == OKButtonOption)
    {
      button = new ToolButton("OK");
      _buttonLayout->addWidget(button);
      button->sigClicked.connect(sigc::mem_fun(this, &Dialog::accept));
    }
  else if (option == CancelButtonOption)
    {
      button = new ToolButton("Cancel");
      _buttonLayout->addWidget(button);
      button->sigClicked.connect(sigc::mem_fun(this, &Dialog::reject));
    }
  else if (option == OKCancelButtonOption)
    {
      button = new ToolButton("OK");
      _buttonLayout->addWidget(button);
      button->sigClicked.connect(sigc::mem_fun(this, &Dialog::accept));

      button = new ToolButton("Cancel");
      _buttonLayout->addWidget(button);
      button->sigClicked.connect(sigc::mem_fun(this, &Dialog::reject));
    }
  else
    {
      button = new ToolButton("Yes");
      _buttonLayout->addWidget(button);
      button->sigClicked.connect(sigc::mem_fun(this, &Dialog::accept));

      button = new ToolButton("No");
      _buttonLayout->addWidget(button);
      button->sigClicked.connect(sigc::mem_fun(this, &Dialog::reject));

      if (option == YesNoCancelButtonOption)
        {
          button = new ToolButton("Cancel");
          _buttonLayout->addWidget(button);
          button->sigClicked.connect(
              sigc::bind<int>(sigc::mem_fun(this, &Dialog::finish), -1));
        }
    }

  addChild(_buttonLayout);
  _buttonLayoutSize = _buttonLayout->preferredSize();
}

void
Dialog::setButtonLayout(LayoutBase* buttonLayout)
{
  if (!buttonLayout)
    return;

  if (_buttonLayout)
    removeChild(_buttonLayout);

  _buttonLayout = buttonLayout;
  addChild(_buttonLayout);
  _buttonLayoutSize = _buttonLayout->preferredSize();
}

void
Dialog::setTitle(const std::string& title)
{
  _title = title;
  _titleSize = FontMetrics::getSize(designer()->defaultFont(TitleFont), _title);
}

int
Dialog::canvasY() const
{
  return _canvasTopLeft.y() + _margin.top() + borderWidth()
      + _titleSize.height();
}

int
Dialog::canvasHeight() const
{
  return height() - (_canvasTopLeft.y() + _margin.vSum() + 2 * borderWidth()
      + _titleSize.height() + _buttonLayoutSize.height() + 2 * spacing());
}

void
Dialog::compose(const Rectangle& rect)
{
  Painter painter(this);
  painter.begin(rect);
  designer()->drawDialog(&painter, this);
  painter.end();
}

bool
Dialog::handleWindowEvent(const DFBWindowEvent& event)
{
  if (event.type == DWET_CLOSE)
    {
      finish(-1);
      return true;
    }

  Widget* target = this;
  if (_windowUIManager->grabbedWidget())
    target = _windowUIManager->grabbedWidget();

  switch (event.type)
    {
  case DWET_MOTION:
    return target->consumePointerEvent(
        PointerEvent(PointerMotion, event.x, event.y));

  case DWET_BUTTONDOWN:
    return target->consumePointerEvent(
        PointerEvent(PointerButtonDown, event.x, event.y, 0,
            (PointerButton) event.button, (PointerButtonMask) event.buttons));

  case DWET_BUTTONUP:
    return target->consumePointerEvent(
        PointerEvent(PointerButtonUp, event.x, event.y, 0,
            (PointerButton) event.button, (PointerButtonMask) event.buttons));

  case DWET_WHEEL:
    return target->consumePointerEvent(
        PointerEvent(PointerWheel, event.x, event.y, event.step));

  case DWET_KEYUP:
    if (event.key_symbol == DIKS_ESCAPE)
      {
        finish(-1);
        return true;
      }
    if (_uiManager->focusedWidget())
      return _uiManager->focusedWidget()->consumeKeyEvent(
          KeyEvent(KeyUpEvent, event.key_symbol, event.modifiers, event.locks));
    return false;

  case DWET_KEYDOWN:
    if (_windowUIManager->focusedWidget())
      return _windowUIManager->focusedWidget()->consumeKeyEvent(
          KeyEvent(KeyDownEvent, event.key_symbol, event.modifiers, event.locks));
    return false;

  default:
    return false;
    }
}

void
Dialog::closeDialog()
{
  hideWindow(Fade);
  _window->UngrabPointer(_window);
  invalidateSurface();
  releaseWindow();
}

void
Dialog::updateButtonLayoutGeometry()
{
  if (_buttonLayout->count() == 0)
    return;

  _buttonLayout->moveTo(borderHorizontalOffset(),
      height() - (_buttonLayoutSize.height() + spacing() + borderWidth()));

  _buttonLayout->setSize(width() - 2 * borderHorizontalOffset(),
      _buttonLayoutSize.height());
}
