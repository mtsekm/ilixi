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

#ifndef ILIXI_VBOXLAYOUT_H_
#define ILIXI_VBOXLAYOUT_H_

#include "ui/LayoutBase.h"

namespace ilixi
{
  //! Vertical layout
  /*!
   * In this layout widgets are aligned vertically on a single line. Default spacing is 5 pixels.
   */
  class VBoxLayout : public LayoutBase
  {
  public:
    VBoxLayout(Widget* parent = 0);

    virtual
    ~VBoxLayout();

    virtual int
    heightForWidth(int width) const;

    virtual Size
    preferredSize() const;

    void
    tile();

  private:
    virtual void
    updateActiveWidgetList();
  };

}

#endif /* ILIXI_VBOXLAYOUT_H_ */
