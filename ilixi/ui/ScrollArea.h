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

#ifndef ILIXI_SCROLLAREA_H_
#define ILIXI_SCROLLAREA_H_

#include "ui/Frame.h"
#include "ui/ScrollBar.h"

namespace ilixi
{
  class ScrollArea : public Frame
  {
  public:
    enum SliderMode
    {
      Auto, AlwaysVisible, Invisible
    };

    ScrollArea(Widget* parent = 0);

    virtual
    ~ScrollArea();

    SliderMode
    getHSliderMode() const;

    SliderMode
    getVSliderMode() const;

    /*!
     * Returns layout's horizontal constraint mode.
     */
    virtual WidgetResizeConstraint
    hConstraint() const;

    /*!
     * Returns layout's vertical constraint mode.
     */
    virtual WidgetResizeConstraint
    vConstraint() const;

    void
    setHSliderMode(SliderMode hMode);

    void
    setVSliderMode(SliderMode vMode);

    /*!
     * Sets a new layout and destroys old one.
     *
     * Warning: Widgets added to previous layout will be destroyed implicitly.
     *
     * @param layout
     */
    void
    setLayout(LayoutBase* layout);

    virtual void
    paint(const Rectangle& targetArea);

  private:
    int _widgetOffsetX;
    int _widgetOffsetY;

    int _frameWidth;
    int _frameHeight;

    SliderMode _hSliderMode;
    SliderMode _vSliderMode;

    ScrollBar* _hSlider;
    ScrollBar* _vSlider;

    Size _layoutSize;

    bool _layoutModified;

    void
    slideHorizontal(int value);

    void
    slideVertical(int value);

    /*!
     * Returns frame's canvas height. Margins are taken into account.
     */
    virtual int
    canvasHeight() const;

    /*!
     * Returns frame's canvas width. Margins are taken into account.
     */
    virtual int
    canvasWidth() const;

    /*!
     * This method sets ScrollArea's layout geometry. Layout is positioned at 0,0 and its width and height are
     * set to width and height of container respectively.
     */
    virtual void
    updateLayoutGeometry();

    /*!
     * Draws the frame border around layout.
     *
     * @param rect
     */
    virtual void
    compose(const Rectangle& rect);

    /*!
     * Blits layout's surface onto scroll area's surface and flips to make changes visible.
     */
    void
    blitLayout();
  };
}

#endif /* ILIXI_SCROLLAREA_H_ */
