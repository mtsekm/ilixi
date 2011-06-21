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

#ifndef ILIXI_FONTMETRICS_H_
#define ILIXI_FONTMETRICS_H_

#include "types/Font.h"
#include "types/Size.h"

namespace ilixi
{
  namespace FontMetrics
  {
    /*!
     * Calculates the size of text given a font.
     * @param font
     * @param text
     * @param singleParagraph if true ignores newlines.
     * @return
     */
    Size
    getSize(const Font* font, std::string text, bool singleParagraph = false);

    /*!
     * Calculates the height of text given its width.
     * @param font
     * @param text
     * @param width
     * @param singleParagraph if true ignores newlines.
     */
    int
    heightForWidth(const Font* font, std::string text, int width,
        bool singleParagraph = false);
  }
}

#endif /* ILIXI_FONTMETRICS_H_ */
