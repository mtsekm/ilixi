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

#ifndef ILIXI_SLIDER_H_
#define ILIXI_SLIDER_H_

#include "ui/SliderBase.h"

namespace ilixi
{
  class Slider : public SliderBase
  {
  public:
    Slider(Widget* parent = 0);

    virtual
    ~Slider();

    virtual Size
    preferredSize() const;

  private:

    void
    compose(const Rectangle& rect);

    virtual void
    updateFrameGeometry();
  };
}

#endif /* ILIXI_SLIDER_H_ */
