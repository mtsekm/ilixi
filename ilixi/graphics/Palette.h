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

#ifndef ILIXI_PALETTE_H_
#define ILIXI_PALETTE_H_

#include "types/Enums.h"
#include "types/Color.h"

namespace ilixi
{
  //! Group of colors for widget states.
  struct ColorGroup
  {
    //! Initialise members to default colours.
    ColorGroup();

    //! Initialise members using given colours.
    ColorGroup(Color base, Color baseAlt, Color backgroundTop,
        Color backgrounMid, Color backgrounBottom, Color borderTop,
        Color borderMid, Color borderBottom, Color text);

    //! Set all colours.
    void
    setColors(Color base, Color baseAlt, Color backgroundTop,
        Color backgrounMid, Color backgrounBottom, Color borderTop,
        Color borderMid, Color borderBottom, Color text);

    //! Base colour used as the background colour of an input widget or row colour inside a list widget.
    Color _base;
    //! Text colour used for drawing text on base.
    Color _baseText;
    //! Alternating row colour used for lists.
    Color _baseAlt;
    //! Text colour used for drawing text on alternative base.
    Color _baseAltText;

    //! Primary background colour.
    Color _backgroundTop;
    //! Alternative background colour; use with gradients.
    Color _backgroundMid;
    //! Alternative background colour; use with gradients.
    Color _backgroundBottom;

    //! Primary border colour.
    Color _borderTop;
    //! Alternative border colour; use with gradients.
    Color _borderMid;
    //! Alternative border colour; use with gradients.
    Color _borderBottom;

    //! Primary colour used for filling a region inside widget.
    Color _fillTop;
    //! Alternative fill colour; use with gradients.
    Color _fillMid;
    //! Alternative fill colour; used with gradients.
    Color _fillBottom;

    //! Text colour; used for drawing text or symbols inside a widget.
    Color _text;
  };

  //! Colors for drawing and filling widgets.
  class Palette
  {
  public:
    /*!
     * Constructor.
     */
    Palette();

    /*!
     * Destructor.
     */
    ~Palette();

    /*!
     * Returns color group given a state.
     */
    ColorGroup&
    getGroup(WidgetState state);

    //! Default state.
    ColorGroup _default;
    //! Exposed state.
    ColorGroup _exposed;
    //! Pressed state.
    ColorGroup _pressed;
    //! Disabled state.
    ColorGroup _disabled;

    //! Window background colour.
    Color _baseTop;
    //! Alternative window background colour; used with gradients.
    Color _baseBottom;

    //! Used for drawing text on widgets with no background.
    Color _text;
    //! Used for drawing disabled text on widgets with no background.
    Color _textDisabled;

    //! Primary colour for drawing focus rectangles.
    Color _focusTop;
    //! Alternative colour for drawing focus rectangles; use with gradients.
    Color _focusMid;
    //! Alternative colour for drawing focus rectangles; use with gradients.
    Color _focusBottom;
  };
}

#endif /* ILIXI_PALETTE_H_ */
