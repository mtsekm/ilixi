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

#ifndef ILIXI_TDESIGNER_H_
#define ILIXI_TDESIGNER_H_

#include "graphics/Designer.h"
#include "types/Rectangle.h"

namespace ilixi
{
  class TDesigner : public Designer
  {
  public:
    TDesigner(const std::string& styleSheet);

    virtual
    ~TDesigner();

    virtual void
    drawPushButton(Painter* painter, PushButton* button);

    virtual void
    drawToolButton(Painter* painter, ToolButton* button);

    virtual void
    drawCheckBox(Painter* painter, Checkbox* button);

    virtual void
    drawComboBox(Painter* painter, ComboBox* combo);

    virtual void
    drawRadioButton(Painter* painter, RadioButton* button);

    virtual void
    drawProgressBar(Painter* painter, ProgressBar* bar);

    virtual void
    drawSlider(Painter* painter, Slider* slider);

    virtual void
    drawScrollBar(Painter* painter, ScrollBar* bar);

    virtual void
    drawDirectionalButton(Painter* painter, DirectionalButton* button);

    virtual void
    drawGroupBox(Painter* painter, GroupBox* box);

    virtual void
    drawTabPanel(Painter* painter, TabPanel* panel);

    virtual void
    drawTabPanelButton(Painter* painter, TabPanelButton* button);

    virtual void
    drawAppFrame(Painter* painter, Application* app);

    virtual void
    drawFrame(Painter* painter, const BorderBase* widget, int x = 0, int y = 0,
        int width = 0, int height = 0, bool filled = false, Corners corners =
            AllCorners);

    virtual void
    drawDialog(Painter* painter, Dialog* dialog);

    virtual void
    drawLineEdit(Painter* painter, LineEdit* input, const Rectangle& selection);

  private:
    void
    drawButtonCheckIndicator(Painter* painter, Button* button, bool horizontal =
        false);
  };
}

#endif /* ILIXI_TDESIGNER_H_ */
