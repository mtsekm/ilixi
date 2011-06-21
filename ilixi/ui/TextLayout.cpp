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

#include "ui/TextLayout.h"
#include "types/FontMetrics.h"

using namespace ilixi;

TextLayout::TextLayout(std::string text, Widget* parent) :
  Widget(parent), _font(NULL), _context(NULL), _layout(NULL), _text(text)
{
  PangoFontMap* fontmap = pango_cairo_font_map_get_default();
  if (fontmap)
    {
      _context = pango_font_map_create_context(fontmap);
      if (_context)
        {
          _layout = pango_layout_new(_context);
          pango_layout_set_markup(_layout, text.c_str(), -1);
          //          pango_layout_set_text(_layout, _text.c_str(), _text.length());
          _text = pango_layout_get_text(_layout);
        }
    }

  sigGeometryUpdated.connect(
      sigc::mem_fun(this, &TextLayout::updateTextLayoutGeometry));
}

TextLayout::~TextLayout()
{
  delete _font;
  g_object_unref(_layout);
  g_object_unref(_context);
}

PangoEllipsizeMode
TextLayout::ellipsizeMode() const
{
  return pango_layout_get_ellipsize(_layout);
}

const Font*
TextLayout::font() const
{
  if (_font)
    return _font;
  return defaultFont();
}

std::string
TextLayout::fontFamily() const
{
  return font()->family();
}

int
TextLayout::fontSize() const
{
  return font()->size();
}

int
TextLayout::indent() const
{
  return pango_layout_get_indent(_layout);
}

PangoGravity
TextLayout::gravity() const
{
  return pango_context_get_base_gravity(_context);
}

bool
TextLayout::justify() const
{
  return pango_layout_get_justify(_layout);
}

PangoAlignment
TextLayout::alignment() const
{
  return pango_layout_get_alignment(_layout);
}

PangoStretch
TextLayout::strecth() const
{
  return font()->strecth();
}

PangoStyle
TextLayout::style() const
{
  return font()->style();
}

std::string
TextLayout::text() const
{
  return _text;
}

PangoVariant
TextLayout::variant() const
{
  return font()->variant();
}

PangoWeight
TextLayout::weight() const
{
  return font()->weight();
}

PangoWrapMode
TextLayout::wrapMode() const
{
  return pango_layout_get_wrap(_layout);
}

Size
TextLayout::textExtents() const
{
  if (_text.empty())
    return Size();
  return FontMetrics::getSize(font(), _text, singleParagraph());
}

int
TextLayout::textHeightForWidth(int width) const
{
  if (_text.empty())
    return 0;
  return FontMetrics::heightForWidth(font(), _text, width, singleParagraph());
}

PangoLayout*
TextLayout::layout() const
{
  return _layout;
}

Point
TextLayout::layoutPosition() const
{
  return _layoutPosition;
}

bool
TextLayout::singleParagraph() const
{
  return pango_layout_get_single_paragraph_mode(_layout);
}

void
TextLayout::append(std::string text)
{
  _text += text;
  pango_layout_set_text(_layout, _text.c_str(), _text.length());
  update();
}

void
TextLayout::setEllipsizeMode(PangoEllipsizeMode ellipsizeMode)
{
  pango_layout_set_ellipsize(_layout, ellipsizeMode);
}

void
TextLayout::setFontFamily(std::string fontFamily)
{
  initFont();
  _font->setFamily(fontFamily);
}

void
TextLayout::setFontSize(int fontSize)
{
  initFont();
  _font->setSize(fontSize);
}

void
TextLayout::setIndent(int indent)
{
  pango_layout_set_indent(_layout, indent);
}

void
TextLayout::setGravity(PangoGravity gravity)
{
  pango_context_set_base_gravity(_context, gravity);
  pango_context_set_gravity_hint(_context, PANGO_GRAVITY_HINT_STRONG);
}

void
TextLayout::setSpacing(int spacing)
{
  pango_layout_set_spacing(_layout, spacing);
}

void
TextLayout::setJustify(bool justify)
{
  pango_layout_set_justify(_layout, justify);
}

void
TextLayout::setHorizontalAlignment(PangoAlignment alignment)
{
  pango_layout_set_alignment(_layout, alignment);
}

void
TextLayout::setStrecth(PangoStretch stretch)
{
  initFont();
  _font->setStretch(stretch);
}

void
TextLayout::setStyle(PangoStyle style)
{
  initFont();
  _font->setStyle(style);
}

void
TextLayout::setText(const std::string &text)
{
  _text = text;
  pango_layout_set_text(_layout, _text.c_str(), _text.length());
  update();
}

void
TextLayout::setMarkup(const std::string &markup)
{
  pango_layout_set_markup(_layout, markup.c_str(), markup.length());
  _text = pango_layout_get_text(_layout);
  update();
}

void
TextLayout::setVariant(PangoVariant variant)
{
  initFont();
  _font->setVariant(variant);
}

void
TextLayout::setWeight(PangoWeight weight)
{
  initFont();
  _font->setWeight(weight);
}

void
TextLayout::setWrapMode(PangoWrapMode wrap)
{
  pango_layout_set_wrap(_layout, wrap);
}

void
TextLayout::updateTextLayoutGeometry()
{
  setTextGeometry(0, 0, width(), height());
}

int
TextLayout::textLayoutWidth() const
{
  return pango_layout_get_width(_layout) / PANGO_SCALE;
}

int
TextLayout::textLayoutHeight() const
{
  return pango_layout_get_height(_layout) / PANGO_SCALE;
}

void
TextLayout::setTextGeometry(int x, int y, int width, int height)
{
  _layoutPosition.moveTo(x, y);
  pango_layout_set_width(_layout, width * PANGO_SCALE);
  pango_layout_set_height(_layout, height * PANGO_SCALE);
}

void
TextLayout::setSingleParagraph(bool single)
{
  pango_layout_set_single_paragraph_mode(_layout, single);
}

void
TextLayout::initFont()
{
  if (!_font)
    _font = new Font(defaultFont()->fontDescription());
}

Font*
TextLayout::defaultFont() const
{
  return designer()->defaultFont(DefaultFont);
}
