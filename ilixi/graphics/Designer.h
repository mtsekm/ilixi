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

#ifndef ILIXI_DESIGNER_H_
#define ILIXI_DESIGNER_H_

#include "types/Rectangle.h"
#include "types/Enums.h"
#include "types/Image.h"
#include "types/Font.h"
#include "graphics/Palette.h"
#include <libxml/tree.h>

namespace ilixi
{
  // Forward declarations.
  class Painter;
  class Widget;
  class Label;
  class Icon;
  class Button;
  class PushButton;
  class ToolButton;
  class Checkbox;
  class ComboBox;
  class RadioButton;
  class ProgressBar;
  class Slider;
  class ScrollBar;
  class DirectionalButton;
  class GroupBox;
  class TabPanel;
  class TabPanelButton;
  class Application;
  class BorderBase;
  class Dialog;
  class MessageDialog;
  class LineEdit;
  class Line;

  //! Draws standard widgets with a basic look.
  /*!
   * This class parses a stylesheet and stores palette, images, fonts and hints.
   * Users can override for drawing in a specific look.
   */
  class Designer
  {
    friend class Application;
  public:
    /*!
     * Constructor.
     * @param styleSheet
     */
    Designer(const std::string& styleSheet);

    /*!
     * Destructor.
     */
    virtual
    ~Designer();

    /*!
     * Draws a label.
     */
    virtual void
    drawLabel(Painter* painter, Label* label);

    /*!
     * Draws an icon.
     */
    virtual void
    drawIcon(Painter* painter, Icon* icon);

    /*!
     * Draws a push button.
     */
    virtual void
    drawPushButton(Painter* painter, PushButton* button);

    /*!
     * Draws a tool button.
     */
    virtual void
    drawToolButton(Painter* painter, ToolButton* button);

    /*!
     * Draws a check box.
     */
    virtual void
    drawCheckBox(Painter* painter, Checkbox* button);

    /*!
     * Draws a combo box.
     */
    virtual void
    drawComboBox(Painter* painter, ComboBox* combo);

    /*!
     * Draws a radio button.
     */
    virtual void
    drawRadioButton(Painter* painter, RadioButton* button);

    /*!
     * Draws a progress bar.
     */
    virtual void
    drawProgressBar(Painter* painter, ProgressBar* bar);

    /*!
     * Draws a slider.
     */
    virtual void
    drawSlider(Painter* painter, Slider* slider);

    /*!
     * Draws a scroll bar.
     */
    virtual void
    drawScrollBar(Painter* painter, ScrollBar* bar);

    /*!
     * Draws a directional button.
     */
    virtual void
    drawDirectionalButton(Painter* painter, DirectionalButton* button);

    /*!
     * Draws a group box.
     */
    virtual void
    drawGroupBox(Painter* painter, GroupBox* box);

    /*!
     * Draws a tab panel.
     */
    virtual void
    drawTabPanel(Painter* painter, TabPanel* panel);

    /*!
     * Draws a tab panel button.
     */
    virtual void
    drawTabPanelButton(Painter* painter, TabPanelButton* button);

    /*!
     * Draws an application frame.
     */
    virtual void
    drawAppFrame(Painter* painter, Application* app);

    /*!
     * Draws a frame.
     */
    virtual void
    drawFrame(Painter* painter, const BorderBase* widget, int x = 0, int y = 0,
        int width = 0, int height = 0, bool filled = false, Corners corners =
            AllCorners);

    /*!
     * Draws a frame with a title.
     */
    virtual void
    drawTitledFrame(Painter* painter, const BorderBase* widget, int x = 0,
        int y = 0, int width = 0, int height = 0, int titleWidth = 0,
        int titleHeight = 0, bool filled = false);

    /*!
     * Draws a dialog's frame and its title.
     */
    virtual void
    drawDialog(Painter* painter, Dialog* dialog);

    /*!
     * Draws a line edit box and its text.
     */
    virtual void
    drawLineEdit(Painter* painter, LineEdit* input, const Rectangle& selection);

    /*!
     * Draws an input cursor inside the line edit.
     */
    virtual void
    drawInputCursor(Painter* painter, LineEdit* input, const Rectangle& cursor,
        bool show, const Rectangle& selection);

    /*!
     * Draws a horizontal or vertical line.
     */
    virtual void
    drawLine(Painter* painter, Line* line);

    /*!
     * Returns the size hint for given type.
     */
    virtual Size
    sizeHint(WidgetSizeHintType type) const;

    /*!
     * Returns the hint for given type.
     */
    int
    hint(WidgetHint type) const;

    /*!
     * Returns the default font for given type.
     */
    Font*
    defaultFont(DesignerFontType type) const;

    /*!
     * Returns the default icon for fiven type.
     */
    Image*
    defaultIcon(DesignerIconType type) const;

  protected:
    //! This property stores the palette.
    Palette _palette;
    //! This property specifies the border width of widgets.
    int _borderWidth;
    //! This property specifies the default frame border radius.
    int _frameBorderRadius;

    //! This property specifies the button radius.
    int _buttonRadius;
    //! This property specifies the space between icon and text.
    int _buttonOffset;
    //! This property specifies the width of tool button indicators.
    int _buttonCheckedIndicatorWidth;
    //! This property specifies  the default size for buttons.
    Size _buttonSize;

    //! This property specifies the check box radius.
    int _checkboxRadius;
    //! This property specifies the space between icon and text.
    int _checkboxOffset;
    //! This property specifies the default size for check box.
    Size _checkboxSize;

    //! This property specifies the space between icon and text.
    int _radiobuttonOffset;
    //! This property specifies the default size for radio button.
    Size _radiobuttonSize;

    //! This property specifies the combo box frame radius.
    int _comboboxRadius;
    //! This property specifies the combo box button width.
    int _comboboxButtonWidth;

    //! This property specifies progress bar frame radius.
    int _progressbarRadius;
    //! This property specifies the default size for progress bar.
    Size _progressbarSize;

    //! This property specifies the slider frame radius.
    int _sliderRadius;
    //! This property specifies the slider frame height.
    int _sliderFrameHeight;
    //! This property specifies the default size for slider.
    Size _sliderSize;

    //! This property specifies the scroll bar button width.
    int _scrollbarButtonWidth;
    //! This property specifies the scroll bar frame radius.
    int _scrollbarRadius;
    //! This property specifies the default size for scroll bar.
    Size _scrollbarSize;

    //! This property specifies the tab panel button height.
    int _tabPanelButtonHeight;
    //! This property specifies the space
    int _tabPanelButtonOffset;
    //! This property specifies the default size for panel.
    Size _tabPanelSize;

    //! This property specifies the frame text input frame radius.
    int _textInputFrameRadius;

    //! This property specifies the dialog shadow.
    int _dialogShadow;

    //! Arrow image used in buttons for up.
    Image* _arrowUp;
    //! Arrow image used in buttons for down.
    Image* _arrowDown;
    //! Arrow image used in buttons for left.
    Image* _arrowLeft;
    //! Arrow image used in buttons for right.
    Image* _arrowRight;
    //! CheckBox not selected image.
    Image* _checkEmpty;
    //! CheckBox partially selected image.
    Image* _checkPartial;
    //! CheckBox selected state image.
    Image* _checkFull;
    //! ScrollBar grid image.
    Image* _grid;
    //! Plus sign image.
    Image* _plusSign;
    //! Minus sign image.
    Image* _minusSign;
    //! Default dialog icon.
    Image* _dialog;
    //! Critical dialog icon.
    Image* _dialogCritical;
    //! Information dialog icon.
    Image* _dialogInfo;
    //! Question dialog icon.
    Image* _dialogQuestion;
    //! Warning dialog icon.
    Image* _dialogWarning;

    //! Default font to render text.
    Font* _defaultFont;
    //! Font for rendering button text.
    Font* _buttonFont;
    //! Font for rendering title text.
    Font* _titleFont;
    //! Font for rendering input text.
    Font* _inputFont;

    /*!
     * Parses an XML file and initialises style parameters.
     *
     * @param styleSheet An XML file to parse.
     * @param path Valid file system path for loading stylesheet and any files.
     */
    void
    setStyleSheet(const std::string& styleSheet, const std::string& path);

    /*!
     * Delete images and fonts.
     */
    void
    cleanStyleElements();

    /*!
     * Parses colors from stylesheet.
     */
    void
    parsePalette(xmlDoc* node);

    /*!
     * Parses fonts from stylesheet.
     */
    void
    parseFonts(xmlDoc* node);

    /*!
     * Parses hints from stylesheet.
     */
    void
    parseHints(xmlDoc* node);

    /*!
     * Parses images from stylesheet.
     */
    void
    parseImages(std::string path, xmlDoc* node);

    void
        drawImage(Painter* painter, Image* image, int x, int y, bool disabled =
            false, const DFBSurfaceBlittingFlags& flags =
            DSBLIT_BLEND_ALPHACHANNEL);

    void
    drawImage(Painter* painter, Image* image, const Rectangle& rect,
        bool disabled = false, const DFBSurfaceBlittingFlags& flags =
            DSBLIT_BLEND_ALPHACHANNEL);
  };
}

#endif /* ILIXI_DESIGNER_H_ */
