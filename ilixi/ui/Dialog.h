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

#ifndef ILIXI_DIALOG_H_
#define ILIXI_DIALOG_H_

#include "core/Window.h"
#include "ui/Frame.h"
#include "ui/HBoxLayout.h"

namespace ilixi
{
  class Dialog : virtual public Window, public Frame
  {
  public:
    //! This enum designates the buttons shown on the dialog.
    enum ButtonOption
    {
      NoButtonOption, //!< No buttons.
      OKButtonOption, //!< OK button.
      CancelButtonOption, //!< Cancel button.
      OKCancelButtonOption,
      YesNoButtonOption, //!< Yes and No buttons.
      YesNoCancelButtonOption
    //!< Yes, No and Cancel buttons.
    };

    /*!
     * Constructor.
     *
     * @param parent
     */
    Dialog(std::string title, ButtonOption option = OKButtonOption,
        Widget* parent = 0);

    /*!
     * Destructor.
     */
    virtual
    ~Dialog();

    /*!
     * Returns a height for frame if its contents are fit inside width.
     *
     * @param width Desired width of frame.
     */
    virtual int
    heightForWidth(int width) const;

    /*!
     * Calculates the preferred size of contents and returns an updated size using canvas, margins and borders.
     */
    virtual Size
    preferredSize() const;

    /*!
     * Closes the dialog and sets the result to accepted (1).
     */
    void
    accept();

    /*!
     * Closes the dialog and sets the result to rejected (0).
     */
    void
    reject();

    /*!
     * Closes the dialog and sets the result to res.
     */
    void
    finish(int res);

    /*!
     * Returns result of this dialog.
     */
    int
    result() const;

    /*!
     * Shows the dialog and blocks until it is accepted (1), rejected (0) or invalid (-1).
     */
    void
    execute();

    /*!
     * Returns the dialog's title.
     */
    std::string
    title() const;

    Size
    buttonLayoutSize() const;

    /*!
     * Returns height of the dialog's title bar.
     */
    Size
    titleSize() const;

    void
    setButtonLayoutOption(ButtonOption option);

    void
    setButtonLayout(LayoutBase* buttonLayout);

    /*!
     * Sets dialog's title.
     *
     * @param title
     */
    void
    setTitle(const std::string& title);

    /*!
     * Paints dialog and its contents.
     *
     * @param targetArea
     * @param forceRepaint
     */
    virtual void
    paint(const Rectangle& targetArea, bool forceRepaint = false);

    /*!
     * This method executes update().
     */
    virtual void
    doLayout();

    /*!
     * This signal is emitted when the dialog is accepted.
     */
    sigc::signal<void> sigAccepted;

    /*!
     * This signal is emitted when the dialog is rejected.
     */
    sigc::signal<void> sigRejected;

    /*!
     * This signal is emitted when the dialog's result is set.
     */
    sigc::signal<void, int> sigFinished;

  protected:
    //! This layout is placed under the dialog and usually contains buttons.
    LayoutBase* _buttonLayout;
    //! This property stores the dialog's result.
    int _result;
    //! This property stores the dialogs's title text.
    std::string _title;
    //! This property stores the dimensions of the title bar text.
    Size _titleSize;
    //! This property stores the dimensions of the button layout.
    Size _buttonLayoutSize;

  private:
    /*!
     * Returns Dialog's canvas y-coordinate including the top margin.
     */
    virtual int
    canvasY() const;

    /*!
     * Returns frame's canvas height excluding margins.
     */
    virtual int
    canvasHeight() const;

    //! Draws dialog frame using designer.
    virtual void
    compose(const Rectangle& rect);

    //! Transfers incoming input events to child widgets.
    bool
    handleWindowEvent(const DFBWindowEvent& event);

    void
    closeDialog();

    void
    updateButtonLayoutGeometry();
  };
}

#endif /* ILIXI_DIALOG_H_ */
