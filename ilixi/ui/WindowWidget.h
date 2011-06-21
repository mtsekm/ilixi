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

#ifndef ILIXI_WINDOWWIDGET_H_
#define ILIXI_WINDOWWIDGET_H_

#include "core/Window.h"
#include "ui/Frame.h"
#include <vector>
#include "types/Rectangle.h"
#include <semaphore.h>

namespace ilixi
{
  //! Base class for windowed widgets.
  class WindowWidget : virtual public Window, public Frame
  {
  public:
    /*!
     * Constructor.
     */
    WindowWidget(Widget* parent = 0);

    /*!
     * Destructor.
     */
    virtual
    ~WindowWidget();

    /*!
     * This method executes update().
     */
    virtual void
    doLayout();

    virtual void
    paint(const Rectangle& targetArea);

    virtual void
    repaint(const Rectangle& rect);

  protected:
    /*!
     * Handles incoming input events if window has focus.
     *
     * @param event
     * @return true if event is consumed.
     */
    bool
    handleWindowEvent(const DFBWindowEvent& event);

  private:
    //! Stores window's dirty regions and a region for update.
    struct
    {
      pthread_mutex_t _listLock;
      sem_t _updateReady;
      sem_t _paintReady;
      Rectangle _updateRegion;
      std::vector<Rectangle> _updateQueue;
    } _updates;

    //! Updates dirty regions inside window.
    virtual void
    updateWindow();

  };

}

#endif /* ILIXI_WINDOWWIDGET_H_ */
