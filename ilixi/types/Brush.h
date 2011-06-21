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

#ifndef ILIXI_BRUSH_H_
#define ILIXI_BRUSH_H_

#include "types/Gradient.h"

//TODO: cairo_fill_rule_t The fill rule is used to determine which regions are inside or outside a complex path.

namespace ilixi
{
  //! Defines color and gradient used for filling inside primitive shapes.
  /*!
   * Brush is used by painter's context during drawing. It has a color, gradient and texture.
   * By default, a brush only applies a solid color.
   */
  class Brush
  {
    friend class Painter;

  public:

    /*!
     * Available brush modes.
     */
    enum BrushMode
    {
      None, //!< Shapes are not filled.
      SolidColorMode, //!< Shapes are filled using a solid color.
      GradientMode, //!< Shapes are filled using a gradient
      TextureMode
    //!< Shapes are filled using a texture (Not yet implemented!)
    };

    /*!
     * Creates a solid white colored brush.
     */
    Brush();

    /*!
     * Copy constructor.
     */
    Brush(const Brush& brush);

    /*!
     * Creates a brush using the given color.
     *
     * @param color Solid brush color.
     */
    Brush(const Color& color);

    /*!
     * Destructor.
     */
    ~Brush();

    /*!
     * Returns brush mode. Default is SolidColorMode.
     */
    BrushMode
    mode() const;

    /*!
     * Returns current solid color.
     */
    Color
    getColor() const;

    /*!
     * Returns current gradient.
     *
     * \sa Gradient::getType()
     */
    Gradient
    getGradient() const;

    /*!
     * Sets the brush's mode.
     * If there is no gradient available for the brush, mode falls back to SolidColor.
     *
     * @param mode Brush mode.
     */
    void
    setBrushMode(BrushMode mode);

    /*!
     * Sets the brush color to the given color.
     *
     * @param color New color.
     */
    void
    setColor(const Color& color);

    /*!
     * The given gradient is assigned to the brush's current gradient.
     * Reference to previous gradient (cairo_pattern_t) is decremented by 1 internally.
     *
     * \code
     * Brush b;
     * RadialGradient rGrad(50, 50, 10, 50, 50, 100);
     * rGrad->addStop(Color(0, 0, 0), 0.0);
     * rGrad->addStop(Color(0, 1, 0), 1);
     * b.setGradient(rGrad);
     * \endcode
     *
     * @param gradient New gradient.
     */
    void
    setGradient(const Gradient& gradient);

    /*!
     * Assignment operator.
     */
    Brush&
    operator=(const Brush &brush);

  private:
    //! This property holds current brush mode.
    BrushMode _mode;
    //! This property holds current brush color.
    Color _color;
    //! This property holds gradient used by the brush.
    Gradient _gradient;
    // TODO: Textures...
    //Texture* texture;

    //! This flag is set to true if brush is edited after it was applied to the context.
    bool _changed;

    //! Applies the brush to the cairo context.
    void
    applyBrush(cairo_t* context);
  };
}

#endif /* ILIXI_BRUSH_H_ */
