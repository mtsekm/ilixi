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

#include "graphics/TDesigner.h"
#include "graphics/Painter.h"
#include "types/LinearGradient.h"
#include "types/RadialGradient.h"
#include "ilixiGUI.h"

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#include "core/Logger.h"

using namespace ilixi;

TDesigner::TDesigner(const std::string& styleSheet) :
  Designer(styleSheet)
{
  _borderWidth = 2;
  setStyleSheet(styleSheet, ILIXI_DATADIR"tdesigner/");
}

TDesigner::~TDesigner()
{
}

void
TDesigner::drawPushButton(Painter* painter, PushButton* button)
{
  Pen* pen = painter->getPen();
  Brush* brush = painter->getBrush();
  const WidgetState state = button->state();

  // set brush
  LinearGradient background(0, 0, 0, button->height());
  if (button->checkable() && button->checked())
    {
      background.addStop(_palette._pressed._backgroundTop, 0);
      background.addStop(_palette._pressed._backgroundBottom, 0.5);
    }
  else
    {
      background.addStop(_palette.getGroup(state)._backgroundTop, 0);
      background.addStop(_palette.getGroup(state)._backgroundBottom, 0.5);
    }
  brush->setGradient(background);

  // set pen
  LinearGradient border(0, 0, 0, button->height());
  border.addStop(_palette.getGroup(state)._borderTop, 0);
  border.addStop(_palette.getGroup(state)._borderBottom, 1);
  pen->setGradient(border);
  pen->setLineWidth(1);

  // draw frame
  painter->drawRoundRectangle(0, 0, button->width(), button->height(),
      _buttonRadius, Painter::StrokeAndFill);

  // draw inner frame
  if (state & FocusedState)
    {
      LinearGradient borderGradient2(0, 0, button->width(), 0);
      borderGradient2.addStop(_palette.getGroup(state)._borderMid, 0);
      borderGradient2.addStop(_palette._focusTop, 0.5);
      borderGradient2.addStop(_palette.getGroup(state)._borderMid, 1);
      pen->setGradient(borderGradient2);
    }
  else
    pen->setColor(_palette.getGroup(state)._borderMid);
  painter->drawRoundRectangle(1, 1, button->width() - 2, button->height() - 2,
      _buttonRadius, Painter::StrokePath);

  bool pressed = button->pressed();

  // draw image
  if (button->hasIcon())
    drawImage(painter, button->icon(), button->iconPosition().x(),
        button->iconPosition().y() + pressed, !button->enabled());

  // draw button's pango layout
  if (!button->text().empty())
    {
      brush->setColor(_palette.getGroup(state)._text);
      painter->drawLayout(button->layout(), button->font(),
          button->layoutPosition().x(), button->layoutPosition().y() + pressed);
    }
}

void
TDesigner::drawToolButton(Painter* painter, ToolButton* button)
{
  Pen* pen = painter->getPen();
  Brush* brush = painter->getBrush();
  const WidgetState state = button->state();

  // set brush
  LinearGradient backgroundGradient(0, 0, 0, button->height());
  backgroundGradient.addStop(_palette.getGroup(state)._backgroundTop, 0);
  backgroundGradient.addStop(_palette.getGroup(state)._backgroundBottom, 0.5);
  brush->setGradient(backgroundGradient);

  // set pen
  LinearGradient borderGradient(0, 0, 0, button->height());
  borderGradient.addStop(_palette.getGroup(state)._borderTop, 0);
  borderGradient.addStop(_palette.getGroup(state)._borderBottom, 1);
  pen->setGradient(borderGradient);
  pen->setLineWidth(1);

  // draw frame
  painter->drawRoundRectangle(0, 0, button->width(), button->height(),
      _buttonRadius, Painter::StrokeAndFill);

  // draw inner ring
  if (state & FocusedState)
    {
      LinearGradient borderGradient2(0, 0, button->width(), 0);
      borderGradient2.addStop(_palette.getGroup(state)._borderMid, 0);
      borderGradient2.addStop(_palette._focusTop, 0.5);
      borderGradient2.addStop(_palette.getGroup(state)._borderMid, 1);
      pen->setGradient(borderGradient2);
    }
  else
    {
      // FIXME color c
      Color c = _palette.getGroup(state)._borderMid;
      c.setAlpha(0.2);
      LinearGradient borderGradient2(0, 0, button->width(), 0);
      borderGradient2.addStop(_palette.getGroup(state)._borderMid, 0);
      borderGradient2.addStop(c, 0.5);
      borderGradient2.addStop(_palette.getGroup(state)._borderMid, 1);
      pen->setGradient(borderGradient2);
    }
  painter->drawRoundRectangle(1, 1, button->width() - 2, button->height() - 2,
      _buttonRadius, Painter::StrokePath);

  // draw check indicator
  ToolButton::ToolButtonStyle buttonStyle = button->getToolButtonStyle();
  if (button->checkable())
    {
      bool horizontal = false;
      if ((buttonStyle == ToolButton::IconBelowText) || (buttonStyle
          == ToolButton::IconAboveText))
        horizontal = true;
      const WidgetState state = button->state();
      int y;
      if (horizontal)
        y = button->height() - _borderWidth - _buttonCheckedIndicatorWidth;
      else
        y = _borderWidth;

      // set pen and brush
      painter->getPen()->setColor(_palette._default._borderTop);

      if (button->checked())
        {
          LinearGradient fillGradient;
          if (horizontal)
            fillGradient.setPatternCoordinates(0, y, 0, y
                + _buttonCheckedIndicatorWidth);
          else
            fillGradient.setPatternCoordinates(_borderWidth, 0, _borderWidth
                + _buttonCheckedIndicatorWidth, 0);
          fillGradient.addStop(_palette.getGroup(state)._fillTop, 0);
          fillGradient.addStop(_palette.getGroup(state)._fillMid, 0.5);
          fillGradient.addStop(_palette.getGroup(state)._fillBottom, 1);
          painter->getBrush()->setGradient(fillGradient);
        }
      else
        painter->getBrush()->setColor(Color(0, 0, 0, 0.2));

      // draw indicator
      if (horizontal)
        painter->drawRoundRectangle(3, y, button->width() - 6,
            _buttonCheckedIndicatorWidth, _buttonRadius - 2,
            Painter::StrokeAndFill);
      else
        painter->drawRoundRectangle(3, _borderWidth,
            _buttonCheckedIndicatorWidth, button->height() - 4, _buttonRadius
                - 2, Painter::StrokeAndFill);
    }

  // Draw button icon and text
  bool pressed = button->pressed();
  switch (buttonStyle)
    {
  case ToolButton::IconOnly:
    if (button->hasIcon())
      drawImage(painter, button->icon(), button->iconPosition().x(),
          button->iconPosition().y() + pressed, !button->enabled());
    break;

  case ToolButton::TextOnly:
    // draw button's pango layout
    if (!button->text().empty())
      {
        brush->setColor(_palette.getGroup(state)._text);
        painter->drawLayout(button->layout(), button->font(),
            button->layoutPosition().x(), button->layoutPosition().y()
                + pressed);
      }
    break;

  default:
    // draw image
    if (button->hasIcon())
      drawImage(painter, button->icon(), button->iconPosition().x(),
          button->iconPosition().y() + pressed, !button->enabled());

    // draw button's pango layout
    if (!button->text().empty())
      {
        brush->setColor(_palette.getGroup(state)._text);
        painter->drawLayout(button->layout(), button->font(),
            button->layoutPosition().x(), button->layoutPosition().y()
                + pressed);
      }
    break;
    }
}

void
TDesigner::drawButtonCheckIndicator(Painter* painter, Button* button,
    bool horizontal)
{
  if (button->checkable())
    {
      const WidgetState state = button->state();
      int y;
      if (horizontal)
        y = button->height() - _borderWidth - _buttonCheckedIndicatorWidth;
      else
        y = _borderWidth;

      // set pen and brush
      painter->getPen()->setColor(_palette._default._borderTop);

      if (button->checked())
        {
          LinearGradient fillGradient;
          if (horizontal)
            fillGradient.setPatternCoordinates(0, y, 0, y
                + _buttonCheckedIndicatorWidth);
          else
            fillGradient.setPatternCoordinates(_borderWidth, 0, _borderWidth
                + _buttonCheckedIndicatorWidth, 0);
          fillGradient.addStop(_palette.getGroup(state)._fillTop, 0);
          fillGradient.addStop(_palette.getGroup(state)._fillMid, 0.5);
          fillGradient.addStop(_palette.getGroup(state)._fillBottom, 1);
          painter->getBrush()->setGradient(fillGradient);
        }
      else
        painter->getBrush()->setColor(Color(0, 0, 0, 0.2));

      // draw indicator
      if (horizontal)
        painter->drawRoundRectangle(3, y, button->width() - 6,
            _buttonCheckedIndicatorWidth, _buttonRadius - 2,
            Painter::StrokeAndFill);
      else
        painter->drawRoundRectangle(3, _borderWidth,
            _buttonCheckedIndicatorWidth, button->height() - 4, _buttonRadius
                - 2, Painter::StrokeAndFill);
    }
}

void
TDesigner::drawCheckBox(Painter* painter, Checkbox* button)
{
  Pen* pen = painter->getPen();
  Brush* brush = painter->getBrush();
  const WidgetState state = button->state();
  bool disabled = state & DisabledState;
  int x = _checkboxOffset;
  int y = (button->height() - _checkboxSize.height()) / 2;

  // set brush and pen
  LinearGradient background(0, y, 0, y + _checkboxSize.height());
  background.addStop(_palette.getGroup(state)._backgroundTop, 0);
  background.addStop(_palette.getGroup(state)._backgroundBottom, 1);
  brush->setGradient(background);
  pen->setColor(_palette.getGroup(state)._borderBottom);
  pen->setLineWidth(1);

  // draw frame
  painter->drawRoundRectangle(x, y, _checkboxSize.width(),
      _checkboxSize.height(), _checkboxRadius, Painter::StrokeAndFill);

  // draw inner ring
  if (state & FocusedState)
    pen->setColor(_palette._focusTop);
  else
    pen->setColor(_palette.getGroup(state)._borderMid);
  painter->drawRoundRectangle(x + 1, y + 1, _checkboxSize.width() - 2,
      _checkboxSize.height() - 2, _checkboxRadius, Painter::StrokePath);

  // draw checkbox state
  Rectangle r(x, y, _checkboxSize.width(), _checkboxSize.height());
  if (button->checkState() == Checkbox::Checked)
    drawImage(painter, _checkFull, r, disabled);
  else if (button->checkState() == Checkbox::Partial)
    drawImage(painter, _checkPartial, r, disabled);
  else
    drawImage(painter, _checkEmpty, r, disabled);

  // draw image
  if (button->hasIcon())
    drawImage(painter, button->icon(), button->iconPosition().x(),
        button->iconPosition().y(), !button->enabled());

  // draw button's pango layout
  if (!button->text().empty())
    {
      brush->setColor(_palette.getGroup(state)._text);
      painter->drawLayout(button->layout(), button->font(),
          button->layoutPosition());
    }
}

void
TDesigner::drawComboBox(Painter* painter, ComboBox* combo)
{
  Pen* pen = painter->getPen();
  Brush* brush = painter->getBrush();
  const WidgetState state = combo->state();

  brush->setColor(_palette.getGroup(state)._base);
  if (state & FocusedState)
    {
      pen->setColor(_palette._focusTop);
      pen->setLineWidth(2);
      // draw frame
      painter->drawRoundRectangle(0, 0, combo->width(), combo->height(),
          _comboboxRadius, Painter::StrokeAndFill);
    }
  else
    {
      pen->setLineWidth(1);
      LinearGradient borderGradient(0, 0, 0, combo->height());
      borderGradient.addStop(_palette.getGroup(state)._borderBottom, 0);
      borderGradient.addStop(_palette.getGroup(state)._borderTop, 1);
      pen->setGradient(borderGradient);

      // draw frame
      painter->drawRoundRectangle(0, 0, combo->width(), combo->height(),
          _comboboxRadius, Painter::StrokePath);
      // draw inner frame
      pen->setColor(_palette.getGroup(state)._borderMid);
      painter->drawRoundRectangle(1, 1, combo->width() - 2,
          combo->height() - 2, _comboboxRadius, Painter::StrokeAndFill);
    }

  // draw button
  LinearGradient background(0, 0, 0, combo->height());
  background.addStop(_palette.getGroup(state)._backgroundTop, 0);
  background.addStop(_palette.getGroup(state)._backgroundBottom, 0.5);
  brush->setGradient(background);
  int x = combo->width() - _comboboxButtonWidth;
  painter->drawRoundRectangle(x, 2, combo->width() - x - 1,
      combo->height() - 4, _comboboxRadius, Painter::FillPath, RightCorners);

  brush->setColor(_palette.getGroup(state)._baseText);
  painter->drawLayout(combo->layout(), combo->font(), combo->layoutPosition());

  drawImage(painter, _arrowDown, Rectangle(x, 1, combo->height(),
      combo->height()), !combo->enabled());
}

void
TDesigner::drawRadioButton(Painter* painter, RadioButton* button)
{
  Pen* pen = painter->getPen();
  Brush* brush = painter->getBrush();
  const WidgetState state = button->state();
  bool disabled = state & DisabledState;
  int x = _radiobuttonOffset;
  int y = (button->height() - _radiobuttonSize.height()) / 2;

  // set brush and pen
  LinearGradient background(0, y, 0, y + _radiobuttonSize.height());
  background.addStop(_palette.getGroup(state)._backgroundTop, 0);
  background.addStop(_palette.getGroup(state)._backgroundBottom, 1);
  brush->setGradient(background);
  pen->setColor(_palette.getGroup(state)._borderBottom);
  pen->setLineWidth(1);

  // draw frame
  painter->drawEllipse(x, y, _radiobuttonSize.width(),
      _radiobuttonSize.height(), Painter::StrokeAndFill);

  // draw inner ring
  if (state & FocusedState)
    pen->setColor(_palette._focusTop);
  else
    pen->setColor(_palette.getGroup(state)._borderMid);
  painter->drawEllipse(x + 1, y + 1, _radiobuttonSize.width() - 2,
      _radiobuttonSize.height() - 2, Painter::StrokePath);

  // draw selection
  pen->setColor(_palette.getGroup(state)._borderBottom);
  if (button->checked())
    brush->setColor(_palette._focusTop);
  else
    brush->setColor(_palette.getGroup(state)._borderMid);
  painter->drawEllipse(x + 4, y + 4, _radiobuttonSize.width() - 8,
      _radiobuttonSize.height() - 8, Painter::StrokeAndFill);

  // draw image
  if (button->hasIcon())
    drawImage(painter, button->icon(), button->iconPosition().x(),
        button->iconPosition().y(), !button->enabled());

  // draw button's pango layout
  if (!button->text().empty())
    {
      brush->setColor(_palette.getGroup(state)._text);
      painter->drawLayout(button->layout(), button->font(),
          button->layoutPosition());
    }
}

void
TDesigner::drawProgressBar(Painter* painter, ProgressBar* bar)
{
  Pen* pen = painter->getPen();
  Brush* brush = painter->getBrush();
  const WidgetState state = bar->state();

  // Calculate positions and dimensions
  Orientation orientation = bar->orientation();
  int frameX = 0;
  int frameY = 0;
  int frameWidth = bar->width();
  int frameHeight = bar->height();
  int fillWidth = frameWidth;
  int fillHeight = frameHeight;
  bool fillInside = bar->maximum() ? true : false;

  LinearGradient background;
  if (orientation == Horizontal)
    {
      frameY = (bar->height() - _progressbarSize.height()) / 2;
      frameHeight = _progressbarSize.height();
      background.setPatternCoordinates(0, frameY, 0, frameY + frameHeight);
    }
  else
    {
      frameX = (bar->width() - _progressbarSize.height()) / 2;
      frameWidth = _progressbarSize.height();
      background.setPatternCoordinates(frameX, 0, frameX + frameWidth, 0);
    }

  // Set frame brush & pen
  background.addStop(_palette.getGroup(state)._backgroundBottom, 0);
  background.addStop(_palette.getGroup(state)._backgroundTop, 1);
  brush->setGradient(background);
  pen->setColor(_palette.getGroup(state)._borderBottom);
  pen->setLineWidth(1);

  // Draw outer frame
  painter->drawRoundRectangle(frameX, frameY, frameWidth, frameHeight,
      _progressbarRadius, Painter::StrokeAndFill);

  // draw inner frame
  if (state & DisabledState)
    pen->setColor(_palette._disabled._borderMid);
  else
    pen->setColor(_palette._default._borderMid);
  painter->drawRoundRectangle(frameX + 1, frameY + 1, frameWidth - 2,
      frameHeight - 2, _progressbarRadius, Painter::StrokePath);

  if (bar->range() != 0)
    {
      if (bar->value())
        {
          painter->drawRoundRectangle(frameX + 1, frameY + 1, frameWidth - 2,
              frameHeight - 2, _progressbarRadius, Painter::ClipPath);

          LinearGradient fillGradient;
          if (orientation == Horizontal)
            {
              fillWidth = (bar->value() * frameWidth) / bar->range();
              fillGradient.setPatternCoordinates(0, frameY, 0, frameY
                  + frameHeight);
            }
          else
            {
              fillHeight = (bar->value() * frameHeight) / bar->range();
              fillGradient.setPatternCoordinates(frameX, 0,
                  frameX + frameWidth, 0);
            }
          fillGradient.addStop(_palette.getGroup(state)._fillTop, 0);
          fillGradient.addStop(_palette.getGroup(state)._fillMid, 0.5);
          fillGradient.addStop(_palette.getGroup(state)._fillBottom, 1);
          brush->setGradient(fillGradient);

          if (orientation == Horizontal)
            painter->drawRectangle(frameX, frameY, fillWidth, frameHeight,
                Painter::FillPath);
          else
            painter->drawRectangle(frameX, frameY + frameHeight - fillHeight,
                frameWidth, fillHeight, Painter::FillPath);

          painter->resetClip();
        }
      if (orientation == Horizontal)
        {
          char percent[6];
          brush->setColor(_palette.getGroup(state)._text);
          sprintf(percent, "%% %d", bar->percentage());
          painter->setLayoutAlignment(PANGO_ALIGN_CENTER);
          painter->drawText(percent, frameX, frameY + 1, frameWidth,
              frameHeight);
        }
    }
}

void
TDesigner::drawSlider(Painter* painter, Slider* slider)
{
  Pen* pen = painter->getPen();
  Brush* brush = painter->getBrush();
  const WidgetState state = slider->state();

  // Calculate positions and dimensions
  Rectangle frameGeometry = slider->indicatorRegion();
  int frameX = frameGeometry.x();
  int frameY = frameGeometry.y();
  int frameWidth = frameGeometry.width();
  int frameHeight = frameGeometry.height();
  int fillWidth = frameWidth;
  int fillHeight = frameHeight;
  Orientation orientation = slider->orientation();

  // Set frame brush & pen
  LinearGradient background;
  if (orientation == Horizontal)
    {
      frameY = frameGeometry.y() + 5;
      frameHeight = 10;
      background.setPatternCoordinates(0, frameY, 0, frameY + frameHeight);
    }
  else
    {
      frameX = frameGeometry.x() + 5;
      frameWidth = 10;
      background.setPatternCoordinates(frameX, 0, frameX + frameWidth, 0);
    }
  background.addStop(_palette._default._backgroundBottom, 0);
  background.addStop(_palette._default._backgroundTop, 1);
  brush->setGradient(background);
  pen->setColor(_palette.getGroup(state)._borderBottom);
  pen->setLineWidth(1);

  // Draw outer frame
  painter->drawRoundRectangle(frameX, frameY, frameWidth, frameHeight,
      _sliderRadius, Painter::StrokeAndFill);

  // draw inner frame
  if (state & DisabledState)
    pen->setColor(_palette._disabled._borderMid);
  else
    pen->setColor(_palette._default._borderMid);
  painter->drawRoundRectangle(frameX + 1, frameY + 1, frameWidth - 2,
      frameHeight - 2, _sliderRadius, Painter::StrokePath);

  if (slider->range() != 0)
    {
      if (slider->value())
        {
          painter->drawRoundRectangle(frameX + 1, frameY + 1, frameWidth - 2,
              frameHeight - 2, _progressbarRadius, Painter::ClipPath);

          LinearGradient fillGradient;
          if (orientation == Horizontal)
            {
              fillWidth = (slider->value() * frameWidth) / slider->range();
              fillGradient.setPatternCoordinates(0, frameY, 0, frameY
                  + frameHeight);
            }
          else
            {
              fillHeight = (slider->value() * frameHeight) / slider->range();
              fillGradient.setPatternCoordinates(frameX, 0,
                  frameX + frameWidth, 0);
            }
          fillGradient.addStop(_palette.getGroup(state)._fillTop, 0);
          fillGradient.addStop(_palette.getGroup(state)._fillMid, 0.5);
          fillGradient.addStop(_palette.getGroup(state)._fillBottom, 1);
          brush->setGradient(fillGradient);

          if (orientation == Horizontal)
            painter->drawRectangle(frameX, frameY, fillWidth, frameHeight,
                Painter::FillPath);
          else
            painter->drawRectangle(frameX, frameY + frameHeight - fillHeight,
                frameWidth, fillHeight, Painter::FillPath);

          painter->resetClip();
        }

      // Set indicator brush and pen
      Rectangle indicatorGeometry = slider->indicatorGeometry();

      LinearGradient indicatorBackground;

      if (orientation == Horizontal)
        indicatorBackground.setPatternCoordinates(0, indicatorGeometry.y(), 0,
            indicatorGeometry.bottom());
      else
        indicatorBackground.setPatternCoordinates(indicatorGeometry.x(), 0,
            indicatorGeometry.right(), 0);

      indicatorBackground.addStop(_palette.getGroup(state)._backgroundTop, 0);
      indicatorBackground.addStop(_palette.getGroup(state)._backgroundBottom, 1);
      brush->setGradient(indicatorBackground);

      pen->setColor(_palette.getGroup(state)._borderBottom);
      pen->setLineWidth(1);

      // draw indicator frame
      painter->drawRoundRectangle(indicatorGeometry.x(), indicatorGeometry.y(),
          indicatorGeometry.width(), indicatorGeometry.height(), _sliderRadius,
          Painter::StrokeAndFill);

      // draw focus ring
      if (state & FocusedState)
        pen->setColor(_palette._focusTop);
      else
        pen->setColor(_palette.getGroup(state)._borderMid);

      painter->drawRoundRectangle(indicatorGeometry.x() + 1,
          indicatorGeometry.y() + 1, indicatorGeometry.width() - 2,
          indicatorGeometry.height() - 2, _sliderRadius, Painter::StrokePath);

      // draw indicator grid image
      bool pressed = state & PressedState;
      if (orientation == Horizontal)
        {
          DFBSurfaceBlittingFlags flags =
              pressed ? (DFBSurfaceBlittingFlags) (DSBLIT_BLEND_ALPHACHANNEL
                  | DSBLIT_FLIP_HORIZONTAL) : DSBLIT_BLEND_ALPHACHANNEL;
          painter->drawImage(_grid, indicatorGeometry, flags);
        }
      else
        {
          DFBSurfaceBlittingFlags flags =
              pressed ? (DFBSurfaceBlittingFlags) (DSBLIT_BLEND_ALPHACHANNEL
                  | DSBLIT_ROTATE90)
                  : (DFBSurfaceBlittingFlags) (DSBLIT_BLEND_ALPHACHANNEL
                      | DSBLIT_ROTATE270);
          painter->drawImage(_grid, indicatorGeometry, flags);
        }
    }
}

void
TDesigner::drawScrollBar(Painter* painter, ScrollBar* bar)
{
  Pen* pen = painter->getPen();
  Brush* brush = painter->getBrush();
  const WidgetState state = bar->state();

  // Calculate positions and dimensions
  Rectangle frameGeometry = bar->indicatorRegion();
  int frameX = frameGeometry.x();
  int frameY = frameGeometry.y();
  int frameWidth = frameGeometry.width();
  int frameHeight = frameGeometry.height();
  Orientation orientation = bar->orientation();

  // Set frame brush & pen
  LinearGradient background;
  if (orientation == Horizontal)
    background.setPatternCoordinates(0, frameY, 0, frameY + frameHeight);
  else
    background.setPatternCoordinates(frameX, 0, frameX + frameWidth, 0);
  background.addStop(_palette._default._backgroundBottom, 0);
  background.addStop(_palette._default._backgroundTop, 1);
  brush->setGradient(background);

  pen->setColor(_palette.getGroup(state)._borderBottom);
  pen->setLineWidth(1);

  // Draw outer frame
  painter->drawRectangle(frameX, frameY, frameWidth, frameHeight,
      Painter::StrokeAndFill);

  // draw focus frame
  if (state & DisabledState)
    pen->setColor(_palette._disabled._borderMid);
  else
    pen->setColor(_palette._default._borderMid);
  painter->drawRectangle(frameX + 1, frameY + 1, frameWidth - 2, frameHeight
      - 2, Painter::StrokePath);

  int range = bar->range();
  if (range != 0)
    {
      Rectangle indicatorGeometry = bar->indicatorGeometry();

      // Set indicator brush and pen
      LinearGradient indicatorBackground;
      if (orientation == Horizontal)
        indicatorBackground.setPatternCoordinates(0, indicatorGeometry.y(), 0,
            indicatorGeometry.bottom());
      else
        indicatorBackground.setPatternCoordinates(indicatorGeometry.x(), 0,
            indicatorGeometry.right(), 0);
      indicatorBackground.addStop(_palette.getGroup(state)._backgroundTop, 0);
      indicatorBackground.addStop(_palette.getGroup(state)._backgroundBottom, 1);
      brush->setGradient(indicatorBackground);

      pen->setColor(_palette.getGroup(state)._borderBottom);
      pen->setLineWidth(1);

      // draw indicator frame
      painter->drawRoundRectangle(indicatorGeometry.x(), indicatorGeometry.y(),
          indicatorGeometry.width(), indicatorGeometry.height(),
          _scrollbarRadius, Painter::StrokeAndFill);

      // draw focus ring
      if (state & FocusedState)
        pen->setColor(_palette._focusTop);
      else
        pen->setColor(_palette.getGroup(state)._borderMid);
      painter->drawRoundRectangle(indicatorGeometry.x() + 1,
          indicatorGeometry.y() + 1, indicatorGeometry.width() - 2,
          indicatorGeometry.height() - 2, _scrollbarRadius, Painter::StrokePath);

      // draw indicator grid image
      bool pressed = state & PressedState;
      if (orientation == Horizontal)
        {
          DFBSurfaceBlittingFlags flags =
              pressed ? (DFBSurfaceBlittingFlags) (DSBLIT_BLEND_ALPHACHANNEL
                  | DSBLIT_FLIP_HORIZONTAL) : DSBLIT_BLEND_ALPHACHANNEL;
          painter->drawImage(_grid, indicatorGeometry.x()
              + (indicatorGeometry.width() - _scrollbarSize.height()) / 2,
              indicatorGeometry.y(), flags);
        }
      else
        {
          DFBSurfaceBlittingFlags flags =
              pressed ? (DFBSurfaceBlittingFlags) (DSBLIT_BLEND_ALPHACHANNEL
                  | DSBLIT_ROTATE90)
                  : (DFBSurfaceBlittingFlags) (DSBLIT_BLEND_ALPHACHANNEL
                      | DSBLIT_ROTATE270);
          painter->drawImage(_grid, indicatorGeometry.x(),
              indicatorGeometry.y() + (indicatorGeometry.height()
                  - _scrollbarSize.height()) / 2, flags);
        }
    }
}

void
TDesigner::drawDirectionalButton(Painter* painter, DirectionalButton* button)
{
  Pen* pen = painter->getPen();
  Brush* brush = painter->getBrush();
  const WidgetState state = button->state();
  Direction direction = button->direction();

  // Set Brush & Pen
  LinearGradient background;
  if (direction == Up || direction == Down)
    background.setPatternCoordinates(0, 0, button->width(), 0);
  else
    background.setPatternCoordinates(0, 0, 0, button->height());
  background.addStop(_palette.getGroup(state)._backgroundTop, 0);
  background.addStop(_palette.getGroup(state)._backgroundBottom, 1);
  brush->setGradient(background);

  pen->setColor(_palette._default._borderBottom);
  pen->setLineWidth(1);

  Corners c;
  Image* img;
  bool pressed = (state & PressedState);

  if (button->type() == DirectionalButton::ArrowButton)
    {
      if (direction == Up)
        {
          img = _arrowUp;
          c = TopCorners;
        }
      else if (direction == Down)
        {
          img = _arrowDown;
          c = BottomCorners;

        }
      else if (direction == Left)
        {
          img = _arrowLeft;
          c = LeftCorners;

        }
      else if (direction == Right)
        {
          img = _arrowRight;
          c = RightCorners;
        }
    }
  else
    {
      if (direction == Up)
        {
          img = _plusSign;
          c = TopCorners;
        }
      else if (direction == Down)
        {
          img = _minusSign;
          c = BottomCorners;
        }
      else if (direction == Left)
        {
          img = _minusSign;
          c = LeftCorners;
        }
      else
        {
          img = _plusSign;
          c = RightCorners;
        }
    }

  painter->drawRoundRectangle(0, 0, button->width(), button->height(),
      _scrollbarRadius, Painter::StrokeAndFill, c);

  // draw inner ring
  if (state & FocusedState)
    pen->setColor(_palette._focusTop);
  else
    pen->setColor(_palette.getGroup(state)._borderMid);
  painter->drawRoundRectangle(1, 1, button->width() - 2, button->height() - 2,
      _scrollbarRadius, Painter::StrokePath, c);

  drawImage(painter, img, Rectangle(4, 4 + pressed, button->width() - 8,
      button->height() - 8), state & DisabledState);
}

void
TDesigner::drawGroupBox(Painter* painter, GroupBox* box)
{
  Size s = box->getTitleTextSize();

  // set pen
  painter->getPen()->setColor(_palette._default._borderBottom);
  painter->getPen()->setLineWidth(1);
  painter->getBrush()->setColor(_palette._baseTop);

  cairo_t* context = painter->cairoContext();
  double x = 0;
  double y = 0;
  double w = box->width();
  double h = box->height();
  painter->getUserCoordinates(x, y, w, h);

  cairo_new_sub_path(context);
  cairo_arc(context, x + w - _frameBorderRadius, y + s.height()
      + _frameBorderRadius, _frameBorderRadius, -1.570796325, 0); // top-right
  cairo_arc(context, x + w - _frameBorderRadius, y + h - _frameBorderRadius,
      _frameBorderRadius, 0, 1.570796325); // bottom-right
  cairo_arc(context, x + _frameBorderRadius, y + h - _frameBorderRadius,
      _frameBorderRadius, 1.570796325, 3.14159265); // bottom-left
  cairo_arc(context, x + _frameBorderRadius, y + _frameBorderRadius,
      _frameBorderRadius, 3.14159265, 4.712388975); // top-left
  cairo_arc(context, x + s.width() + 2 * box->borderHorizontalOffset()
      - _frameBorderRadius, y + _frameBorderRadius, _frameBorderRadius,
      -1.570796325, 0); // top-right (title)
  cairo_arc_negative(context, x + s.width() + 2 * box->borderHorizontalOffset()
      + _frameBorderRadius, y + s.height() - _frameBorderRadius,
      _frameBorderRadius, 3.14159265, 1.570796325); // bottom-right (title)
  cairo_close_path(context);

  if (box->backgroundFilled())
    painter->drawCurrentPath(Painter::StrokeAndFill);
  else
    painter->drawCurrentPath(Painter::StrokePath);

  ++x;
  ++y;
  w -= 2;
  h -= 2;
  painter->getPen()->setColor(_palette._default._borderMid);
  cairo_new_sub_path(context);
  cairo_arc(context, x + w - _frameBorderRadius, y + s.height()
      + _frameBorderRadius, _frameBorderRadius, -1.570796325, 0); // top-right
  cairo_arc(context, x + w - _frameBorderRadius, y + h - _frameBorderRadius,
      _frameBorderRadius, 0, 1.570796325); // bottom-right
  cairo_arc(context, x + _frameBorderRadius, y + h - _frameBorderRadius,
      _frameBorderRadius, 1.570796325, 3.14159265); // bottom-left
  cairo_arc(context, x + _frameBorderRadius, y + _frameBorderRadius,
      _frameBorderRadius, 3.14159265, 4.712388975); // top-left
  cairo_arc(context, x + s.width() + 2 * box->borderHorizontalOffset() - 2
      - _frameBorderRadius, y + _frameBorderRadius, _frameBorderRadius,
      -1.570796325, 0); // top-right (title)
  cairo_arc_negative(context, x + s.width() + 2 * box->borderHorizontalOffset()
      - 2 + _frameBorderRadius, y + s.height() - _frameBorderRadius,
      _frameBorderRadius, 3.14159265, 1.570796325); // bottom-right (title)
  cairo_close_path(context);
  painter->drawCurrentPath(Painter::StrokePath);

  // TODO Draw groupbox text using textbase layoutPosition.
  painter->setFont(_titleFont);
  painter->getBrush()->setColor(_palette.getGroup(box->state())._text);
  painter->drawText(box->title(), box->borderHorizontalOffset(), _borderWidth);
}

void
TDesigner::drawTabPanel(Painter* painter, TabPanel* panel)
{
  int x = 0;
  int y = _tabPanelButtonHeight;
  int width = panel->width();
  int height = panel->height() - _tabPanelButtonHeight;
  const WidgetState state = panel->state();
  drawFrame(painter, panel, x, y, width, height, true, AllExceptTopLeft);
}

void
TDesigner::drawTabPanelButton(Painter* painter, TabPanelButton* button)
{
  Pen* pen = painter->getPen();
  Brush* brush = painter->getBrush();
  const WidgetState state = button->state();
  double x = 0;
  double y = _tabPanelButtonOffset;
  double width = button->width();
  double height = button->height();

  double M_D2R = 0.0174532925;

  LinearGradient background(0, 0, 0, button->height());
  if (button->checked())
    {
      y = 0;
      height += _tabPanelButtonOffset;
      background.addStop(_palette._pressed._backgroundTop, 0);
      background.addStop(_palette._pressed._backgroundBottom, 1);
    }
  else
    {
      background.addStop(_palette.getGroup(state)._backgroundTop, 0);
      background.addStop(_palette.getGroup(state)._backgroundBottom, 1);
    }
  brush->setGradient(background);

  // set pen
  if (state & DisabledState)
    pen->setColor(_palette._disabled._borderBottom);
  else
    pen->setColor(_palette._default._borderBottom);
  pen->setLineWidth(1);

  painter->drawRoundRectangle(x, y, width, height, _frameBorderRadius,
      Painter::StrokeAndFill, TopCorners);

  // draw focus ring
  pen->setColor(_palette.getGroup(state)._borderMid);
  painter->drawRoundRectangle(x + 1, y + 1, width - 2, height - 2,
      _frameBorderRadius, Painter::StrokePath, TopCorners);

  // draw button Image
  x = std::max(_frameBorderRadius, _borderWidth);

  if (button->hasIcon())
    painter->drawImage(button->icon(), button->iconPosition().x(),
        button->iconPosition().y() - button->checked());

  // draw button's pango layout
  if (!button->text().empty())
    {
      brush->setColor(_palette.getGroup(state)._text);
      painter ->drawLayout(button->layout(), button->font(),
          button->layoutPosition().x(), button->layoutPosition().y() + y);
    }
}

void
TDesigner::drawAppFrame(Painter* painter, Application* app)
{
  painter->getBrush()->setColor(_palette._baseTop);
  painter->drawRectangle(0, 0, app->width(), app->height(), Painter::FillPath);
}

void
TDesigner::drawFrame(Painter* painter, const BorderBase* widget, int x, int y,
    int width, int height, bool filled, Corners corners)
{
  if (width == 0)
    width = widget->width();
  if (height == 0)
    height = widget->height();
  Pen* pen = painter->getPen();

  if (widget->borderStyle() == NoBorder)
    return;
  else if (widget->borderStyle() == PlainBorder)
    {
      pen->setLineWidth(widget->borderWidth());
      pen->setColor(_palette.getGroup(widget->state())._borderBottom);
      painter->drawRectangle(0, 0, width, height);
    }
  else if (widget->borderStyle() == RaisedBorder)
    {
      if (filled)
        {
          painter->getBrush()->setColor(_palette._baseTop);
          painter->drawRectangle(x, y, width, height, Painter::FillPath);
        }

      pen->setLineWidth(widget->borderWidth());
      pen->setColor(_palette.getGroup(widget->state())._borderTop);
      painter->drawLine(x, y, x + width, y);
      painter->drawLine(x, y, x, y + height);

      pen->setColor(_palette.getGroup(widget->state())._borderBottom);
      painter->drawLine(x + width, y, x + width, y + height);
      painter->drawLine(x, y + height, x + width, y + height);
    }
  else if (widget->borderStyle() == SunkenBorder)
    {
    }
  else if (widget->borderStyle() == StyledBorder)
    {
      // set pen
      pen->setColor(_palette._default._borderBottom);
      pen->setLineWidth(1);

      // draw frame
      if (filled)
        {
          painter->getBrush()->setColor(_palette._baseTop);
          painter->drawRoundRectangle(x, y, width, height, _frameBorderRadius,
              Painter::StrokeAndFill, corners);
        }
      else
        painter->drawRoundRectangle(x, y, width, height, _frameBorderRadius,
            Painter::StrokePath);
      // draw focus ring
      if (widget->state() & DisabledState)
        pen->setColor(_palette._disabled._borderMid);
      else
        pen->setColor(_palette._default._borderMid);
      painter->drawRoundRectangle(x + 1, y + 1, width - 2, height - 2,
          _frameBorderRadius, Painter::StrokePath, corners);
    }
}

void
TDesigner::drawDialog(Painter* painter, Dialog* dialog)
{
  Pen* pen = painter->getPen();
  int titleHeight = dialog->titleSize().height();
  // set pen
  pen->setColor(_palette._default._borderBottom);
  pen->setLineWidth(1);
  painter->getBrush()->setColor(_palette._default._borderMid);

  // draw frame
  painter->drawRoundRectangle(0, 0, dialog->width(), dialog->height(),
      _frameBorderRadius, Painter::StrokeAndFill);

  // draw inner ring
  painter->getBrush()->setColor(_palette._baseTop);
  pen->setColor(_palette._default._borderMid);
  painter->drawRoundRectangle(1, titleHeight + 1, dialog->width() - 2,
      dialog->height() - titleHeight - 2, _frameBorderRadius,
      Painter::StrokeAndFill);

  // draw title
  if (dialog->state() & DisabledState)
    painter->getBrush()->setColor(_palette._textDisabled);
  else
    painter->getBrush()->setColor(_palette._text);

  painter->setFont(_titleFont);
  painter->setEllipsizeMode(PANGO_ELLIPSIZE_END);
  painter->setLayoutAlignment(PANGO_ALIGN_CENTER);
  painter->drawText(dialog->title(), _frameBorderRadius, 1, dialog->width() - 2
      * _frameBorderRadius, titleHeight);
}

void
TDesigner::drawLineEdit(Painter* painter, LineEdit* input,
    const Rectangle& selection)
{
  Pen* pen = painter->getPen();
  Brush* brush = painter->getBrush();
  const WidgetState state = input->state();

  brush->setColor(_palette.getGroup(state)._base);

  if (state & FocusedState)
    {
      // set pen & brush
      pen->setColor(_palette._focusTop);
      pen->setLineWidth(2);

      // draw frame
      painter->drawRoundRectangle(0, 0, input->width(), input->height(),
          _textInputFrameRadius, Painter::StrokeAndFill);
    }
  else
    {
      // set pen & brush
      pen->setLineWidth(1);
      LinearGradient borderGradient(0, 0, 0, input->height());
      borderGradient.addStop(_palette.getGroup(state)._borderBottom, 0);
      borderGradient.addStop(_palette.getGroup(state)._borderTop, 1);
      pen->setGradient(borderGradient);

      // draw frame
      painter->drawRoundRectangle(0, 0, input->width(), input->height(),
          _textInputFrameRadius, Painter::StrokePath);

      // draw inner ring
      pen->setColor(_palette.getGroup(state)._borderMid);
      painter->drawRoundRectangle(1, 1, input->width() - 2,
          input->height() - 2, _textInputFrameRadius, Painter::StrokeAndFill);
    }

  // draw selection
  if (!selection.isNull())
    {
      brush->setColor(_palette._focusTop);
      painter->drawRectangle(selection, Painter::FillPath);
    }

  // draw button's pango layout
  if (!input->text().empty())
    {
      brush->setColor(_palette.getGroup(state)._baseText);
      painter->drawLayout(input->layout(), input->font(),
          input->layoutPosition());
    }
}

