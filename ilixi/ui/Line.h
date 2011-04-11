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

#ifndef ILIXI_LINE_H_
#define ILIXI_LINE_H_

#include "ui/Widget.h"

namespace ilixi
{

  class Line : public Widget
  {
  public:
    enum Orientation
    {
      Vertical, Horizontal
    };

    Line(Orientation orientation = Horizontal, Widget* parent = 0);

    virtual
    ~Line();

    virtual Size
    preferredSize() const;

    Orientation
    orientation() const;

    void
    setOrientation(Orientation orientation);

  protected:
    Orientation _orientation;

    virtual void
    compose(const Rectangle& rect);
  };

}

#endif /* ILIXI_LINE_H_ */
