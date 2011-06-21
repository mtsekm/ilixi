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

#ifndef ILIXI_SLIDERBASE_H_
#define ILIXI_SLIDERBASE_H_

#include "ui/Widget.h"
#include <sigc++/signal.h>

namespace ilixi
{
  class SliderBase : public Widget
  {
  public:
    /*!
     * Constructor.
     * @param parent
     */
    SliderBase(Widget* parent = 0);

    /*!
     * Destructor.
     */
    virtual
    ~SliderBase();

    /*!
     * Returns the bounding rectangle of indicator in absolute coordinates.
     */
    Rectangle
    indicatorGeometry() const;

    Rectangle
    indicatorRegion() const;

    /*!
     * Returns true if slider is inverted.
     */
    bool
    inverted() const;

    /*!
     * Returns slider's maximum value limit.
     */
    int
    maximum() const;

    /*!
     * Returns slider's minimum value limit.
     */
    int
    minimum() const;

    /*!
     * Returns slider's orientation.
     */
    Orientation
    orientation() const;

    /*!
     * Returns slider's step amount.
     */
    int
    step() const;

    /*!
     * Returns slider's page step amount.
     */
    int
    pageStep() const;

    /*!
     * Returns the difference between maximum and minimum limits.
     */
    int
    range() const;

    /*!
     * Returns slider's value.
     */
    int
    value() const;

    /*!
     * Sets slider's inverted property. Only vertical slider's act inverted.
     *
     * If inverted is true and orientation is vertical,
     * slider's maximum value will be at bottom instead of top.
     *
     * @param inverted
     */
    virtual void
    setInverted(bool inverted);

    /*!
     * Sets slider's maximum limit.
     * @param maximum
     */
    void
    setMaximum(int maximum);

    /*!
     * Sets slider's minimum limit.
     * @param minimum
     */
    void
    setMinimum(int minimum);

    /*!
     * Sets slider's orientation.
     *
     * @param orientation Horizontal or Vertical
     */
    virtual void
    setOrientation(Orientation orientation);

    /*!
     * Sets slider's minimum and maximum limits.
     * @param minimum
     * @param maximum
     */
    void
    setRange(int minimum, int maximum);

    /*!
     * Sets slider's step amount. This value specifies how much slider is moved when an arrow key is pressed or mouse wheel is rotated.
     * If step is 0 then slider will not respond to arrow keys or mouse wheel.
     * @param step This value is usually smaller than pageStep amount.
     */
    void
    setStep(int step);

    /*!
     *  Sets slider's page step amount.  This value specifies how much slider is moved when a PageUp or PageDown key is pressed.
     *  If pageStep is 0 then slider will not respond to Page keys.
     * @param pageStep This value is usually larger than step amount.
     */
    void
    setPageStep(int pageStep);

    /*!
     * Sets slider's value. Emits sigValueChanged.
     * @param value
     */
    void
    setValue(int value, bool signal = true);

    /*!
     * This signal is emitted while slider is being dragged.
     */
    sigc::signal<void, int> sigMoved;

    /*!
     * This signal is emitted when pointer button is pressed over slider.
     */
    sigc::signal<void> sigPressed;

    /*!
     * This signal is emitted when pointer button is released over slider.
     */
    sigc::signal<void> sigReleased;

    /*!
     * This signal is emitted when pointer is released and slider's value is changed.
     */
    sigc::signal<void, int> sigValueChanged;

  protected:
    /*!
     * This method adds given amount to slider's value.
     *
     * @param amount An amount representing a positive change in slider's value.
     */
    void
    addAmount(int amount);

    /*!
     * This method subtracts given amount from slider's value.
     *
     * @param amount An amount representing a negative change in slider's value.
     */
    void
    subAmount(int amount);

  protected:
    //! This property holds slider's maximum limit.
    int _maximum;
    //! This property holds slider's minimum limit.
    int _minimum;
    //! This property holds the current value of slider.
    int _value;
    //! This property holds the step amount of slider.
    int _step;
    //! This property holds the page step amount of slider.
    int _pageStep;
    //! This property holds the orientation of the progress bar.
    Orientation _orientation;
    //! This property holds the bounding rectangle around indicator.
    Rectangle _indicator;

    Rectangle _indicatorRegion;
    //! This property holds the bounding rectangle of slider's actual fill geometry.
    Rectangle _sRect;
    //! This is the difference between _maximum and _minimum. This is stored in order to simplify calculations.
    int _range;
    //! This flag specifies whether slider is inverted.
    bool _inverted;

    //! Updates indicator geometry.
    void
    updateIndicatorPosition();

    virtual void
    keyUpEvent(const KeyEvent& event);

    virtual void
    pointerButtonDownEvent(const PointerEvent& event);

    virtual void
    pointerButtonUpEvent(const PointerEvent& event);

    virtual void
    pointerMotionEvent(const PointerEvent& event);

    virtual void
    pointerWheelEvent(const PointerEvent& event);

    /*!
     * Just updates widget.
     */
    virtual void
    enterEvent(const PointerEvent& event);

    /*!
     * Just updates widget.
     */
    virtual void
    leaveEvent(const PointerEvent& event);

    /*!
     * Just updates widget.
     */
    virtual void
    focusInEvent();

    /*!
     * Just updates widget.
     */
    virtual void
    focusOutEvent();

  };
}

#endif /* ILIXI_SLIDERBASE_H_ */
