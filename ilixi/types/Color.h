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

#ifndef ILIXI_COLOR_H_
#define ILIXI_COLOR_H_

namespace ilixi
{
  //! Defines a color with ARGB components.
  class Color
  {
  public:
    /*!
     * Creates a black color.
     */
    Color();

    /*!
     * Copy constructor.
     */
    Color(const Color& color);

    /*!
     * Creates a color using given values.
     * @param red [0-1]
     * @param green [0-1]
     * @param blue [0-1]
     * @param alpha [0-1]
     */
    Color(double red, double green, double blue, double alpha = 1.0);

    /*!
     * Destructor.
     */
    ~Color();

    /*!
     * Returns color alpha value.
     */
    double
    alpha() const;

    /*!
     * Returns color blue value.
     */
    double
    blue() const;

    /*!
     * Returns color green value.
     */
    double
    green() const;

    /*!
     * Returns color red value.
     */
    double
    red() const;

    /*!
     * Set color alpha value.
     * @param alpha [0-1]
     */
    void
    setAlpha(double alpha);

    /*!
     * Set color blue value.
     * @param blue [0-1]
     */
    void
    setBlue(double blue);

    /*!
     * Set color green value.
     * @param green [0-1]
     */
    void
    setGreen(double green);

    /*!
     * Set color red value.
     * @param red [0-1]
     */
    void
    setRed(double red);

    /*!
     * Assigns a copy of color to this color, and returns a reference to it.
     */
    Color&
    operator=(const Color & color);

    /*!
     * Substracts given value from RGB values.
     */
    Color&
    operator-(const double i);

    /*!
     * Adds given value to RGB values.
     */
    Color&
    operator+(const double i);

    /*!
     * Returns true if this color has the same ARGB values as c; otherwise returns false.
     */
    bool
    operator==(const Color & c) const;

    /*!
     * Returns true if this color has a different ARGB values from c; otherwise returns false.
     */
    bool
    operator!=(const Color & c) const;

  private:
    //! Red component
    double _red;
    //! Green component
    double _green;
    //! Blue component
    double _blue;
    //! Alpha component
    double _alpha;
  };
}

#endif /* ILIXI_COLOR_H_ */
