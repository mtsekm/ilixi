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

#ifndef ILIXI_TEXTLAYOUT_H_
#define ILIXI_TEXTLAYOUT_H_

#include "ui/Widget.h"
#include "types/Font.h"
#include <string>

namespace ilixi
{
  //! Base class for widgets which render text.
  /*!
   * This class provides methods for manipulating a PangoLayout and its
   * PangoContext. It is mainly used by widgets which have their text positioned and rendered in a
   * specific way regularly, for example Label or PushButton.
   *
   * Instead of relying solely on Painter for text rendering, speed and ease of use is achieved by
   * working on own Pango interfaces.
   */
  class TextLayout : virtual public Widget
  {
  public:
    /*!
     * Constructor.
     *
     * @param parent
     * @return
     */
    TextLayout(std::string text, Widget* parent = 0);

    /*!
     * Destructor.
     */
    virtual
    ~TextLayout();

    /*!
     * Returns ellipsization mode for text inside widget's text layout.
     */
    PangoEllipsizeMode
    ellipsizeMode() const;

    /*!
     * Returns the font used by this widget.
     */
    const Font*
    font() const;

    /*!
     * Returns the family of widget's font.
     */
    std::string
    fontFamily() const;

    /*!
     * Returns the size of widget's font in points.
     */
    int
    fontSize() const;

    /*!
     * Returns the width in pixels to indent each paragraph.
     * \sa setIndent()
     */
    int
    indent() const;

    /*!
     * Returns orientation of glyphs.
     * \sa setGravity()
     */
    PangoGravity
    gravity() const;

    /*!
     * Returns true if text is justified, false otherwise.
     */
    bool
    justify() const;

    /*!
     * Returns the horizontal alignment of text.
     */
    PangoAlignment
    alignment() const;

    /*!
     * Returns an enumaration specifying the various strecth types possible for the font.
     */
    PangoStretch
    strecth() const;

    /*!
     * Returns an enumeration specifying the various slant styles possible for the font.
     */
    PangoStyle
    style() const;

    /*!
     * Returns the text to be displayed.
     */
    std::string
    text() const;

    /*!
     * Returns an enumeration specifying capitalization variant of the font.
     */
    PangoVariant
    variant() const;

    /*!
     * Returns an enumeration specifying the weight (boldness) of the font.
     */
    PangoWeight
    weight() const;

    /*!
     * Gets the wrap mode for the layout.
     */
    PangoWrapMode
    wrapMode() const;

    /*!
     * Returns the dimensions of text layout in pixels.
     */
    Size
    textExtents() const;

    /*!
     * Returns the height of the text layout given its width.
     *
     * @param width in pixels
     */
    int
    textHeightForWidth(int width) const;

    PangoLayout*
    layout() const;

    Point
    layoutPosition() const;

    bool
    singleParagraph() const;

    /*!
     * Append text to layout.
     *
     * @param text
     */
    void
    append(std::string text);

    /*!
     * Sets text ellipsization mode of text inside widget's text layout.
     *
     * @param ellipsization
     */
    void
    setEllipsizeMode(PangoEllipsizeMode ellipsization);

    /*!
     * Sets the family of the font.
     *
     * @param fontFamily Family of font as a string, e.g. "sans" or "monospace"
     */
    void
    setFontFamily(std::string fontFamily);

    /*!
     * Sets the size of the font in points.
     *
     * @param fontSize Size of font in points.
     */
    void
    setFontSize(int fontSize);

    /*!
     * Sets the width in pixels to indent each paragraph. A negative value of
     * indent will produce a hanging indentation. That is, the first line will
     * have the full width, and subsequent lines will be indented by the absolute
     * value of indent.
     *
     * @param indent The amount by which to indent.
     */
    void
    setIndent(int indent);

    /*!
     * Sets orientation of glyphs.
     *
     * @param gravity
     */
    void
    setGravity(PangoGravity gravity);

    /*!
     * Sets the amount of spacing in Pango unit between the lines of the layout.
     *
     * @param spacing the amount of spacing
     */
    void
    setSpacing(int spacing);

    /*!
     * Sets whether each complete line should be stretched to fill the
     * entire width of the widget's text layout.
     *
     * @param justify Whether the lines should be justified.
     */
    void
    setJustify(bool justify);

    /*!
     * Sets the horizontal alignment of widget's text layout.
     *
     * @param alignment Horizontal alignment.
     */
    void
    setHorizontalAlignment(PangoAlignment alignment);

    /*!
     * Sets the  strecth type for the font.
     *
     * @param stretch
     */
    void
    setStrecth(PangoStretch stretch);

    /*!
     * Sets the slant style for the font.
     *
     * @param style
     */
    void
    setStyle(PangoStyle style);

    /*!
     * Sets the text of PangoLayout.
     *
     * @param text Text to display.
     */
    virtual void
    setText(const std::string &text);

    /*!
     * Sets the text of PangoLayout using markup.
     *
     * @param markup
     */
    void
    setMarkup(const std::string &markup);

    /*!
     * Sets the variant for the font.
     *
     * @param variant
     */
    void
    setVariant(PangoVariant variant);

    /*!
     * Sets the weight for the font.
     *
     * @param weight
     */
    void
    setWeight(PangoWeight weight);

    /*!
     * Sets the wrap mode.
     *
     * @param wrap the wrap mode
     */
    void
    setWrapMode(PangoWrapMode wrap);

  protected:
    //! This property the holds the font for the widget.
    Font* _font;
    //! Pointer to pango context.
    PangoContext* _context;
    //! Pointer to pango layout.
    PangoLayout* _layout;
    //! This property holds text to display inside widget's text layout.
    std::string _text;
    //! This property stores the position of pango layout and it is used for drawing text.
    Point _layoutPosition;

    /*!
     * This method is called if widget's geometry is modified.
     * It should be used for setting the specific geometry of pango layout.
     *
     * You should reimplement this method in your class.
     */
    virtual void
    updateTextLayoutGeometry();

    int
    textLayoutWidth() const;

    int
    textLayoutHeight() const;

    /*!
     * Sets the position and size of the pango layout.
     *
     * @param x Pango layout x-coordinate.
     * @param y Pango layout y-coordinate.
     * @param width in pixels.
     * @param height in pixels.
     */
    void
    setTextGeometry(int x, int y, int width, int height);

    /*!
     * Sets whether text layout show new lines as paragraph seperators. If true,
     * text is displayed in a single line. By default this is set to false.
     *
     * @param single if false allow multiple lines.
     */
    void
    setSingleParagraph(bool single);

  private:
    void
    initFont();

    virtual Font*
    defaultFont() const;
  };
}

#endif /* ILIXI_TEXTLAYOUT_H_ */
