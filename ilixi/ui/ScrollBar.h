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

#ifndef ILIXI_SCROLLBAR_H_
#define ILIXI_SCROLLBAR_H_

#include "core/Thread.h"
#include "ui/SliderBase.h"
#include "ui/Button.h"

namespace ilixi
{
  class ScrollBar;
  class DirectionalButton;

  class ScrollBarThread : public Thread
  {
  public:
    ScrollBarThread(ScrollBar* parent);

    ~ScrollBarThread();

    int
    run();

    void
    setSlideAmount(int amount);

  private:
    ScrollBar* _parent;
    int _amount;
  };

  class ScrollBar : public SliderBase
  {
  public:
    ScrollBar(Widget* parent = 0);

    virtual
    ~ScrollBar();

    virtual Size
    preferredSize() const;

    //    virtual void
    //    setInverted(bool inverted);
    //
    //    virtual void
    //    setOrientation(Orientation orientation);

  private:
    ScrollBarThread _thread;
    DirectionalButton* button1;
    DirectionalButton* button2;

    void
    startSlide(int direction);

    void
    stopSlide();

    void
    compose(const Rectangle& rect);

    virtual void
    updateFrameGeometry();

    virtual void
    updateChildrenGeometry();

    void
    disableButtons(int value);
  };
}

#endif /* ILIXI_SCROLLBAR_H_ */
