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

#ifndef ILIXI_ICON_H_
#define ILIXI_ICON_H_

#include "ui/Widget.h"
#include "types/Image.h"

namespace ilixi
{

  class Icon : public Widget
  {
  public:
    Icon(std::string path, Widget* parent = 0);

    Icon(Image* image, Widget* parent = 0);

    virtual
    ~Icon();

    virtual Size
    preferredSize() const;

    bool
    border() const;

    bool
    fillBackground() const;

    Image*
    image() const;

    void
    setBorder(bool border);

    void
    setFillBackground(bool fill);

  private:
    bool _border;bool _fillBackground;bool _ownImage;

    Image* _image;

    void
    compose(const Rectangle& rect);
  };

}

#endif /* ILIXI_ICON_H_ */
