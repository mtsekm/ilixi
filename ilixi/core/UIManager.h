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
#ifndef ILIXI_FOCUSMANAGER_H_
#define ILIXI_FOCUSMANAGER_H_

#include <list>
#include "types/Event.h"

namespace ilixi
{
  // forward declerations
  class Widget;
  class Window;

  //! Keeps track of focused and exposed widgets.
  /*!
   * This class is mainly used by window based widgets in order to forward key inputs inside application's main loop.
   * Each widget that is placed inside a container within the window is added to an internal list.
   * This list is reconstructed every time window becomes visible. In addition, widget's use UIManager to create their
   * surfaces, i.e. a sub-surface of root window.
   */
  class UIManager
  {
  public:
    /*!
     * Constructor.
     *
     * @param root
     */
    UIManager(Window* root);

    /*!
     * Destructor.
     */
    ~UIManager();

    /*!
     * Returns window which created this instance.
     */
    Window*
    root();

    /*!
     * Returns currently exposed widget inside window.
     */
    Widget*
    exposedWidget() const;

    /*!
     * Returns focused widget inside window.
     */
    Widget*
    focusedWidget() const;

    /*!
     * Returns grabbed widget if any.
     */
    Widget*
    grabbedWidget() const;

    /*!
     * Returns widget which is the target of OSK app.
     */
    Widget*
    oskWidget() const;

    /*!
     * Clears internal list of widgets and sets focused and exposed widgets to NULL.
     */
    void
    reset();

    /*!
     * Adds widget to internal list.
     */
    void
    addWidget(Widget* widget);

    /*!
     * Removes widget from internal list.
     */
    void
    removeWidget(Widget* widget);

    /*!
     * Sets widgets exposed flag and executes onEnter/onLeave methods of widgets.
     *
     * @param widget becomes exposed.
     * @param pointerEvent usually an event of type PointerMotion.
     * @return true if successful.
     */
    bool
    setExposedWidget(Widget* widget, const PointerEvent& pointerEvent);

    /*!
     * Sets widgets focused flag and executes focusInEvent/focusOutEvent methods of widgets.
     * Shows OSK app, if widget has OSKInput flag set.
     *
     * @param widget becomes focused.
     * @return true if successful.
     */
    bool
    setFocusedWidget(Widget* widget);

    /*!
     * Sets grabbed widget. If there is a grabbed widget PointerMotion events are passed to this
     * widget directly inside the main application loop.
     *
     * @param widget grabs pointer events.
     * @return true if successful.
     */
    bool
    setGrabbedWidget(Widget* widget);

    /*!
     * Sets the target of OSK app.
     *
     * @param widget becomes the target of OSK app.
     * @return true if successful.
     */
    bool
    setOSKWidget(Widget* widget);

    /*!
     * Iterates internal list and selects next widget which is able to accept focus.
     * <b> You should always use default arguments. </b>
     *
     * @param found
     * @param iter
     */
    void
    selectNext(bool found = false, bool iter = false);

    /*!
     * Iterates internal list and selects previous widget which is able to accept focus.
     * <b> You should always use default arguments. </b>
     *
     * @param found
     * @param iter
     */
    void
    selectPrevious(bool found = false, bool iter = false);

    void
    setOSKWidgetText(char* text);

  private:
    typedef std::list<Widget*> widgetList;
    typedef widgetList::iterator widgetListIterator;
    typedef widgetList::reverse_iterator widgetListReverseIterator;

    //! Points to owner.
    Window* _rootWindow;
    //! Points to currently focused widget.
    Widget* _focusedWidget;
    //! Points to currently exposed widget.
    Widget* _exposedWidget;
    //! Points to currently grabbed widget.
    Widget* _grabbedWidget;
    //! Widget that requested osk input.
    Widget* _oskWidget;
    //! This vector holds widgets inside the layout.
    widgetList widgets;
  };
}

#endif /* ILIXI_FOCUSMANAGER_H_ */
