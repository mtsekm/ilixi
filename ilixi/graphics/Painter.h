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

#ifndef ILIXI_PAINTER_H_
#define ILIXI_PAINTER_H_

#include "ui/Widget.h"
#include "types/Brush.h"
#include "types/Pen.h"
#include "types/Font.h"
#include "types/Image.h"

namespace ilixi
{
  //! Draws primitive shapes and renders text.
  /*!
   * Painter paints on a widget's surface using its pen and brush. It is also used
   * for drawing text. For example, following code will draw a circle inside widget's
   * surface using default pen:
   * \code
   * void
   * MyWidget::compose(const Rectangle& rect)
   * {
   *    Painter painter(this);        // Painter should use this widget's surface.
   *    painter.begin();              // Lock widget's surface.
   *    painter.drawEllipse(0, 0, 100, 100);
   *    painter.end();
   * }
   * \endcode
   */
  class Painter
  {
  public:
    /*!
     * This enum is provided for convenience. It simply maps from cairo's implementation.
     */
    enum AntiAliasMode
    {
      AliasDefault,//!< Use the default antialiasing for the subsystem and target device
      AliasNone, //!< Use a bilevel alpha mask
      AliasGray, //!< Perform single-color antialiasing
      AliasSubPixel
    //!< Perform antialiasing by taking advantage of the order of subpixel elements on devices such as LCD panels
    };

    /*!
     * This enum is used to control behaviour of drawing methods, e.g. drawRectangle.
     */
    enum DrawingMode
    {
      StrokePath, //!< Outline shape using pen.
      StrokeAndFill, //!< Outline shape using pen and fill inside using brush.
      FillPath, //!< Fill inside shape using brush.
      ClipPath, //!< Create a clipping path.
      AddPath
    //!< Add path to context.
    };

    /*!
     * Constructor creates a new painter instance for the given widget.
     * Upon creation default pen and font are applied to cairo and pango layouts
     * respectively.
     *
     * @param widget Painter will work on this widget's surface.
     * @param clipRect Clipping is applied to this rectangle.
     */
    Painter(Widget* widget);

    /*!
     * Destructor. Calls end() automatically.
     */
    ~Painter();

    /*!
     * Locks current widget's surface for drawing operations and blocks
     * consecutive calls to begin() until end() is called. You should
     * call this method in order to serialise access to the widget's surface.
     */
    void
    begin(const Rectangle& clipRect);

    /*!
     * Unlocks current widget's surface. This method is automatically
     * called upon destruction of the painter.
     */
    void
    end();

    cairo_t*
    cairoContext() const;

    PangoLayout*
    pangoLayout() const;

    /*!
     * Returns current anti-aliasing setting.
     */
    AntiAliasMode
    getAntiAliasMode() const;

    /*!
     * Returns the position of strong and weak cursors within the pango layout.
     *
     * @param index the byte index of the cursor
     * @param weak location to store weak cursor position
     * @param strong location to store strong cursor position
     */
    void
    getCursorPosition(int index, Rectangle* weak = 0, Rectangle* strong = 0);

    /*!
     * Returns current ellipsization setting.
     */
    PangoEllipsizeMode
    getEllipsizeMode() const;

    /*!
     * Returns current alignment of pango layout.
     */
    PangoAlignment
    getLayoutAlignment() const;

    /*!
     * Returns the amount of indentation in pixels.
     */
    int
    getIndent() const;

    /*!
     * Returns whether the lines are justified.
     */
    bool
    getJustify() const;

    /*!
     * Returns the dimensions of the given text.
     */
    int
    getHeightForWidth(std::string text, int width);

    void
    getPixelExtents(std::string text, int* width, int* height);

    /*!
     * Returns painter's current brush used for filling shapes. This method is useful if you wish to alter
     * current brush without setting a new one. Changes to the brush are automatically applied to context
     * before filling operations.
     *
     * For example, following code will set brush color to white.
     * \code
     * painter.getBrush()->setColor(Color(1, 1, 1));
     * \endcode
     */
    Brush*
    getBrush() const;

    /*!
     * Returns painter's current font used for rendering text. This method is useful if you wish to alter
     * current font without setting a new one. Changes to the font are automatically applied to context
     * before filling operations.
     *
     * For example, following code will set brush color to white.
     * \code
     * painter.getFont()->setFamily("Sans");
     * \endcode
     */
    Font*
    getFont() const;

    /*!
     * Returns painter's current pen for drawing shapes. This method is useful if you wish to alter
     * current pen without setting a new one. Changes to the pen are automatically applied to context
     * before filling operations.
     *
     * For example, following code will set brush color to white.
     * \code
     * painter.getPen()->setColor(Color(1, 1, 1));
     * \endcode
     */
    Pen*
    getPen();

    /*!
     * Draws an arc inside the bounding rectangle defined by the top left point (x, y)
     * and dimensions (width, height) between start and end angles using current pen.
     *
     * Angles are measured in degrees. An angle of 0 is in the direction of the positive X axis (right).
     * An angle of 90 degrees is in the direction of the positive Y axis (down). Angles increase in
     * the direction from the positive X axis toward the positive Y axis (Clockwise).
     *
     * The following code draws a semi-circle.
     * \code
     * painter.drawArc(0, 0, 100, 100, 180, 360);
     * \endcode
     *
     * \image html DrawArc.png "Example code output"
     *
     * @param x X coordinate of bounding rectangle.
     * @param y X coordinate of bounding rectangle.
     * @param width Width of bounding rectangle.
     * @param height Height of bounding rectangle.
     * @param angleStart Start angle in degrees.
     * @param angleEnd End angle in degrees.
     */
    void
    drawArc(double x, double y, double width, double height, double angleStart,
        double angleEnd);

    /*!
     * Draws an arc inside the bounding rectangle between start and end angles using current pen.
     *
     * \sa drawArc(double x, double y, double width, double height, double angle1, double angle2);
     *
     * @param rect Bounding rectangle.
     * @param angleStart Start angle in degrees.
     * @param angleEnd End angle in degrees.
     */
    void
    drawArc(const Rectangle& rect, double angleStart, double angleEnd);

    /*!
     * Draws a chord inside the bounding rectangle defined by top left point (x, y)
     * and dimensions (width, height) between start and end angles using current pen.
     *
     * The following code draws a closed semi-circle with current pen
     * and fills it with current brush.
     * \code
     * painter.getPen()->setColor(Color(0, 0, 0));
     * painter.getBrush()->setColor(Color(0, 1, 0));
     * painter.getBrush()->setBrushMode(Brush::SolidColorMode);
     * painter.drawChord(0, 0, 100, 100, 180, 360, Painter::StrokeAndFill);
     * \endcode
     *
     * \image html DrawChord.png "Example code output"
     *
     * @param x X coordinate of bounding rectangle.
     * @param y X coordinate of bounding rectangle.
     * @param width Width of bounding rectangle.
     * @param height Height of bounding rectangle.
     * @param angleStart Start angle in degrees.
     * @param angleEnd End angle in degrees.
     * @param mode Set painter's drawing mode.
     */
    void
    drawChord(double x, double y, double width, double height,
        double angleStart, double angleEnd, DrawingMode mode = StrokePath);

    /*!
     * Draws a chord inside the bounding rectangle between start and end angles using current pen.
     *
     * @param rect Bounding rectangle.
     * @param angleStart Start angle in degrees.
     * @param angleEnd End angle in degrees.
     * @param mode Set painter's drawing mode.
     */
    void
    drawChord(const Rectangle& rect, double angleStart, double angleEnd,
        DrawingMode mode = StrokePath);

    /*!
     * Draws an ellipse inside the bounding rectangle defined by top left point (x, y)
     * and dimensions (width, height).
     *
     * The following code will draw an ellipse.
     * \code
     * painter.getPen()->setColor(Color(0, 0, 0));
     * painter.getBrush()->setColor(Color(0, 1, 0));
     * painter.getBrush()->setBrushMode(Brush::SolidColorMode);
     * painter.drawEllipse(0, 0, 100, 100, Painter::StrokeAndFill);
     * \endcode
     *
     * \image html DrawEllipse.png "Example code output"
     *
     * @param x X coordinate of bounding rectangle.
     * @param y X coordinate of bounding rectangle.
     * @param width Width of bounding rectangle.
     * @param height Height of bounding rectangle.
     * @param mode Set painter's drawing mode.
     */
    void
    drawEllipse(double x, double y, double width, double height,
        DrawingMode mode = StrokePath);

    /*!
     * Draws an ellipse inside the bounding rectangle.
     *
     * @param rect Bounding rectangle.
     * @param mode Set painter's drawing mode.
     */
    void
    drawEllipse(const Rectangle& rect, DrawingMode mode = StrokePath);

    /*!
     * Draws a line between two points defined by (x1, y1) and (x2, y2).
     * By default AntiAliasing is On and this will effect the appearance of lines.
     *
     * The following code segment will draw a single pixel line.
     * \code
     * painter.getPen()->setLineWidth(1);
     * painter.drawLine(0, 0, 100, 100);
     * \endcode
     *
     * \image html DrawLine.png "Example code output"
     *
     * @param x1 X coordinate of start point.
     * @param y1 Y coordinate of start point.
     * @param x2 X coordinate of end point.
     * @param y2 Y coordinate of end point.
     * @param mode Set painter's drawing mode.
     */
    void
    drawLine(double x1, double y1, double x2, double y2, DrawingMode mode =
        StrokePath);

    /*!
     * Draws a line between two given points p1 and p2.
     *
     * @param p1 Start point.
     * @param p2 End point.
     * @param mode Set painter's drawing mode.
     */
    void
    drawLine(const Point& p1, const Point& p2, DrawingMode mode = StrokePath);

    /*!
     * Draws a line inside the bounding rectangle.
     *
     * @param rect Bounding rectangle.
     * @param mode Set painter's drawing mode.
     */
    void
    drawLine(const Rectangle& rect, DrawingMode mode = StrokePath);

    /*!
     * Draws a closed polygon using given points.
     *
     * The following code draws a triangle.
     * \code
     * Point points[3] = { Point(100, 10), Point(200, 200), Point(10, 200) };
     * painter.drawPolygon(points, 3);
     * \endcode
     *
     * \image html DrawPolygon.png "Example code output"
     *
     * @param points Array of points.
     * @param pointCount Number of points in the array.
     * @param mode Set painter's drawing mode.
     */
    void
    drawPolygon(const Point* points, int pointCount, DrawingMode mode =
        StrokePath);

    /*!
     * Draws a rectangle inside the bounding rectangle defined by top left point (x, y)
     * and (width, height).
     *
     * The following code will draw a filled rectangle.
     * \code
     * painter.getPen()->setColor(Color(0, 0, 0));
     * painter.getBrush()->setColor(Color(0, 1, 0));
     * painter.getBrush()->setBrushMode(Brush::SolidColorMode);
     * painter.drawRectangle(0, 0, 100, 100, Painter::StrokeAndFill);
     * \endcode
     *
     * \image html DrawRectangle.png "Example code output"
     *
     * @param x X coordinate of bounding rectangle.
     * @param y X coordinate of bounding rectangle.
     * @param width Width of bounding rectangle.
     * @param height Height of bounding rectangle.
     * @param mode Set painter's drawing mode.
     */
    void
    drawRectangle(double x, double y, double width, double height,
        DrawingMode mode = StrokePath);

    /*!
     * Draws a rectangle inside the bounding rectangle.
     *
     * @param rect Bounding rectangle.
     * @param mode Set painter's drawing mode.
     */
    void
    drawRectangle(const Rectangle& rect, DrawingMode mode = StrokePath);

    /*!
     * Draws a rectangle inside the bounding rectangle defined by top left point (x, y)
     * and dimensions (width, height).
     *
     * The following code will draw a filled rounded rectangle with a radius of 5px.
     * \code
     * painter.getPen()->setColor(Color(0, 0, 0));
     * painter.getBrush()->setColor(Color(0, 1, 0));
     * painter.getBrush()->setBrushMode(Brush::SolidColorMode);
     * painter.drawRoundRectangle(0, 0, 100, 100, 5, Painter::StrokeAndFill);
     * \endcode
     *
     * \image html DrawRoundRectangle.png "Example code output"
     *
     * @param x X coordinate of bounding rectangle.
     * @param y X coordinate of bounding rectangle.
     * @param width Width of bounding rectangle.
     * @param height Height of bounding rectangle.
     * @param radius Corner radius.
     * @param mode Set painter's drawing mode.
     * @param Corners specifies which corners should be rounded.
     */
    void
        drawRoundRectangle(double x, double y, double width, double height,
            int radius, DrawingMode mode = StrokePath, Corners corners =
                AllCorners);

    /*!
     * Draws a rounded rectangle inside the bounding rectangle.
     *
     * @param rect Bounding rectangle.
     * @param radius Corner radius.
     * @param mode Set painter's drawing mode.
     * @param Corners specifies which corners should be rounded.
     */
    void
    drawRoundRectangle(const Rectangle& rect, int radius, DrawingMode mode =
        StrokePath, Corners corners = AllCorners);

    /*!
     * Draws the text with its topleft positioned at given point (x, y). By default all text will be rendered.
     * If you only wish to limit the number of characters to display, set the
     * singleLine parameter to false and use a positive number for length.
     *
     * Regardless of the values for singleLine and length parameters text is
     * clipped if it is outside widget's surface boundary.
     *
     * Text is rendered using current pen color and font. \sa getFont(), getPen()
     *
     * For example the following code will render a simple message.
     * \code
     * painter.drawText("Hello World!", 0, 0);
     * \endcode
     *
     * \image html DrawText.png "Example code output"
     *
     * @param text Text to draw.
     * @param x X coordinate of topleft point.
     * @param y X coordinate of topleft point.
     * @param length Maximum length of text.
     * @param mode Set painter's drawing mode.
     */
    void
        drawText(const std::string& text, int x, int y, DrawingMode mode =
            FillPath);

    /*!
     * Renders the text inside the bounding rectangle defined by top left point (x, y)
     * and dimensions (width, height). Text is clipped if it lies outside the defined rectangle
     * and ellipsization is set to EllipsizeNone.
     *
     * Text is rendered using current pen color and font. \sa getFont(), getPen()
     *
     * For example the following code will render a clipped text.
     * \code
     * painter.drawText(0, 0, 50, 20, "Hello World!");
     * \endcode
     *
     * \image html DrawText2.png "Example code output"
     *
     * @param text Text to draw.
     * @param x X coordinate of bounding rectangle.
     * @param y X coordinate of bounding rectangle.
     * @param width Width of bounding rectangle.
     * @param height Height of bounding rectangle.
     * @param vAlign vertical alignment.
     * @param wordWrap If true, word wrapping is applied.
     * @param mode Set painter's drawing mode.
     */
    void
    drawText(const std::string& text, int x, int y, int width, int height,
        VerticalAlignment vAlign = AlignVTop, bool wordWrap = true,
        DrawingMode mode = FillPath);

    void
    drawLayout(PangoLayout* layout, const Font* font, const Point& p,
        DrawingMode mode = FillPath);

    void
    drawLayout(PangoLayout* layout, const Font* font, int x, int y,
        DrawingMode mode = FillPath);

    void
    drawImage(Image* image, const Rectangle& rect,
        const DFBSurfaceBlittingFlags& flags = DSBLIT_BLEND_ALPHACHANNEL,
        bool colorize = false, int red = 255, int green = 255, int blue = 255,
        int alpha = 255);

    void
    drawImage(Image* image, int x, int y, const DFBSurfaceBlittingFlags& flags =
        DSBLIT_BLEND_ALPHACHANNEL, bool colorize = false, int red = 255,
        int green = 255, int blue = 255, int alpha = 255);

    void
    drawPoint(double x, double y);

    void
    drawCurrentPath(DrawingMode mode = StrokePath);

    /*!
     * Sets antialiasing mode of the cairo rasterizer.
     *
     * @param mode
     */
    void
    setAntiAliasMode(AntiAliasMode mode);

    /*!
     * Sets ellipsization mode of the pango.
     *
     * @param mode
     */
    void
    setEllipsizeMode(PangoEllipsizeMode mode);

    /*!
     * Sets how partial lines are aligned inside the layout.
     *
     * @param alignment
     */
    void
    setLayoutAlignment(PangoAlignment alignment);

    /*!
     * Sets the width to indent each paragraph. A negative value will
     * produce hanging indentation.
     *
     * @param indent Amount of indentation in pixels.
     */
    void
    setIndent(int indent);

    /*!
     * Sets whether each complete line should be stretched
     * to fill the entire width of the layout.
     *
     * @param justify Whether the lines in the layout should be justified.
     */
    void
    setJustify(bool justify);

    /*!
     * Sets painter's brush.
     * Brush is used to fill inside the primitive shapes.
     *
     * @param brush Brush.
     */
    void
    setBrush(Brush* brush);

    /*!
     * Sets painter's font.
     * Font is used for rendering text.
     *
     * @param font
     */
    void
    setFont(const Font* font);

    /*!
     * Sets painter's pen.
     * Pen is used to draw outline of primitive shapes.
     *
     * @param pen Pen.
     */
    void
    setPen(Pen* pen);

    void
    setOperator(cairo_operator_t op);

    void
    resetClip();

    //! Transform coordinates so that cairo's stroke operation fills only opaque pixels.
    void
    getUserCoordinates(double &x, double &y, double &width, double &height);

  private:
    //! This property holds Painter's current widget.
    Widget* _myWidget;
    //! This property holds current anti aliasing mode.
    AntiAliasMode _antiAliasMode;
    //! This flag is set to true if brush is active.
    bool _brushActive;

    //! Pointer to painter surface.
    Surface* _surface;
    //! Pointer to _surface's cairo context.
    cairo_t* _context;
    //! Interface to _surface's layout.
    PangoLayout* _pangoLayout;

    //! This is painter's current pen.
    Pen _pen;
    //! This is painter's current brush.
    Brush* _brush;
    //! This is painter's current font.
    Font* _font;

    //! This is painter's default font.
    Font* _defaultFont;

    //    const Font* _widgetFont;
    //    bool widgetFontApplied;

    void
    addPoint(double x, double y);

    //! Apply brush to context if it is modified.
    void
    applyBrush();
    //! Apply font to context if it is modified.
    void
    applyFont();
    //! Apply pen to content if it is modified.
    void
    applyPen();
    //! Applies brush and/or pen accordingly.
    void
    applyDrawingMode(DrawingMode mode);
  };
}

#endif /* ILIXI_PAINTER_H_ */
