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

#ifndef ILIXI_FONT_H_
#define ILIXI_FONT_H_

#include <string>
#include <pango/pangocairo.h>

namespace ilixi
{
  //! Specifies a font for drawing text.
  /*!
   * Font class is used for creating and modifying Pango FontDescriptions.
   */
  class Font
  {
    friend class Painter;

  public:
    /*!
     * Creates a font with a default description.
     */
    Font();

    /*!
     * Creates a new font description from a string representation
     * in the form "[FAMILY-LIST] [STYLE-OPTIONS] [SIZE]", where
     * FAMILY-LIST is a comma separated list of families optionally
     * terminated by a comma, STYLE_OPTIONS is a whitespace separated
     * list of words where each WORD describes one of style, variant,
     * weight, stretch, or gravity, and SIZE is a decimal number
     * (size in points) or optionally followed by the unit modifier
     * "px" for absolute size. Any one of the options may be absent.
     * If FAMILY-LIST is absent, then the family_name field of the
     * resulting font description will be initialized to NULL. If
     * STYLE-OPTIONS is missing, then all style options will be set
     * to the default values. If SIZE is missing, the size in the
     * resulting font description will be set to 0.
     *
     * @param fontDescription Representation of a font description.
     */
    Font(const std::string& fontDescription);

    /*!
     * Copy constructor.
     */
    Font(const Font& font);

    /*!
     * Creates a new font by copying given description.
     */
    Font(const PangoFontDescription* desc);

    /*!
     * Destructor.
     */
    ~Font();

    /*!
     * Returns font description.
     */
    PangoFontDescription*
    fontDescription() const;

    /*!
     * Returns font family name.
     */
    const char*
    family() const;

    /*!
     * Returns font gravity.
     */
    PangoGravity
    gravity() const;

    /*!
     * Returns font size.
     */
    int
    size() const;

    /*!
     * Returns font strecth mode.
     */
    PangoStretch
    strecth() const;

    /*!
     * Returns font style.
     */
    PangoStyle
    style() const;

    /*!
     * Returns font variant.
     */
    PangoVariant
    variant() const;

    /*!
     * Returns font weight.
     */
    PangoWeight
    weight() const;

    /*!
     * Set font family.
     */
    void
    setFamily(std::string family);

    /*!
     * Set font gravity.
     */
    void
    setGravity(PangoGravity gravity);

    /*!
     * Set font size.
     */
    void
    setSize(int size);

    /*!
     * Set font stretch.
     */
    void
    setStretch(PangoStretch stretch);

    /*!
     * Set font style.
     */
    void
    setStyle(PangoStyle style);

    /*!
     * Set font variant.
     */
    void
    setVariant(PangoVariant variant);

    /*!
     * Set font weight.
     */
    void
    setWeight(PangoWeight weight);

    /*!
     * Returns font description as a string.
     */
    char*
    toString();

    /*!
     * Assigns f to this font and returns a reference to it.
     */
    Font&
    operator=(const Font& f);

    /*!
     * Returns true if this font is equal to f; otherwise returns false.
     */
    bool
    operator==(const Font &f);

    /*!
     * Returns true if this font is different from f; otherwise returns false.
     */
    bool
    operator!=(const Font &f);

  private:
    //! Pointer to pango font description.
    PangoFontDescription* _desc;
    //! This flag is set to true if pen is edited after it was applied to the context.
    bool _changed;

    //! Applies the pen to the pango layout.
    void
    applyFont(PangoLayout* pangoLayout);
  };
}

#endif /* ILIXI_FONT_H_ */
