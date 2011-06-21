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

#ifndef ILIXI_BUTTON_H_
#define ILIXI_BUTTON_H_

#include "ui/TextLayout.h"
#include "types/Image.h"

namespace ilixi
{
  //! Base class for buttons.
  class Button : public TextLayout
  {
  public:
    /*!
     * Constructor.
     *
     * @param text
     * @param parent
     */
    Button(std::string text, Widget* parent = 0);

    /*!
     * Destructor.
     */
    virtual
    ~Button();

    /*!
     * Returns true if button is checkable.
     */
    bool
    checkable() const;

    /*!
     * If button is checkable and checked, returns true.
     */
    bool
    checked() const;

    /*!
     * Returns icon's size.
     * \sa setIcon(), setIconSize()
     */
    Size
    getIconSize() const;

    /*!
     * Returns button's image, if any.
     */
    Image*
    icon() const;

    Point
    iconPosition() const;

    /*!
     * Returns true if button has an image.
     */
    bool
    hasIcon() const;

    /*!
     * Simulates a pointer click over button.  Signals (pressed and clicked) are emitted.
     *
     * @param ms If this value is positive click action is animated for given milliseconds.
     */
    void
    click(unsigned int ms = 0);

    /*!
     * Changes button's checked state.
     */
    virtual void
    toggleState();

    /*!
     * Set button's checkable behaviour.
     *
     * @param checkable True if button should be checkable.
     */
    void
    setCheckable(bool checkable);

    /*!
     * Sets button's checked state only if button is checkable.
     *
     * @param checked True if button should appear check.
     */
    void
    setChecked(bool checked);

    /*!
     * Sets button's icon. This function loads an image file to memory and stores it until button is destroyed.
     *
     * @param iconPath Path to an image file.
     * @param size Image is strected to this size.
     */
    void
    setIcon(const std::string& iconPath, const Size& size = Size(24, 24));

    /*!
     * This signal is emitted when button is pressed and then released.
     *
     * For example, if you need to connect this signal to some other button's click()
     * you could write:
     * \code
     * #include <sigc/sigc++>
     * ...
     * Button button1 = new Button("Button 1");
     * ...
     * Button button2 = new Button("Button 2");
     * button2->sigClicked.connect(sigc::hide(sigc::mem_fun(button1, &Button::click)));
     * \endcode
     *
     * Please refer to <A HREF="http://libsigc.sourceforge.net/doc.shtml" target="_blank">libsigc++ documentation</A>  for more information on usage.
     */
    sigc::signal<void> sigClicked;

    /*!
     * This signal is emitted when button is pressed down.
     *
     * \sa sigClicked
     */
    sigc::signal<void> sigPressed;

    /*!
     * This signal is emitted when button is released.
     *
     * \sa sigClicked
     */
    sigc::signal<void> sigReleased;

    sigc::signal<void, bool> sigCheckChanged;

  protected:
    //! This property holds whether a button is checkable.
    bool _checkable;
    //! This property holds button's checked state.
    bool _checked;
    //! This property is used for emitting clicked signals.
    bool _doClick;
    //! This property holds button's icon.
    Image* _icon;
    //! This property stores the local position of the icon.
    Point _iconPosition;

    virtual void
    keyUpEvent(const KeyEvent& event);

    virtual void
    pointerButtonDownEvent(const PointerEvent& event);

    virtual void
    pointerButtonUpEvent(const PointerEvent& event);

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

  private:
    virtual Font*
    defaultFont() const;
  };

}

#endif /* ILIXI_BUTTON_H_ */
