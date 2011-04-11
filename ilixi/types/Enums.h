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

#ifndef ILIXI_ENUMS_H_
#define ILIXI_ENUMS_H_

namespace ilixi
{
  /*!
   * The state of a widget is mainly used for painting and it is usually changed upon receiving input events.
   */
  enum WidgetState
  {
    DefaultState = 0x00, //!< Widget is visible and at its default state.
    ExposedState = 0x01, //! Pointer is over widget. \sa Widget::enterEvent(), Widget::leaveEvent()
    FocusedState = 0x02, //!< Widget has key input focus.
    PressedState = 0x04, //!< Pointer is pressed and it is over widget.
    DisabledState = 0x08, //!< Widget is disabled.
    InvisibleState = 0x10
  //!< Widget is invisible.
  };

  /*!
   * These flags specify various functionality of the surface of a widget.
   */
  enum SurfaceDescriptionFlags
  {
    InitialiseSurface = 0x01, //!< If set widget should (re)initialise its surface.
    SurfaceModified = 0x02, //!< If set widget's surface geometry is modified.
    SharedSize = 0x04, //!< Widget's width and height are always equal in surface and frame geometries.
    HasOwnSurface = 0x08, //!< Widget has an independent surface and its surface is not a sub-surface of any parent widget.
    RootWindow = 0x10
  //!< Widget's surface is directly acquired from root Window (not a sub-surface).
  };

  /*!
   * These ORed values are used to describe the functionality of a widget's surface and geometry.
   */
  enum SurfaceDescription
  {
    DefaultDescription = 0x05, //!< (InitialiseSurface | SharedSize)
    BlitDescription = 0x09, //!< Used if widget blits itself onto another widget, e.g. main widget inside a ScrollArea. (InitialiseSurface | HasOwnSurface)
    WindowDescription = 0x15
  //!< Used if widget is derived from Window, e.g. Application or Dialog. (InitialiseSurface | SharedSize | RootWindow)
  };

  /*!
   * This enum specifies various input capabilities for widgets.
   */
  enum InputCapability
  {
    KeyInput = 0x01, //!< Widget can consume key input events.
    PointerInput = 0x02, //!< Widget can consume pointer events except motion.
    PointerTracking = 0x04, //!< Widget can consume pointer motion events contiguously.
    OSKInput = 0x08,
  //!< Widget can use OSK inputs.
  };

  /*!
   * These ORed values are used to define which input events can be consumed by a widget.
   */
  enum WidgetInputMethod
  {
    NoInput = 0x00, //!< Widget does not consume pointer or key events.
    KeyInputOnly = 0x01, //!< Widget is only capable of consuming key events. (KeyInput)
    PointerInputOnly = 0x02, //!< Widget is only capable of consuming pointer events excluding motion. (PointerInput)
    KeyAndPointerInput = 0x03, //!< Widget is capable of consuming both key and pointer events. (KeyInput | PointerInput)
    PointerInputTracking = 0x06, //!< Widget is only capable of consuming pointer events including motion. (PointerInput | PointerTracking)
    KeyAndPointerInputTracking = 0x07, //!< Widget is capable of consuming both key and pointer events. (KeyInput | PointerInput | PointerTracking)
    OSKInputEnabled = 0x0B, //!< (KeyInput | PointerInput | OSKInput)
    OSKInputTracking = 0x0F
  //!< (KeyInput | PointerInput | PointerTracking | OSKInput)
  };

  /*!
   * ResizeConstraint is used to specify how widgets should behave inside layouts.
   *
   * Each widget has horizontal and vertical constraints and they alter layout's
   * default resizing behaviour for width or height respectively.
   */
  enum WidgetResizeConstraint
  {
    FixedConstraint = 0x00, //!< Only widget's preferredSize() must be used. Widget can not grow or shrink.
    MinimumConstraint = 0x01, //!< Widget's preferredSize() provides minimum. However, widget can grow if needed. (GrowPolicy)
    MaximumConstraint = 0x02, //!< Widget's preferredSize() provides maximum. However, widget can shrink if needed. (ShrinkPolicy)
    NoConstraint = 0x03, //!< Widget's preferredSize() is used. However, widget can grow or shrink if needed. (GrowPolicy | ShrinkPolicy)
    MinimumExpandingConstraint = 0x05, //!< Widget's preferredSize() is used. However, widget can not shrink. Widget should expand if possible. (GrowPolicy | ExpandPolicy)
    ExpandingConstraint = 0x07, //!< Widget's preferredSize() is used. However, widget can grow or shrink if needed. Widget should expand if possible. (GrowPolicy | ShrinkPolicy | ExpandPolicy)
    IgnoredConstraint = 0x08
  //!< Widget is ignored by layout. Its position or size is not modified.
  };

  /*!
   * This enum is used to set widget's resize constraints.
   */
  enum WidgetResizePolicy
  {
    GrowPolicy = 0x01, //!< Widget is able to grow and function properly.
    ShrinkPolicy = 0x02,//!< Widget is able to shrink and function properly.
    ExpandPolicy = 0x04, //!< Widget should get as much space as possible.
    IgnorePolicy = 0x08,
  //!< Widget is ignored by layout manager.
  };

  /*!
   * This is enum is used to control widget's border drawing style.
   */
  enum WidgetBorderStyle
  {
    NoBorder, //!< NoBorder
    PlainBorder, //!< PlainBorder
    RaisedBorder, //!< RaisedBorder
    SunkenBorder, //!< SunkenBorder
    StyledBorder
  };

  //  /*!
  //   * This enum is provided for convenience. It simply maps from pango's implementation.
  //   */
  //  enum EllipsizeMode
  //  {
  //    EllipsizeNone, //!< No ellipsization
  //    EllipsizeStart, //!< Omit characters at the start of the text
  //    EllipsizeMiddle, //!< Omit characters in the middle of the text
  //    EllipsizeEnd
  //  //!< Omit characters at the end of the text
  //  };

  enum DesignerFontType
  {
    DefaultFont, ButtonFont, TitleFont, InputFont
  };

  enum DesignerIconType
  {
    InformationMessage, QuestionMessage, WarningMessage, CriticalMessage
  };

  enum WidgetSizeHintType
  {
    ButtonSizeHint,
    CheckBoxSizeHint,
    RadioButtonSizeHint,
    ProgressBarSizeHint,
    SliderSizeHint,
    ScrollBarSizeHint,
    ScrollBarButtonSizeHint,
    TabPanelSizeHint,
  };

  enum WidgetHint
  {
    BorderWidth,
    FrameBorderRadius,
    TextInputFrameRadius,
    DialogShadow,

    ButtonHeight,
    ButtonWidth,
    ButtonOffset,
    ButtonRadius,
    ButtonCheckedIndicatorWidth,

    RadioHeight,
    RadioWidth,
    RadioOffset,

    CheckBoxHeight,
    CheckBoxWidth,
    CheckBoxOffset,
    CheckBoxRadius,

    ComboBoxRadius,
    ComboBoxButtonWidth,

    ProgressBarHeight,
    ProgressBarWidth,
    ProgressBarRadius,

    SliderHeight,
    SliderWidth,
    SliderRadius,
    SliderFrameHeight,

    ScrollBarHeight,
    ScrollBarWidth,
    ScrollBarRadius,
    ScrollBarButtonHeight,
    ScrollBarButtonWidth,

    TabPanelHeight,
    TabPanelWidth,
    TabPanelButtonHeight
  };

  /*!
   * This enum is used to specify orientation of a widget, e.g. a horizontal slider control.
   */
  enum Orientation
  {
    Horizontal, //!< Horizontal orientation
    Vertical
  //!< Vertical orientation
  };

  /*!
   * This enum is used to specify vertical alignment of text within layouts..
   */
  enum VerticalAlignment
  {
    AlignVTop, //!< Top alignment.
    AlignVCenter, //!< Middle alignment.
    AlignVBottom
  //!< Bottom alignment.
  };

  /*!
   * This enum specifies direction, e.g. an arrow direction inside a button.
   */
  enum Direction
  {
    Left, //!< Left
    Right, //!< Right
    Up, //!< Up
    Down
  //!< Down
  };

  /*!
   * This enum specifies a corner inside a rectangle.
   */
  enum Corner
  {
    TopLeft = 0x01, //!< Top left corner.
    TopRight = 0x02, //!< Top right corner.
    BottomLeft = 0x04,//!< Bottom left corner.
    BottomRight = 0x08
  //!< Bottom right corner.
  };

  /*!
   * This enum specifies groups of corners of a rectangle.
   */
  enum Corners
  {
    NoCorners = 0x00, //!< No corners.
    LeftCorners = 0x05, //!< Left corners (TopLeft | BottomLeft).
    RightCorners = 0x0A, //!< Right corners (TopRight | BottomRight).
    TopCorners = 0x03, //!< Top corners (TopLeft | TopRight).
    BottomCorners = 0x0C,//!< Bottom corners (BottomLeft | BottomRight).
    AllCorners = 0x0F, //!< All corners (TopLeft | TopRight | BottomLeft | BottomRight).
    AllExceptTopLeft = 0x0E, //!< (TopRight | BottomLeft | BottomRight).
    AllExceptTopRight = 0x0D, //!< (TopLeft | BottomLeft | BottomRight).
    AllExceptBottomLeft = 0x0B, //!< (TopLeft | TopRight | BottomRight).
    AllExceptBottomRight = 0x07,
  //!< (TopLeft | TopRight | BottomLeft).
  };

  struct LayoutData
  {
    int row;
    int col;
    int hSpan;
    int vSpan;
    // alignment
  };
}

#endif /* ILIXI_ENUMS_H_ */
