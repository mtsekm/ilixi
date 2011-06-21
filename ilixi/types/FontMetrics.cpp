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

#include "types/FontMetrics.h"

using namespace ilixi;

Size
FontMetrics::getSize(const Font* font, std::string text, bool singleParagraph)
{
  int w = -1, h = -1;
  // Gets a default PangoCairoFontMap to use with Cairo.
  PangoFontMap* fontmap = pango_cairo_font_map_get_default();
  if (fontmap)
    {
      // Create a PangoContext connected to fontmap.
      PangoContext* context = pango_font_map_create_context(fontmap);
      if (context)
        {
          pango_context_set_font_description(context, font->fontDescription());
          PangoLayout* layout = pango_layout_new(context);
          pango_layout_set_single_paragraph_mode(layout, singleParagraph);
          pango_layout_set_text(layout, text.c_str(), text.length());
          pango_layout_get_pixel_size(layout, &w, &h);

          g_object_unref(layout);
          g_object_unref(context);
        }
    }
  return Size(w, h);
}

int
FontMetrics::heightForWidth(const Font* font, std::string text, int w,
    bool singleParagraph)
{
  int h = -1;
  // Get a default PangoCairoFontMap to use with Cairo.
  PangoFontMap* fontmap = pango_cairo_font_map_get_default();

  if (fontmap)
    {
      // Create a PangoContext connected to fontmap.
      PangoContext* context = pango_font_map_create_context(fontmap);
      if (context)
        {
          pango_context_set_font_description(context, font->fontDescription());
          PangoLayout* layout = pango_layout_new(context);

          pango_layout_set_single_paragraph_mode(layout, singleParagraph);
          pango_layout_set_width(layout, w * PANGO_SCALE);

          pango_layout_set_text(layout, text.c_str(), text.length());
          pango_layout_get_pixel_size(layout, NULL, &h);

          g_object_unref(layout);
          g_object_unref(context);
        }
    }
  return h;
}
