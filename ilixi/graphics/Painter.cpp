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

#include "graphics/Painter.h"
#include <pango/pangocairo.h>
#include "ui/TextLayout.h"
#include "core/Logger.h"

//! PI
const double M_PI = 3.14159265;

//! PI /180 (Used for conversion from degrees to radians)
const double M_D2R = 0.0174532925;

using namespace ilixi;

Painter::Painter(Widget* widget) :
  _myWidget(widget), _antiAliasMode(AliasSubPixel), _brushActive(true)
{
  _surface = _myWidget->surface();
  _context = _surface->cairoContext();
  _pangoLayout = pango_cairo_create_layout(_context);
  _font = 0;

  _brush = new Brush();
  _defaultFont = new Font("sans 8");

  cairo_set_antialias(_context, (cairo_antialias_t) _antiAliasMode);
}

Painter::~Painter()
{
  delete _brush;
  delete _defaultFont;
  g_object_unref(_pangoLayout);
  end();
}

void
Painter::begin(const Rectangle& clipRect)
{
  _surface->lock();
  _surface->clip(clipRect);
}

void
Painter::end()
{
  _surface->resetClip();
  _surface->unlock();
}

cairo_t*
Painter::cairoContext() const
{
  return _context;
}

PangoLayout*
Painter::pangoLayout() const
{
  return _pangoLayout;
}

Painter::AntiAliasMode
Painter::getAntiAliasMode() const
{
  return _antiAliasMode;
}

void
Painter::getCursorPosition(int index, Rectangle* weak, Rectangle* strong)
{
  PangoRectangle strongR;
  PangoRectangle weakR;
  pango_layout_get_cursor_pos(_pangoLayout, index, &strongR, &weakR);

  if (strong)
    {
      strong->moveTo(strongR.x / PANGO_SCALE, strongR.y / PANGO_SCALE);
      strong->setSize(1, strongR.height / PANGO_SCALE);
    }

  if (weak)
    {
      weak->moveTo(weakR.x / PANGO_SCALE, weakR.y / PANGO_SCALE);
      weak->setSize(1, weakR.height / PANGO_SCALE);
    }
}

PangoEllipsizeMode
Painter::getEllipsizeMode() const
{
  return pango_layout_get_ellipsize(_pangoLayout);
}

PangoAlignment
Painter::getLayoutAlignment() const
{
  return pango_layout_get_alignment(_pangoLayout);
}

int
Painter::getIndent() const
{
  return PANGO_SCALE * pango_layout_get_indent(_pangoLayout);
}

bool
Painter::getJustify() const
{
  return pango_layout_get_justify(_pangoLayout);
}

int
Painter::getHeightForWidth(std::string text, int width)
{
  applyFont();
  pango_layout_set_width(_pangoLayout, width * PANGO_SCALE);
  pango_layout_set_text(_pangoLayout, text.c_str(), text.length());
  int height;
  pango_layout_get_pixel_size(_pangoLayout, &width, &height);
  pango_layout_set_width(_pangoLayout, -1);
  return height;
}

void
Painter::getPixelExtents(std::string text, int* width, int* height)
{
  applyFont();
  pango_layout_set_text(_pangoLayout, text.c_str(), text.length());
  pango_layout_get_pixel_size(_pangoLayout, width, height);
}

Brush*
Painter::getBrush() const
{
  return _brush;
}

Font*
Painter::getFont() const
{
  return _font;
}

Pen*
Painter::getPen()
{
  return &_pen;
}

void
Painter::drawArc(double x, double y, double width, double height,
    double angle1, double angle2)
{
  cairo_save(_context);
  cairo_translate(_context, x + width / 2.0, y + height / 2.0);
  cairo_scale(_context, width / 2.0, height / 2.0);
  cairo_arc(_context, 0.0, 0.0, 1.0, angle1 * M_D2R, angle2 * M_D2R);
  cairo_restore(_context);
  applyPen();
  cairo_stroke(_context);
}

void
Painter::drawArc(const Rectangle& rect, double angle1, double angle2)
{
  drawArc(rect.x(), rect.y(), rect.width(), rect.height(), angle1, angle2);
}

void
Painter::drawChord(double x, double y, double width, double height,
    double angle1, double angle2, DrawingMode mode)
{
  cairo_save(_context);
  cairo_translate(_context, x + width / 2.0, y + height / 2.0);
  cairo_scale(_context, width / 2.0, height / 2.0);
  cairo_arc(_context, 0.0, 0.0, 1.0, angle1 * M_D2R, angle2 * M_D2R);
  cairo_close_path(_context);
  cairo_restore(_context);
  applyDrawingMode(mode);
}

void
Painter::drawChord(const Rectangle& rect, double angle1, double angle2,
    DrawingMode mode)
{
  drawChord(rect.x(), rect.y(), rect.width(), rect.height(), angle1, angle2,
      mode);
}

void
Painter::drawEllipse(double x, double y, double width, double height,
    DrawingMode mode)
{
  cairo_save(_context);
  cairo_new_sub_path(_context);
  cairo_translate(_context, x + width / 2.0, y + height / 2.0);
  cairo_scale(_context, width / 2.0, height / 2.0);
  cairo_arc(_context, 0.0, 0.0, 1.0, 0.0, 2 * M_PI);
  cairo_close_path(_context);
  cairo_restore(_context);
  applyDrawingMode(mode);
}

void
Painter::drawEllipse(const Rectangle& rect, DrawingMode mode)
{
  drawEllipse(rect.x(), rect.y(), rect.width(), rect.height(), mode);
}

void
Painter::drawLine(double x1, double y1, double x2, double y2, DrawingMode mode)
{
  cairo_move_to(_context, x1, y1);
  cairo_line_to(_context, x2, y2);
  applyDrawingMode(mode);
}

void
Painter::drawLine(const Rectangle& rect, DrawingMode mode)
{
  drawLine(rect.x(), rect.y(), rect.bottom(), rect.right(), mode);
}

void
Painter::drawLine(const Point& p1, const Point& p2, DrawingMode mode)
{
  drawLine(p1.x(), p1.y(), p2.x(), p2.y(), mode);
}

void
Painter::drawPolygon(const Point* points, int pointCount, DrawingMode mode)
{
  cairo_new_sub_path(_context);
  cairo_move_to(_context, points[0].x(), points[0].y());
  for (int i = 1; i < pointCount; i++)
    cairo_line_to(_context, points[i].x(), points[i].y());
  cairo_close_path(_context);
  applyDrawingMode(mode);
}

void
Painter::drawRectangle(double x, double y, double width, double height,
    DrawingMode mode)
{
  if (mode != FillPath)
    getUserCoordinates(x, y, width, height);
  cairo_rectangle(_context, x, y, width, height);
  applyDrawingMode(mode);
}

void
Painter::drawRectangle(const Rectangle& rect, DrawingMode mode)
{
  drawRectangle(rect.x(), rect.y(), rect.width(), rect.height(), mode);
}

void
Painter::drawRoundRectangle(double x, double y, double width, double height,
    int radius, DrawingMode mode, Corners corners)
{
  if (mode != FillPath)
    getUserCoordinates(x, y, width, height);
  cairo_save(_context);
  cairo_new_sub_path(_context);

  // Top-right
  if (corners & TopRight)
    cairo_arc(_context, x + width - radius, y + radius, radius, -1.57079628, 0);
  else
    cairo_line_to(_context, x + width, y);

  // Bottom-right
  if (corners & BottomRight)
    cairo_arc(_context, x + width - radius, y + height - radius, radius, 0,
        1.57079628);
  else
    cairo_line_to(_context, x + width, y + height);

  // Bottom-left
  if (corners & BottomLeft)
    cairo_arc(_context, x + radius, y + height - radius, radius, 1.57079628,
        3.14159265);
  else
    cairo_line_to(_context, x, y + height);

  // Top-left
  if (corners & TopLeft)
    cairo_arc(_context, x + radius, y + radius, radius, 3.14159265, 4.712388975);
  else
    cairo_line_to(_context, x, y);

  cairo_close_path(_context);
  cairo_restore(_context);
  applyDrawingMode(mode);
}

void
Painter::drawRoundRectangle(const Rectangle& rect, int radius,
    DrawingMode mode, Corners corners)
{
  drawRoundRectangle(rect.x(), rect.y(), rect.width(), rect.height(), radius,
      mode, corners);
}

void
Painter::drawText(const std::string& text, int x, int y, DrawingMode mode)
{
  // FIXME: drawText on point does not work atm.
  applyFont();
  cairo_move_to(_context, x, y);
  pango_layout_set_height(_pangoLayout, 0);
  pango_layout_set_text(_pangoLayout, text.c_str(), text.length());

  pango_cairo_update_layout(_context, _pangoLayout);
  pango_cairo_layout_path(_context, _pangoLayout);
  applyDrawingMode(mode);
}

void
Painter::drawText(const std::string& text, int x, int y, int width, int height,
    VerticalAlignment vAlign, bool wordWrap, DrawingMode mode)
{
  // set clipping
  // FIXME: clipping text requires width-1 ?
  //  drawRectangle(x, y, width - 1, height, ClipPath);

  // Temporary variables store height and width of text
  int heightTemp = 0;
  int widthTemp = 0;
  applyFont();

  pango_layout_set_text(_pangoLayout, text.c_str(), text.length());
  pango_layout_get_pixel_size(_pangoLayout, &widthTemp, &heightTemp);

  if (wordWrap)
    {
      pango_layout_set_width(_pangoLayout, width * PANGO_SCALE);
      pango_layout_set_height(_pangoLayout, height * PANGO_SCALE);
      pango_layout_get_pixel_size(_pangoLayout, &widthTemp, &heightTemp);
    }
  else
    {
      pango_layout_set_height(_pangoLayout, 0);
      if (widthTemp > width)
        pango_layout_set_width(_pangoLayout, -1);
      else
        pango_layout_set_width(_pangoLayout, width * PANGO_SCALE);
    }

  switch (vAlign)
    {
  case AlignVTop:
    break;
  case AlignVCenter:
    y += (height - heightTemp) / 2.0;
    break;
  case AlignVBottom:
    y += (height - heightTemp);
    break;
    }

  cairo_move_to(_context, x, y);

  pango_cairo_update_layout(_context, _pangoLayout);
  //  pango_cairo_show_layout(_context, _pangoLayout);
  pango_cairo_layout_path(_context, _pangoLayout);
  applyDrawingMode(mode);
  //  resetClip();
}

void
Painter::drawLayout(PangoLayout* layout, const Font* font, const Point& p,
    DrawingMode mode)
{
  drawLayout(layout, font, p.x(), p.y(), mode);
}

void
Painter::drawLayout(PangoLayout* layout, const Font* font, int x, int y,
    DrawingMode mode)
{
  pango_layout_set_font_description(layout, font->fontDescription());
  cairo_move_to(_context, x, y);
  pango_cairo_layout_path(_context, layout);
  applyDrawingMode(mode);
}

void
Painter::drawImage(Image* image, const Rectangle& rect,
    const DFBSurfaceBlittingFlags& flags, bool colorize, int red, int green,
    int blue, int alpha)
{
  DFBRectangle r =
    { rect.x(), rect.y(), rect.width(), rect.height() };
  IDirectFBSurface* dfbSurface = _surface->DFBSurface();
  if (colorize)
    {
      dfbSurface->SetColor(dfbSurface, red, green, blue, alpha);
      dfbSurface->SetBlittingFlags(dfbSurface,
          (DFBSurfaceBlittingFlags) (flags | DSBLIT_COLORIZE));
    }
  else
    dfbSurface->SetBlittingFlags(dfbSurface, flags);
  dfbSurface->StretchBlit(dfbSurface, image->getDFBSurface(), NULL, &r);
}

void
Painter::drawImage(Image* image, int x, int y,
    const DFBSurfaceBlittingFlags& flags, bool colorize, int red, int green,
    int blue, int alpha)
{
  IDirectFBSurface* dfbSurface = _surface->DFBSurface();
  if (colorize)
    {
      dfbSurface->SetColor(dfbSurface, red, green, blue, alpha);
      dfbSurface->SetBlittingFlags(dfbSurface,
          (DFBSurfaceBlittingFlags) (flags | DSBLIT_COLORIZE));
    }
  else
    dfbSurface->SetBlittingFlags(dfbSurface, flags);
  dfbSurface->Blit(dfbSurface, image->getDFBSurface(), NULL, x, y);
}

void
Painter::drawPoint(double x, double y)
{
  cairo_move_to(_context, x, y);
  cairo_line_to(_context, x, y);
  cairo_stroke(_context);
}

void
Painter::drawCurrentPath(DrawingMode mode)
{
  applyDrawingMode(mode);
}

void
Painter::setAntiAliasMode(AntiAliasMode mode)
{
  _antiAliasMode = mode;
  cairo_set_antialias(_context, (cairo_antialias_t) mode);
}

void
Painter::setEllipsizeMode(PangoEllipsizeMode mode)
{
  pango_layout_set_ellipsize(_pangoLayout, mode);
}

void
Painter::setLayoutAlignment(PangoAlignment alignment)
{
  pango_layout_set_alignment(_pangoLayout, alignment);
}

void
Painter::setIndent(int indent)
{
  pango_layout_set_indent(_pangoLayout, indent * PANGO_SCALE);
}

void
Painter::setJustify(bool justify)
{
  pango_layout_set_justify(_pangoLayout, justify);
}

void
Painter::setBrush(Brush* brush)
{
  if (_brush)
    delete _brush;
  _brush = brush;
  _brush->applyBrush(_context);
  _brushActive = true;
}

void
Painter::setFont(const Font* font)
{
  delete _font;
  _font = const_cast<Font*> (font);
  _font->applyFont(_pangoLayout);
}

void
Painter::setPen(Pen* pen)
{
  _pen = *pen;
  _pen.applyPen(_context);
  _brushActive = false;
}

void
Painter::setOperator(cairo_operator_t op)
{
  cairo_set_operator(_context, op);
}

void
Painter::resetClip()
{
  cairo_reset_clip(_context);
}

void
Painter::addPoint(double x, double y)
{
  cairo_move_to(_context, x, y);
  cairo_line_to(_context, x, y);
}

void
Painter::applyBrush()
{
  if (!_brushActive || _brush->_changed)
    {
      _brush->applyBrush(_context);
      _brushActive = true;
    }
}

void
Painter::applyFont()
{
  if (_font)
    {
      _font->applyFont(_pangoLayout);
      //      pango_layout_set_font_description(_pangoLayout,
      //          _font->getFontDescription());
    }
  else if (_defaultFont->_changed)
    {
      _defaultFont->applyFont(_pangoLayout);
    }
}

void
Painter::applyPen()
{
  if (_brushActive || _pen._changed)
    {
      _pen.applyPen(_context);
      _brushActive = false;
    }
}

void
Painter::applyDrawingMode(DrawingMode mode)
{
  if (mode == AddPath)
    return;
  else if (mode == StrokePath)
    {
      applyPen();
      cairo_stroke(_context);
    }
  else if (mode == FillPath)
    {
      applyBrush();
      cairo_fill(_context);
    }
  else if (mode == StrokeAndFill)
    {
      applyBrush();
      cairo_fill_preserve(_context);
      applyPen();
      cairo_stroke(_context);
    }
  else if (mode == ClipPath)
    cairo_clip(_context);
}

void
Painter::getUserCoordinates(double &x, double &y, double &width, double &height)
{
  int lw = _pen.getLineWidth();
  double offset = .5 * lw;
  double offsetEnd = (lw > 1) ? lw : 1;
  x += offset;
  y += offset;
  width -= offsetEnd;
  height -= offsetEnd;
}
