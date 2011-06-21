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

#include "types/Font.h"

using namespace ilixi;

Font::Font() :
  _changed(true)
{
  _desc = pango_font_description_new();
}

Font::Font(const std::string& fontDescription) :
  _changed(true)
{
  _desc = pango_font_description_from_string(fontDescription.c_str());
}

Font::Font(const Font& font) :
  _changed(true)
{
  _desc = pango_font_description_copy_static(font._desc);
}

Font::Font(const PangoFontDescription* desc)
{
  _desc = pango_font_description_copy_static(desc);
}

Font::~Font()
{
  pango_font_description_free(_desc);
}

PangoFontDescription*
Font::fontDescription() const
{
  return _desc;
}

const char*
Font::family() const
{
  return pango_font_description_get_family(_desc);
}

PangoGravity
Font::gravity() const
{
  return pango_font_description_get_gravity(_desc);
}

int
Font::size() const
{
  return pango_font_description_get_size(_desc);
}

PangoStretch
Font::strecth() const
{
  return pango_font_description_get_stretch(_desc);
}

PangoStyle
Font::style() const
{
  return pango_font_description_get_style(_desc);
}

PangoVariant
Font::variant() const
{
  return pango_font_description_get_variant(_desc);
}

PangoWeight
Font::weight() const
{
  return pango_font_description_get_weight(_desc);
}

void
Font::setFamily(std::string family)
{
  pango_font_description_set_family(_desc, family.c_str());
  _changed = true;
}

void
Font::setGravity(PangoGravity gravity)
{
  pango_font_description_set_gravity(_desc, gravity);
  _changed = true;
}

void
Font::setSize(int size)
{
  pango_font_description_set_size(_desc, size * PANGO_SCALE);
  _changed = true;
}

//void
//Font::setAbsoluteSize(int size)
//{
//  pango_font_description_set_absolute_size(_desc, size * PANGO_SCALE);
//  _changed = true;
//}

void
Font::setStretch(PangoStretch stretch)
{
  pango_font_description_set_stretch(_desc, stretch);
  _changed = true;
}

void
Font::setStyle(PangoStyle style)
{
  pango_font_description_set_style(_desc, style);
  _changed = true;
}

void
Font::setVariant(PangoVariant variant)
{
  pango_font_description_set_variant(_desc, variant);
  _changed = true;
}

void
Font::setWeight(PangoWeight weight)
{
  pango_font_description_set_weight(_desc, weight);
  _changed = true;
}

char*
Font::toString()
{
  return pango_font_description_to_string(_desc);
}

Font&
Font::operator=(const Font& font)
{
  if (this != &font)
    {
      _desc = pango_font_description_copy(font._desc);
      _changed = true;
    }
  return *this;
}

bool
Font::operator==(const Font &font)
{
  return pango_font_description_equal(_desc, font._desc);
}

bool
Font::operator!=(const Font &font)
{
  return !(*this == font);
}

void
Font::applyFont(PangoLayout* pangoLayout)
{
  pango_layout_set_font_description(pangoLayout, _desc);
  _changed = false;
}
