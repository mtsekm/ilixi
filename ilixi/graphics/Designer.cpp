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

#include "graphics/Designer.h"
#include "graphics/Painter.h"
#include "types/LinearGradient.h"
#include "types/RadialGradient.h"
#include "ilixiGUI.h"

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#include "core/Logger.h"

using namespace ilixi;

Designer::Designer(const std::string& styleSheet)
{
  _borderWidth = 1;
  //setStyleSheet(styleSheet, DATADIR"/designer/");
}

Designer::~Designer()
{
  cleanStyleElements();
}

void
Designer::setStyleSheet(const std::string& styleSheet, const std::string& path)
{
  if (styleSheet == "")
    return;

  ILOG_DEBUG("Parsing style file...");
  //  LIBXML_TEST_VERSION

  xmlDocPtr doc;
  xmlNodePtr cur;

  std::string styleFile = "";
  styleFile.append(path).append(styleSheet);

  doc = xmlParseFile(styleFile.c_str());

  if (doc == NULL)
    {
      ILOG_ERROR("Style file not parsed successfully!");
      return;
    }

  cur = xmlDocGetRootElement(doc);

  if (cur == NULL)
    {
      ILOG_ERROR("Empty document!");
      xmlFreeDoc(doc);
      return;
    }

  if (xmlStrcmp(cur->name, (const xmlChar *) "style"))
    {
      ILOG_ERROR("Not a valid style file!");
      xmlFreeDoc(doc);
      return;
    }
  //  cleanStyleElements();
  cur = cur->xmlChildrenNode;
  while (cur != NULL)
    {
      if (xmlStrcmp(cur->name, (const xmlChar *) "palette") == 0)
        parsePalette((xmlDoc*) cur);

      if (xmlStrcmp(cur->name, (const xmlChar *) "fonts") == 0)
        parseFonts((xmlDoc*) cur);

      if (xmlStrcmp(cur->name, (const xmlChar *) "hints") == 0)
        parseHints((xmlDoc*) cur);

      if (xmlStrcmp(cur->name, (const xmlChar *) "images") == 0)
        parseImages(path, (xmlDoc*) cur);

      cur = cur->next;
    }

  xmlFreeDoc(doc);
  ILOG_INFO("Parsed style file.");
}

void
Designer::cleanStyleElements()
{
  // clear images
  delete _arrowUp;
  delete _arrowDown;
  delete _arrowLeft;
  delete _arrowRight;
  delete _checkEmpty;
  delete _checkFull;
  delete _checkPartial;
  delete _grid;
  delete _plusSign;
  delete _minusSign;
  delete _dialog;
  delete _dialogInfo;
  delete _dialogWarning;
  delete _dialogCritical;
  delete _dialogQuestion;
  // clear fonts
  delete _defaultFont;
  delete _buttonFont;
  delete _titleFont;
  delete _inputFont;
}

void
Designer::parsePalette(xmlDoc* node)
{
  ILOG_DEBUG(" > Parsing palette...");
  xmlNodePtr colorNode;
  xmlNodePtr item = ((xmlNodePtr) node)->xmlChildrenNode;
  while (item != NULL)
    {
      if (xmlStrcmp(item->name, (const xmlChar *) "group") == 0)
        {
          xmlChar *groupName = xmlGetProp(item, (const xmlChar *) "name");
          ILOG_DEBUG("   > Parsing group %s", groupName);

          colorNode = item->xmlChildrenNode;
          while (colorNode != NULL)
            {
              xmlChar* target = xmlGetProp(colorNode,
                  (const xmlChar *) "target");
              if (target)
                {
                  xmlChar* redAttr = xmlGetProp(colorNode,
                      (const xmlChar *) "red");
                  xmlChar* greenAttr = xmlGetProp(colorNode,
                      (const xmlChar *) "green");
                  xmlChar* blueAttr = xmlGetProp(colorNode,
                      (const xmlChar *) "blue");
                  xmlChar* alphaAttr = xmlGetProp(colorNode,
                      (const xmlChar *) "alpha");

                  Color color;
                  if (redAttr)
                    color.setRed(atoi((char*) redAttr) / 255.0);
                  if (greenAttr)
                    color.setGreen(atoi((char*) greenAttr) / 255.0);
                  if (blueAttr)
                    color.setBlue(atoi((char*) blueAttr) / 255.0);
                  if (alphaAttr)
                    color.setAlpha(atoi((char*) alphaAttr) / 255.0);

                  if (!xmlStrcmp(groupName, (const xmlChar *) "application"))
                    {
                      if (!xmlStrcmp(target, (const xmlChar *) "_baseTop"))
                        _palette._baseTop = color;
                      else if (!xmlStrcmp(target,
                          (const xmlChar *) "_baseBottom"))
                        _palette._baseBottom = color;
                      else if (!xmlStrcmp(target, (const xmlChar *) "_text"))
                        _palette._text = color;
                      else if (!xmlStrcmp(target,
                          (const xmlChar *) "_textDisabled"))
                        _palette._textDisabled = color;
                      else if (!xmlStrcmp(target, (const xmlChar *) "_focusTop"))
                        _palette._focusTop = color;
                      else if (!xmlStrcmp(target, (const xmlChar *) "_focusMid"))
                        _palette._focusMid = color;
                      else if (!xmlStrcmp(target,
                          (const xmlChar *) "_focusBottom"))
                        _palette._focusBottom = color;
                      else
                        ILOG_ERROR(
                            "Color target=\"%s\" is not supported for %s!",
                            target, groupName);
                    }
                  else if (!xmlStrcmp(groupName, (const xmlChar *) "default"))
                    {
                      if (!xmlStrcmp(target, (const xmlChar *) "_base"))
                        _palette._default._base = color;
                      else if (!xmlStrcmp(target, (const xmlChar *) "_baseText"))
                        _palette._default._baseText = color;
                      else if (!xmlStrcmp(target, (const xmlChar *) "_baseAlt"))
                        _palette._default._baseAlt = color;
                      else if (!xmlStrcmp(target,
                          (const xmlChar *) "_baseAltText"))
                        _palette._default._baseAltText = color;
                      else if (!xmlStrcmp(target,
                          (const xmlChar *) "_backgroundTop"))
                        _palette._default._backgroundTop = color;
                      else if (!xmlStrcmp(target,
                          (const xmlChar *) "_backgroundMid"))
                        _palette._default._backgroundMid = color;
                      else if (!xmlStrcmp(target,
                          (const xmlChar *) "_backgroundBottom"))
                        _palette._default._backgroundBottom = color;
                      else if (!xmlStrcmp(target,
                          (const xmlChar *) "_borderTop"))
                        _palette._default._borderTop = color;
                      else if (!xmlStrcmp(target,
                          (const xmlChar *) "_borderMid"))
                        _palette._default._borderMid = color;
                      else if (!xmlStrcmp(target,
                          (const xmlChar *) "_borderBottom"))
                        _palette._default._borderBottom = color;
                      else if (!xmlStrcmp(target, (const xmlChar *) "_fillTop"))
                        _palette._default._fillTop = color;
                      else if (!xmlStrcmp(target, (const xmlChar *) "_fillMid"))
                        _palette._default._fillMid = color;
                      else if (!xmlStrcmp(target,
                          (const xmlChar *) "_fillBottom"))
                        _palette._default._fillBottom = color;
                      else if (!xmlStrcmp(target, (const xmlChar *) "_text"))
                        _palette._default._text = color;
                      else
                        ILOG_ERROR("Color target=\"%s\" is not supported for %s!", target, groupName);
                    }

                  else if (!xmlStrcmp(groupName, (const xmlChar *) "exposed"))
                    {
                      if (!xmlStrcmp(target, (const xmlChar *) "_base"))
                        _palette._exposed._base = color;
                      else if (!xmlStrcmp(target, (const xmlChar *) "_baseText"))
                        _palette._exposed._baseText = color;
                      else if (!xmlStrcmp(target, (const xmlChar *) "_baseAlt"))
                        _palette._exposed._baseAlt = color;
                      else if (!xmlStrcmp(target,
                          (const xmlChar *) "_baseAltText"))
                        _palette._exposed._baseAltText = color;
                      else if (!xmlStrcmp(target,
                          (const xmlChar *) "_backgroundTop"))
                        _palette._exposed._backgroundTop = color;
                      else if (!xmlStrcmp(target,
                          (const xmlChar *) "_backgroundMid"))
                        _palette._exposed._backgroundMid = color;
                      else if (!xmlStrcmp(target,
                          (const xmlChar *) "_backgroundBottom"))
                        _palette._exposed._backgroundBottom = color;
                      else if (!xmlStrcmp(target,
                          (const xmlChar *) "_borderTop"))
                        _palette._exposed._borderTop = color;
                      else if (!xmlStrcmp(target,
                          (const xmlChar *) "_borderMid"))
                        _palette._exposed._borderMid = color;
                      else if (!xmlStrcmp(target,
                          (const xmlChar *) "_borderBottom"))
                        _palette._exposed._borderBottom = color;
                      else if (!xmlStrcmp(target, (const xmlChar *) "_fillTop"))
                        _palette._exposed._fillTop = color;
                      else if (!xmlStrcmp(target, (const xmlChar *) "_fillMid"))
                        _palette._exposed._fillMid = color;
                      else if (!xmlStrcmp(target,
                          (const xmlChar *) "_fillBottom"))
                        _palette._exposed._fillBottom = color;
                      else if (!xmlStrcmp(target, (const xmlChar *) "_text"))
                        _palette._exposed._text = color;
                      else
                        ILOG_ERROR("Color target=\"%s\" is not supported for %s!", target, groupName);
                    }
                  else if (!xmlStrcmp(groupName, (const xmlChar *) "pressed"))
                    {
                      if (!xmlStrcmp(target, (const xmlChar *) "_base"))
                        _palette._pressed._base = color;
                      else if (!xmlStrcmp(target, (const xmlChar *) "_baseText"))
                        _palette._pressed._baseText = color;
                      else if (!xmlStrcmp(target, (const xmlChar *) "_baseAlt"))
                        _palette._pressed._baseAlt = color;
                      else if (!xmlStrcmp(target,
                          (const xmlChar *) "_baseAltText"))
                        _palette._pressed._baseAltText = color;
                      else if (!xmlStrcmp(target,
                          (const xmlChar *) "_backgroundTop"))
                        _palette._pressed._backgroundTop = color;
                      else if (!xmlStrcmp(target,
                          (const xmlChar *) "_backgroundMid"))
                        _palette._pressed._backgroundMid = color;
                      else if (!xmlStrcmp(target,
                          (const xmlChar *) "_backgroundBottom"))
                        _palette._pressed._backgroundBottom = color;
                      else if (!xmlStrcmp(target,
                          (const xmlChar *) "_borderTop"))
                        _palette._pressed._borderTop = color;
                      else if (!xmlStrcmp(target,
                          (const xmlChar *) "_borderMid"))
                        _palette._pressed._borderMid = color;
                      else if (!xmlStrcmp(target,
                          (const xmlChar *) "_borderBottom"))
                        _palette._pressed._borderBottom = color;
                      else if (!xmlStrcmp(target, (const xmlChar *) "_fillTop"))
                        _palette._pressed._fillTop = color;
                      else if (!xmlStrcmp(target, (const xmlChar *) "_fillMid"))
                        _palette._pressed._fillMid = color;
                      else if (!xmlStrcmp(target,
                          (const xmlChar *) "_fillBottom"))
                        _palette._pressed._fillBottom = color;
                      else if (!xmlStrcmp(target, (const xmlChar *) "_text"))
                        _palette._pressed._text = color;
                      else
                        ILOG_ERROR("Color target=\"%s\" is not supported for %s!", target, groupName);
                    }
                  else if (!xmlStrcmp(groupName, (const xmlChar *) "disabled"))
                    {
                      if (!xmlStrcmp(target, (const xmlChar *) "_base"))
                        _palette._disabled._base = color;
                      else if (!xmlStrcmp(target, (const xmlChar *) "_baseText"))
                        _palette._disabled._baseText = color;
                      else if (!xmlStrcmp(target, (const xmlChar *) "_baseAlt"))
                        _palette._disabled._baseAlt = color;
                      else if (!xmlStrcmp(target,
                          (const xmlChar *) "_baseAltText"))
                        _palette._disabled._baseAltText = color;
                      else if (!xmlStrcmp(target,
                          (const xmlChar *) "_backgroundTop"))
                        _palette._disabled._backgroundTop = color;
                      else if (!xmlStrcmp(target,
                          (const xmlChar *) "_backgroundMid"))
                        _palette._disabled._backgroundMid = color;
                      else if (!xmlStrcmp(target,
                          (const xmlChar *) "_backgroundBottom"))
                        _palette._disabled._backgroundBottom = color;
                      else if (!xmlStrcmp(target,
                          (const xmlChar *) "_borderTop"))
                        _palette._disabled._borderTop = color;
                      else if (!xmlStrcmp(target,
                          (const xmlChar *) "_borderMid"))
                        _palette._disabled._borderMid = color;
                      else if (!xmlStrcmp(target,
                          (const xmlChar *) "_borderBottom"))
                        _palette._disabled._borderBottom = color;
                      else if (!xmlStrcmp(target, (const xmlChar *) "_fillTop"))
                        _palette._disabled._fillTop = color;
                      else if (!xmlStrcmp(target, (const xmlChar *) "_fillMid"))
                        _palette._disabled._fillMid = color;
                      else if (!xmlStrcmp(target,
                          (const xmlChar *) "_fillBottom"))
                        _palette._disabled._fillBottom = color;
                      else if (!xmlStrcmp(target, (const xmlChar *) "_text"))
                        _palette._disabled._text = color;
                      else
                        ILOG_ERROR("Color target=\"%s\" is not supported for %s!", target, groupName);
                    }
                  xmlFree(redAttr);
                  xmlFree(greenAttr);
                  xmlFree(blueAttr);
                  xmlFree(alphaAttr);
                }
              xmlFree(target);
              colorNode = colorNode->next;
            }
          xmlFree(groupName);
        }
      item = item->next;
    }
}

void
Designer::parseFonts(xmlDoc* node)
{
  ILOG_DEBUG(" > Parsing fonts...");
  xmlNodePtr item = ((xmlNodePtr) node)->xmlChildrenNode;
  while (item != NULL)
    {
      if (xmlStrcmp(item->name, (const xmlChar *) "font") == 0)
        {
          xmlChar *nameAttr = xmlGetProp(item, (const xmlChar *) "name");
          xmlChar *familyAttr = xmlGetProp(item, (const xmlChar *) "family");
          xmlChar *sizeAttr = xmlGetProp(item, (const xmlChar *) "size");
          xmlChar *weightAttr = xmlGetProp(item, (const xmlChar *) "weight");

          if (nameAttr && familyAttr && sizeAttr)
            {

              PangoWeight weight = PANGO_WEIGHT_MEDIUM;
              if (weightAttr)
                {
                  if (xmlStrcmp(weightAttr, (const xmlChar *) "thin") == 0)
                    weight = PANGO_WEIGHT_THIN;
                  else if (xmlStrcmp(weightAttr, (const xmlChar *) "ultralight")
                      == 0)
                    weight = PANGO_WEIGHT_ULTRALIGHT;
                  else if (xmlStrcmp(weightAttr, (const xmlChar *) "light")
                      == 0)
                    weight = PANGO_WEIGHT_LIGHT;
                  else if (xmlStrcmp(weightAttr, (const xmlChar *) "book") == 0)
                    weight = PANGO_WEIGHT_BOOK;
                  else if (xmlStrcmp(weightAttr, (const xmlChar *) "normal")
                      == 0)
                    weight = PANGO_WEIGHT_NORMAL;
                  else if (xmlStrcmp(weightAttr, (const xmlChar *) "medium")
                      == 0)
                    weight = PANGO_WEIGHT_MEDIUM;
                  else if (xmlStrcmp(weightAttr, (const xmlChar *) "semibold")
                      == 0)
                    weight = PANGO_WEIGHT_SEMIBOLD;
                  else if (xmlStrcmp(weightAttr, (const xmlChar *) "bold") == 0)
                    weight = PANGO_WEIGHT_BOLD;
                  else if (xmlStrcmp(weightAttr, (const xmlChar *) "ultrabold")
                      == 0)
                    weight = PANGO_WEIGHT_ULTRABOLD;
                  else if (xmlStrcmp(weightAttr, (const xmlChar *) "heavy")
                      == 0)
                    weight = PANGO_WEIGHT_HEAVY;
                  else if (xmlStrcmp(weightAttr, (const xmlChar *) "ultraheavy")
                      == 0)
                    weight = PANGO_WEIGHT_ULTRAHEAVY;
                }

              if (xmlStrcmp(nameAttr, (const xmlChar *) "Default") == 0)
                {
                  _defaultFont = new Font((char*) familyAttr);
                  _defaultFont->setSize(atoi((char*) sizeAttr));
                  _defaultFont->setWeight(weight);
                }
              else if (xmlStrcmp(nameAttr, (const xmlChar *) "Button") == 0)
                {
                  _buttonFont = new Font((char*) familyAttr);
                  _buttonFont->setSize(atoi((char*) sizeAttr));
                  _buttonFont->setWeight(weight);
                }
              else if (xmlStrcmp(nameAttr, (const xmlChar *) "Title") == 0)
                {
                  _titleFont = new Font((char*) familyAttr);
                  _titleFont->setSize(atoi((char*) sizeAttr));
                  _titleFont->setWeight(weight);
                }
              else if (xmlStrcmp(nameAttr, (const xmlChar *) "Input") == 0)
                {
                  _inputFont = new Font((char*) familyAttr);
                  _inputFont->setSize(atoi((char*) sizeAttr));
                  _inputFont->setWeight(weight);
                }
              else
                ILOG_ERROR("Font name=\"%s\" is not supported!", nameAttr);
            }

          xmlFree(nameAttr);
          xmlFree(familyAttr);
          xmlFree(sizeAttr);
          xmlFree(weightAttr);
        }
      item = item->next;
    }
}

void
Designer::parseHints(xmlDoc* node)
{
  ILOG_DEBUG(" > Parsing hints...");
  xmlNodePtr item = ((xmlNodePtr) node)->xmlChildrenNode;
  while (item != NULL)
    {
      if (xmlStrcmp(item->name, (const xmlChar *) "hint") == 0)
        {
          xmlChar *name = xmlGetProp(item, (const xmlChar *) "name");

          if (xmlStrcmp(name, (const xmlChar *) "Frame") == 0)
            {
              xmlChar *radius = xmlGetProp(item, (const xmlChar *) "radius");
              if (radius)
                _frameBorderRadius = atoi((char*) radius);

              xmlFree(radius);
            }

          else if (xmlStrcmp(name, (const xmlChar *) "Button") == 0)
            {
              xmlChar *width = xmlGetProp(item, (const xmlChar *) "width");
              xmlChar *height = xmlGetProp(item, (const xmlChar *) "height");
              xmlChar *radius = xmlGetProp(item, (const xmlChar *) "radius");
              xmlChar *offset = xmlGetProp(item, (const xmlChar *) "offset");
              xmlChar *cw = xmlGetProp(item,
                  (const xmlChar *) "checkedIndicatorWidth");

              if (width && height)
                _buttonSize = Size(atoi((char*) width), atoi((char*) height));
              if (radius)
                _buttonRadius = atoi((char*) radius);
              if (offset)
                _buttonOffset = atoi((char*) offset);
              if (cw)
                _buttonCheckedIndicatorWidth = atoi((char*) cw);

              xmlFree(width);
              xmlFree(height);
              xmlFree(radius);
              xmlFree(offset);
              xmlFree(cw);
            }

          else if (xmlStrcmp(name, (const xmlChar *) "CheckBox") == 0)
            {
              xmlChar *width = xmlGetProp(item, (const xmlChar *) "width");
              xmlChar *height = xmlGetProp(item, (const xmlChar *) "height");
              xmlChar *radius = xmlGetProp(item, (const xmlChar *) "radius");
              xmlChar *offset = xmlGetProp(item, (const xmlChar *) "offset");

              if (width && height)
                _checkboxSize = Size(atoi((char*) width), atoi((char*) height));
              if (radius)
                _checkboxRadius = atoi((char*) radius);
              if (offset)
                _checkboxOffset = atoi((char*) offset);

              xmlFree(width);
              xmlFree(height);
              xmlFree(radius);
              xmlFree(offset);
            }

          else if (xmlStrcmp(name, (const xmlChar *) "ComboBox") == 0)
            {
              xmlChar *width =
                  xmlGetProp(item, (const xmlChar *) "buttonWidth");
              xmlChar *radius = xmlGetProp(item, (const xmlChar *) "radius");

              if (width)
                _comboboxButtonWidth = atoi((char*) width);
              if (radius)
                _comboboxRadius = atoi((char*) radius);

              xmlFree(width);
              xmlFree(radius);
            }
          else if (xmlStrcmp(name, (const xmlChar *) "RadioButton") == 0)
            {
              xmlChar *width = xmlGetProp(item, (const xmlChar *) "width");
              xmlChar *height = xmlGetProp(item, (const xmlChar *) "height");
              xmlChar *offset = xmlGetProp(item, (const xmlChar *) "offset");

              if (width && height)
                _radiobuttonSize = Size(atoi((char*) width),
                    atoi((char*) height));
              if (offset)
                _radiobuttonOffset = atoi((char*) offset);

              xmlFree(width);
              xmlFree(height);
              xmlFree(offset);
            }
          else if (xmlStrcmp(name, (const xmlChar *) "ProgressBar") == 0)
            {
              xmlChar *width = xmlGetProp(item, (const xmlChar *) "width");
              xmlChar *height = xmlGetProp(item, (const xmlChar *) "height");
              xmlChar *radius = xmlGetProp(item, (const xmlChar *) "radius");

              if (width && height)
                _progressbarSize = Size(atoi((char*) width),
                    atoi((char*) height));
              if (radius)
                _progressbarRadius = atoi((char*) radius);

              xmlFree(width);
              xmlFree(height);
              xmlFree(radius);
            }
          else if (xmlStrcmp(name, (const xmlChar *) "Slider") == 0)
            {
              xmlChar *width = xmlGetProp(item, (const xmlChar *) "width");
              xmlChar *height = xmlGetProp(item, (const xmlChar *) "height");
              xmlChar *radius = xmlGetProp(item, (const xmlChar *) "radius");
              xmlChar *fh = xmlGetProp(item, (const xmlChar *) "frameHeight");

              if (width && height)
                _sliderSize = Size(atoi((char*) width), atoi((char*) height));
              if (radius)
                _sliderRadius = atoi((char*) radius);
              if (fh)
                _sliderFrameHeight = atoi((char*) fh);

              xmlFree(width);
              xmlFree(height);
              xmlFree(radius);
              xmlFree(fh);
            }
          else if (xmlStrcmp(name, (const xmlChar *) "ScrollBar") == 0)
            {
              xmlChar *width = xmlGetProp(item, (const xmlChar *) "width");
              xmlChar *height = xmlGetProp(item, (const xmlChar *) "height");
              xmlChar *radius = xmlGetProp(item, (const xmlChar *) "radius");
              xmlChar *bw = xmlGetProp(item, (const xmlChar *) "buttonWidth");

              if (width && height)
                _scrollbarSize
                    = Size(atoi((char*) width), atoi((char*) height));
              if (radius)
                _scrollbarRadius = atoi((char*) radius);
              if (bw)
                _scrollbarButtonWidth = atoi((char*) bw);

              xmlFree(width);
              xmlFree(height);
              xmlFree(radius);
              xmlFree(bw);
            }

          else if (xmlStrcmp(name, (const xmlChar *) "TabPanel") == 0)
            {

              xmlChar *width = xmlGetProp(item, (const xmlChar *) "width");
              xmlChar *height = xmlGetProp(item, (const xmlChar *) "height");
              xmlChar *bh = xmlGetProp(item, (const xmlChar *) "buttonHeight");
              xmlChar *bo = xmlGetProp(item, (const xmlChar *) "buttonOffset");

              if (width && height)
                _tabPanelSize = Size(atoi((char*) width), atoi((char*) height));
              if (bh)
                _tabPanelButtonHeight = atoi((char*) bh);
              if (bo)
                _tabPanelButtonOffset = atoi((char*) bo);

              xmlFree(width);
              xmlFree(height);
              xmlFree(bh);
              xmlFree(bo);
            }
          else if (xmlStrcmp(name, (const xmlChar *) "TextInput") == 0)
            {
              xmlChar *radius = xmlGetProp(item, (const xmlChar *) "radius");

              if (radius)
                _textInputFrameRadius = atoi((char*) radius);

              xmlFree(radius);
            }
          else if (xmlStrcmp(name, (const xmlChar *) "Dialog") == 0)
            {
              xmlChar *sh = xmlGetProp(item, (const xmlChar *) "shadow");

              if (sh)
                _dialogShadow = atoi((char*) sh);

              xmlFree(sh);
            }
          else
            ILOG_ERROR("Hint name=\"%s\" is not supported!", name);
          xmlFree(name);
        }
      item = item->next;
    }
}

void
Designer::parseImages(std::string pathFS, xmlDoc* node)
{
  ILOG_DEBUG(" > Parsing images...");
  xmlNodePtr item = ((xmlNodePtr) node)->xmlChildrenNode;
  while (item != NULL)
    {
      if (xmlStrcmp(item->name, (const xmlChar *) "image") == 0)
        {
          xmlChar *nameAttr = xmlGetProp(item, (const xmlChar *) "name");
          xmlChar *pathAttr = xmlGetProp(item, (const xmlChar *) "path");

          if (nameAttr == NULL)
            ILOG_ERROR("Image name attribute is empty!");
          else if (pathAttr == NULL)
            ILOG_ERROR("Path is empty for %s!", nameAttr);
          else
            {
              char* name = (char *) nameAttr;
              std::string path = std::string(pathFS).append((char *) pathAttr);

              if (!strcmp(name, "CheckEmpty"))
                _checkEmpty = new Image(path, _checkboxSize.width(),
                    _checkboxSize.height());
              else if (!strcmp(name, "CheckFull"))
                _checkFull = new Image(path, _checkboxSize.width(),
                    _checkboxSize.height());
              else if (!strcmp(name, "CheckPartial"))
                _checkPartial = new Image(path, _checkboxSize.width(),
                    _checkboxSize.height());
              else if (!strcmp(name, "Grid"))
                _grid = new Image(path, _scrollbarSize.height(),
                    _scrollbarSize.height());
              else if (!strcmp(name, "ArrowUp"))
                _arrowUp = new Image(path, _scrollbarButtonWidth,
                    _scrollbarSize.height());
              else if (!strcmp(name, "ArrowDown"))
                _arrowDown = new Image(path, _scrollbarButtonWidth,
                    _scrollbarSize.height());
              else if (!strcmp(name, "ArrowLeft"))
                _arrowLeft = new Image(path, _scrollbarButtonWidth,
                    _scrollbarSize.height());
              else if (!strcmp(name, "ArrowRight"))
                _arrowRight = new Image(path, _scrollbarButtonWidth,
                    _scrollbarSize.height());
              else if (!strcmp(name, "Dialog"))
                _dialog = new Image(path, 24, 24);
              else if (!strcmp(name, "Critical"))
                _dialogCritical = new Image(path, 48, 48);
              else if (!strcmp(name, "Information"))
                _dialogInfo = new Image(path, 48, 48);
              else if (!strcmp(name, "Question"))
                _dialogQuestion = new Image(path, 48, 48);
              else if (!strcmp(name, "Warning"))
                _dialogWarning = new Image(path, 48, 48);
              else if (!strcmp(name, "Plus"))
                _plusSign = new Image(path, 48, 48);
              else if (!strcmp(name, "Minus"))
                _minusSign = new Image(path, 48, 48);
              else
                ILOG_ERROR("Image name=\"%s\" is not supported!", name);
            }
          xmlFree(nameAttr);
          xmlFree(pathAttr);
        }
      item = item->next;
    }
}

void
Designer::drawLabel(Painter* painter, Label* label)
{
  if (label->state() & DisabledState)
    painter->getBrush()->setColor(_palette._textDisabled);
  else
    painter->getBrush()->setColor(_palette._text);

  painter->drawLayout(label->layout(), label->font(), label->layoutPosition());
}

void
Designer::drawIcon(Painter* painter, Icon* icon)
{
  if (icon->border())
    {
      painter->getPen()->setLineWidth(1);
      painter->getPen()->setColor(_palette._default._borderBottom);
      painter->drawRectangle(0, 0, icon->width(), icon->height(),
          Painter::StrokePath);
      painter->drawImage(icon->image(),
          Rectangle(1, 1, icon->width() - 1, icon->height() - 1));
    }
  else
    painter->drawImage(icon->image(),
        Rectangle(0, 0, icon->width(), icon->height()));
}

void
Designer::drawPushButton(Painter* painter, PushButton* button)
{
  Pen* pen = painter->getPen();
  Brush* brush = painter->getBrush();
  const WidgetState state = button->state();

  // set brush
  LinearGradient background(0, 0, 0, button->height());
  if (button->checkable() && button->checked())
    brush->setColor(_palette._pressed._backgroundBottom);
  else
    brush->setColor(_palette.getGroup(state)._backgroundBottom);

  // set pen
  pen->setColor(_palette.getGroup(state)._borderBottom);
  pen->setLineWidth(1);

  // draw frame
  painter->drawRoundRectangle(0, 0, button->width(), button->height(),
      _buttonRadius, Painter::StrokeAndFill);

  // draw image
  if (button->hasIcon())
    drawImage(painter, button->icon(), button->iconPosition().x(),
        button->iconPosition().y() + button->pressed(), !button->enabled());

  // draw button's pango layout
  if (!button->text().empty())
    {
      brush->setColor(_palette.getGroup(state)._text);
      painter->drawLayout(button->layout(), button->font(),
          button->layoutPosition().x(),
          button->layoutPosition().y() + button->pressed());
    }
}

void
Designer::drawToolButton(Painter* painter, ToolButton* button)
{
}

void
Designer::drawCheckBox(Painter* painter, Checkbox* button)
{
  Pen* pen = painter->getPen();
  Brush* brush = painter->getBrush();
  const WidgetState state = button->state();
  bool disabled = state & DisabledState;
  int x = _checkboxOffset;
  int y = (button->height() - _checkboxSize.height()) / 2;

  // set brush and pen
  brush->setColor(_palette.getGroup(state)._backgroundBottom);
  pen->setColor(_palette.getGroup(state)._borderBottom);
  pen->setLineWidth(1);

  // draw frame
  painter->drawRoundRectangle(x, y, _checkboxSize.width(),
      _checkboxSize.height(), _checkboxRadius, Painter::StrokeAndFill);

  // draw checkbox state
  Rectangle r(x, y, _checkboxSize.width(), _checkboxSize.height());
  if (button->checkState() == Checkbox::Checked)
    drawImage(painter, _checkFull, r, disabled);
  else if (button->checkState() == Checkbox::Partial)
    drawImage(painter, _checkPartial, r, disabled);
  else
    drawImage(painter, _checkEmpty, r, disabled);

  // draw image
  if (button->hasIcon())
    drawImage(painter, button->icon(), button->iconPosition().x(),
        button->iconPosition().y(), !button->enabled());

  // draw button's pango layout
  if (!button->text().empty())
    {
      brush->setColor(_palette.getGroup(state)._text);
      painter->drawLayout(button->layout(), button->font(),
          button->layoutPosition());
    }
}

void
Designer::drawComboBox(Painter* painter, ComboBox* combo)
{
  Pen* pen = painter->getPen();
  Brush* brush = painter->getBrush();
  const WidgetState state = combo->state();

  pen->setColor(_palette.getGroup(state)._borderBottom);
  pen->setLineWidth(1);
  // draw frame
  painter->drawRoundRectangle(0, 0, combo->width(), combo->height(),
      _comboboxRadius, Painter::StrokePath);

  painter->getBrush()->setColor(_palette.getGroup(state)._backgroundBottom);

  int x = combo->width() - _comboboxButtonWidth;
  painter->drawRoundRectangle(x, 1, combo->width() - x, combo->height() - 2,
      _comboboxRadius, Painter::FillPath, RightCorners);

  brush->setColor(_palette.getGroup(state)._baseText);
  painter->drawLayout(combo->layout(), combo->font(), combo->layoutPosition());

  drawImage(painter, _arrowDown, x, 0, !combo->enabled());
}

void
Designer::drawRadioButton(Painter* painter, RadioButton* button)
{
  Pen* pen = painter->getPen();
  Brush* brush = painter->getBrush();
  const WidgetState state = button->state();
  bool disabled = state & DisabledState;
  int x = _radiobuttonOffset;
  int y = (button->height() - _radiobuttonSize.height()) / 2;

  // set brush and pen
  brush->setColor(_palette.getGroup(state)._backgroundBottom);
  pen->setColor(_palette.getGroup(state)._borderBottom);
  pen->setLineWidth(1);

  // draw frame
  painter->drawEllipse(x, y, _radiobuttonSize.width(),
      _radiobuttonSize.height(), Painter::StrokeAndFill);

  // draw selection
  pen->setColor(_palette.getGroup(state)._borderBottom);
  if (button->checked())
    brush->setColor(_palette._focusTop);
  else
    brush->setColor(_palette.getGroup(state)._borderMid);
  painter->drawEllipse(x + 4, y + 4, _radiobuttonSize.width() - 8,
      _radiobuttonSize.height() - 8, Painter::StrokeAndFill);

  // draw image
  if (button->hasIcon())
    drawImage(painter, button->icon(), button->iconPosition().x(),
        button->iconPosition().y(), !button->enabled());

  // draw button's pango layout
  if (!button->text().empty())
    {
      brush->setColor(_palette.getGroup(state)._text);
      painter->drawLayout(button->layout(), button->font(),
          button->layoutPosition());
    }
}

void
Designer::drawProgressBar(Painter* painter, ProgressBar* bar)
{
  Pen* pen = painter->getPen();
  Brush* brush = painter->getBrush();
  const WidgetState state = bar->state();

  // Calculate positions and dimensions
  Orientation orientation = bar->orientation();
  int frameX = 0;
  int frameY = 0;
  int frameWidth = bar->width();
  int frameHeight = bar->height();
  int fillWidth = frameWidth;
  int fillHeight = frameHeight;
  bool fillInside = bar->maximum() ? true : false;

  LinearGradient background;
  if (orientation == Horizontal)
    {
      frameY = (bar->height() - _progressbarSize.height()) / 2;
      frameHeight = _progressbarSize.height();
      background.setPatternCoordinates(0, frameY, 0, frameY + frameHeight);
    }
  else
    {
      frameX = (bar->width() - _progressbarSize.height()) / 2;
      frameWidth = _progressbarSize.height();
      background.setPatternCoordinates(frameX, 0, frameX + frameWidth, 0);
    }

  // Set frame brush & pen
  background.addStop(_palette.getGroup(state)._backgroundBottom, 0);
  background.addStop(_palette.getGroup(state)._backgroundTop, 1);
  brush->setGradient(background);
  pen->setColor(_palette.getGroup(state)._borderBottom);
  pen->setLineWidth(1);

  // Draw outer frame
  painter->drawRoundRectangle(frameX, frameY, frameWidth, frameHeight,
      _progressbarRadius, Painter::StrokeAndFill);

  // draw inner frame
  if (state & DisabledState)
    pen->setColor(_palette._disabled._borderMid);
  else
    pen->setColor(_palette._default._borderMid);
  painter->drawRoundRectangle(frameX + 1, frameY + 1, frameWidth - 2,
      frameHeight - 2, _progressbarRadius, Painter::StrokePath);

  if (bar->range() != 0)
    {
      if (bar->value())
        {
          painter->drawRoundRectangle(frameX + 1, frameY + 1, frameWidth - 2,
              frameHeight - 2, _progressbarRadius, Painter::ClipPath);

          LinearGradient fillGradient;
          if (orientation == Horizontal)
            {
              fillWidth = (bar->value() * frameWidth) / bar->range();
              fillGradient.setPatternCoordinates(0, frameY, 0,
                  frameY + frameHeight);
            }
          else
            {
              fillHeight = (bar->value() * frameHeight) / bar->range();
              fillGradient.setPatternCoordinates(frameX, 0,
                  frameX + frameWidth, 0);
            }
          fillGradient.addStop(_palette.getGroup(state)._fillTop, 0);
          fillGradient.addStop(_palette.getGroup(state)._fillMid, 0.5);
          fillGradient.addStop(_palette.getGroup(state)._fillBottom, 1);
          brush->setGradient(fillGradient);

          if (orientation == Horizontal)
            painter->drawRectangle(frameX, frameY, fillWidth, frameHeight,
                Painter::FillPath);
          else
            painter->drawRectangle(frameX, frameY + frameHeight - fillHeight,
                frameWidth, fillHeight, Painter::FillPath);

          painter->resetClip();
        }
      if (orientation == Horizontal)
        {
          char percent[6];
          brush->setColor(_palette.getGroup(state)._text);
          sprintf(percent, "%% %d", bar->percentage());
          painter->setLayoutAlignment(PANGO_ALIGN_CENTER);
          painter->drawText(percent, frameX, frameY, frameWidth,
              frameHeight, AlignVCenter);
        }
    }
}

void
Designer::drawSlider(Painter* painter, Slider* slider)
{
  Pen* pen = painter->getPen();
  Brush* brush = painter->getBrush();
  const WidgetState state = slider->state();

  // Calculate positions and dimensions
  Rectangle frameGeometry = slider->indicatorRegion();
  int frameX = frameGeometry.x();
  int frameY = frameGeometry.y();
  int frameWidth = frameGeometry.width();
  int frameHeight = frameGeometry.height();
  int fillWidth = frameWidth;
  int fillHeight = frameHeight;
  Orientation orientation = slider->orientation();

  // Set frame brush & pen
  LinearGradient background;
  if (orientation == Horizontal)
    {
      frameY = frameGeometry.y() + 5;
      frameHeight = 10;
      background.setPatternCoordinates(0, frameY, 0, frameY + frameHeight);
    }
  else
    {
      frameX = frameGeometry.x() + 5;
      frameWidth = 10;
      background.setPatternCoordinates(frameX, 0, frameX + frameWidth, 0);
    }
  background.addStop(_palette._default._backgroundBottom, 0);
  background.addStop(_palette._default._backgroundTop, 1);
  brush->setGradient(background);
  pen->setColor(_palette.getGroup(state)._borderBottom);
  pen->setLineWidth(1);

  // Draw outer frame
  painter->drawRoundRectangle(frameX, frameY, frameWidth, frameHeight,
      _sliderRadius, Painter::StrokeAndFill);

  // draw inner frame
  if (state & DisabledState)
    pen->setColor(_palette._disabled._borderMid);
  else
    pen->setColor(_palette._default._borderMid);
  painter->drawRoundRectangle(frameX + 1, frameY + 1, frameWidth - 2,
      frameHeight - 2, _sliderRadius, Painter::StrokePath);

  if (slider->range() != 0)
    {
      if (slider->value())
        {
          painter->drawRoundRectangle(frameX + 1, frameY + 1, frameWidth - 2,
              frameHeight - 2, _progressbarRadius, Painter::ClipPath);

          LinearGradient fillGradient;
          if (orientation == Horizontal)
            {
              fillWidth = (slider->value() * frameWidth) / slider->range();
              fillGradient.setPatternCoordinates(0, frameY, 0,
                  frameY + frameHeight);
            }
          else
            {
              fillHeight = (slider->value() * frameHeight) / slider->range();
              fillGradient.setPatternCoordinates(frameX, 0,
                  frameX + frameWidth, 0);
            }
          fillGradient.addStop(_palette.getGroup(state)._fillTop, 0);
          fillGradient.addStop(_palette.getGroup(state)._fillMid, 0.5);
          fillGradient.addStop(_palette.getGroup(state)._fillBottom, 1);
          brush->setGradient(fillGradient);

          if (orientation == Horizontal)
            painter->drawRectangle(frameX, frameY, fillWidth, frameHeight,
                Painter::FillPath);
          else
            painter->drawRectangle(frameX, frameY + frameHeight - fillHeight,
                frameWidth, fillHeight, Painter::FillPath);

          painter->resetClip();
        }

      // Set indicator brush and pen
      Rectangle indicatorGeometry = slider->indicatorGeometry();

      LinearGradient indicatorBackground;

      if (orientation == Horizontal)
        indicatorBackground.setPatternCoordinates(0, indicatorGeometry.y(), 0,
            indicatorGeometry.bottom());
      else
        indicatorBackground.setPatternCoordinates(indicatorGeometry.x(), 0,
            indicatorGeometry.right(), 0);

      indicatorBackground.addStop(_palette.getGroup(state)._backgroundTop, 0);
      indicatorBackground.addStop(_palette.getGroup(state)._backgroundBottom, 1);
      brush->setGradient(indicatorBackground);

      pen->setColor(_palette.getGroup(state)._borderBottom);
      pen->setLineWidth(1);

      // draw indicator frame
      painter->drawRoundRectangle(indicatorGeometry.x(), indicatorGeometry.y(),
          indicatorGeometry.width(), indicatorGeometry.height(), _sliderRadius,
          Painter::StrokeAndFill);

      // draw focus ring
      if (state & FocusedState)
        pen->setColor(_palette._focusTop);
      else
        pen->setColor(_palette.getGroup(state)._borderMid);

      painter->drawRoundRectangle(indicatorGeometry.x() + 1,
          indicatorGeometry.y() + 1, indicatorGeometry.width() - 2,
          indicatorGeometry.height() - 2, _sliderRadius, Painter::StrokePath);

      // FIXME: Slider grid image is not shown properly if slider is filled inside
      // draw indicator grid image
      bool pressed = state & PressedState;
      if (orientation == Horizontal)
        {
          DFBSurfaceBlittingFlags flags =
              pressed ? (DFBSurfaceBlittingFlags) (DSBLIT_BLEND_ALPHACHANNEL
                  | DSBLIT_FLIP_HORIZONTAL) : DSBLIT_BLEND_ALPHACHANNEL;
          painter->drawImage(_grid, indicatorGeometry, flags);
        }
      else
        {
          DFBSurfaceBlittingFlags flags =
              pressed ? (DFBSurfaceBlittingFlags) (DSBLIT_BLEND_ALPHACHANNEL
                  | DSBLIT_ROTATE90)
                  : (DFBSurfaceBlittingFlags) (DSBLIT_BLEND_ALPHACHANNEL
                      | DSBLIT_ROTATE270);
          painter->drawImage(_grid, indicatorGeometry, flags);
        }
    }
}

void
Designer::drawScrollBar(Painter* painter, ScrollBar* bar)
{
  Pen* pen = painter->getPen();
  Brush* brush = painter->getBrush();
  const WidgetState state = bar->state();

  // Calculate positions and dimensions
  Rectangle frameGeometry = bar->indicatorRegion();
  int frameX = frameGeometry.x();
  int frameY = frameGeometry.y();
  int frameWidth = frameGeometry.width();
  int frameHeight = frameGeometry.height();

  // Set frame brush & pen
  brush->setColor(_palette.getGroup(state)._baseAlt);
  pen->setLineWidth(1);
  pen->setColor(_palette.getGroup(state)._borderTop);
  painter->drawRectangle(frameX, frameY, frameWidth, frameHeight,
      Painter::StrokeAndFill);

  if (bar->range())
    {
      pen->setColor(_palette.getGroup(state)._borderTop);
      brush->setColor(_palette.getGroup(state)._backgroundTop);
      Rectangle indicatorGeometry = bar->indicatorGeometry();
      painter->drawRectangle(indicatorGeometry.x(), indicatorGeometry.y(),
          indicatorGeometry.width(), indicatorGeometry.height(),
          Painter::StrokeAndFill);
    }
}

void
Designer::drawDirectionalButton(Painter* painter, DirectionalButton* button)
{
  //  Pen* pen = painter->getPen();
  //  Brush* brush = painter->getBrush();
  //  const WidgetState state = button->getState();
  //
  //  // Set Brush & Pen
  //  brush->setColor(_palette.getGroup(state)._backgroundTop);
  //  pen->setColor(_palette.getGroup(state)._borderTop);
  //  pen->setLineWidth(1);
  //
  //  // Paint button frame using brush and pen
  //  painter->drawRectangle(0, 0, button->width(), button->height(),
  //      Painter::StrokeAndFill);
  //  bool pressed = button->pressed();
  //  switch (button->direction())
  //    {
  //  case Up:
  //    painter->drawImage(_scrollArrow, pressed, pressed);
  //    break;
  //  case Down:
  //    painter->drawImage(
  //        _scrollArrow,
  //        pressed,
  //        pressed,
  //        (DFBSurfaceBlittingFlags) (DSBLIT_BLEND_ALPHACHANNEL | DSBLIT_ROTATE180));
  //    break;
  //  case Left:
  //    painter->drawImage(
  //        _scrollArrow,
  //        pressed,
  //        pressed,
  //        (DFBSurfaceBlittingFlags) (DSBLIT_BLEND_ALPHACHANNEL | DSBLIT_ROTATE270));
  //    break;
  //  case Right:
  //    painter->drawImage(_scrollArrow, pressed, pressed,
  //        (DFBSurfaceBlittingFlags) (DSBLIT_BLEND_ALPHACHANNEL | DSBLIT_ROTATE90));
  //    break;
  //    }
}

void
Designer::drawGroupBox(Painter* painter, GroupBox* box)
{
  Size s = box->getTitleTextSize();
  int y = s.height() / 2;
  drawFrame(painter, box, 0, y, box->width(), box->height() - y);

  int x = (box->width() - s.width()) / 2;
  drawFrame(painter, box, x - 10, 0, s.width() + 20,
      s.height() + 2 * _borderWidth, true);
  painter->setFont(_titleFont);
  painter->getBrush()->setColor(_palette.getGroup(box->state())._text);
  painter->drawText(box->title(), x, _borderWidth);
}

void
Designer::drawTabPanel(Painter* painter, TabPanel* panel)
{
}

void
Designer::drawTabPanelButton(Painter* painter, TabPanelButton* button)
{
}

void
Designer::drawAppFrame(Painter* painter, Application* app)
{
  painter->getBrush()->setColor(_palette._baseTop);
  painter->drawRectangle(0, 0, app->width(), app->height(), Painter::FillPath);
}

void
Designer::drawFrame(Painter* painter, const BorderBase* widget, int x, int y,
    int width, int height, bool filled, Corners corners)
{
  //  if (widget->borderStyle() == NoBorder || widget->borderWidth() == 0)
  //    return;
  //  // set width and height
  //  if (width == 0)
  //    width = widget->width();
  //  if (height == 0)
  //    height = widget->height();
  //
  //  Pen* pen = painter->getPen();
  //  // Draw plain frame
  //  if (widget->borderStyle() == PlainBorder)
  //    {
  //      pen->setLineWidth(widget->borderWidth());
  //      pen->setColor(_palette.getGroup(widget->getState())._borderBottom);
  //      painter->drawRectangle(x, y, width, height);
  //    }
  //  else if (widget->borderStyle() == RaisedBorder)
  //    {
  //      //      widget->setBorderWidth(4);
  //      int lw = widget->borderWidth();
  //      // Set pen
  //      pen->setLineWidth(lw);
  //      Color c = _palette.getGroup(widget->getState())._borderBottom;
  //      pen->setColor(c);
  //      painter->drawRectangle(x, y, width, height);
  //      // XXX Complete SimpleDesigner::RaisedFrame using polygons...
  //      pen->setColor(c - 0.2);
  //      painter->drawLine(x, height - lw + 2, width, height - lw + 2);
  //      painter->drawLine(width - 2, y, width - 2, height - lw + 2);
  //    }
  //  else if (widget->borderStyle() == SunkenBorder)
  //    {
  //    }
  //  else if (widget->borderStyle() == StyledBorder)
  //    {
  //      // set pen
  //      pen->setColor(_palette._default._borderBottom);
  //      pen->setLineWidth(1);
  //
  //      // draw frame
  //      painter->drawRoundRectangle(x, y, width, height, _frameBorderRadius,
  //          Painter::StrokePath);
  //      // draw focus ring
  //      pen->setColor(_palette._default._borderMid);
  //      painter->drawRoundRectangle(x + 1, y + 1, width - 2, height - 2,
  //          _frameBorderRadius, Painter::StrokePath);
  //    }
}

void
Designer::drawTitledFrame(Painter* painter, const BorderBase* widget, int x,
    int y, int width, int height, int titleWidth, int titleHeight, bool filled)
{
}

void
Designer::drawDialog(Painter* painter, Dialog* dialog)
{
}

void
Designer::drawLineEdit(Painter* painter, LineEdit* input,
    const Rectangle& selection)
{
}

void
Designer::drawInputCursor(Painter* painter, LineEdit* input,
    const Rectangle& cursor, bool show, const Rectangle& selection)
{
  Brush* brush = painter->getBrush();
  const WidgetState state = input->state();

  if (selection.isNull())
    {
      brush->setColor(_palette.getGroup(state)._base);
      painter->drawRectangle(cursor, Painter::FillPath);
    }
  else // draw selection
    {
      brush->setColor(_palette._focusTop);
      painter->drawRectangle(cursor, Painter::FillPath);
    }

  // draw button's pango layout
  if (!input->text().empty())
    {
      brush->setColor(_palette.getGroup(state)._baseText);
      painter->drawLayout(input->layout(), input->font(),
          input->layoutPosition());
    }

  if (show)
    {
      brush->setColor(Color(1, 0, 0, 0.5));
      painter->drawRectangle(cursor, Painter::FillPath);
    }
}

void
Designer::drawLine(Painter* painter, Line* line)
{
  // set width and height
  int x2 = 0;
  int y2 = 0;
  int width = 0;
  int height = 0;

  if (line->orientation() == Line::Horizontal)
    {
      y2 = 1;
      width = line->width();
      height = 1;
    }
  else
    {
      x2 = 1;
      width = 1;
      height = line->height();
    }

  Pen* pen = painter->getPen();
  pen->setLineWidth(1);
  pen->setColor(_palette._default._borderBottom);
  painter->drawRectangle(0, 0, width, height, Painter::StrokePath);
  pen->setColor(_palette._default._borderMid);
  painter->drawRectangle(x2, y2, width, height, Painter::StrokePath);
}

Size
Designer::sizeHint(WidgetSizeHintType type) const
{
  switch (type)
    {
  case ButtonSizeHint:
    return _buttonSize;
  case CheckBoxSizeHint:
    return _checkboxSize;
  case RadioButtonSizeHint:
    return _radiobuttonSize;
  case ProgressBarSizeHint:
    return _progressbarSize;
  case SliderSizeHint:
    return _sliderSize;
  case ScrollBarButtonSizeHint:
    return Size(_scrollbarButtonWidth, _scrollbarSize.height());
  case ScrollBarSizeHint:
    return _scrollbarSize;
  case TabPanelSizeHint:
    return _tabPanelSize;

  default:
    return Size();
    }
}

int
Designer::hint(WidgetHint type) const
{
  switch (type)
    {
  case BorderWidth:
    return _borderWidth;

  case FrameBorderRadius:
    return _frameBorderRadius;

  case TextInputFrameRadius:
    return _textInputFrameRadius;

  case DialogShadow:
    return _dialogShadow;

  case ButtonHeight:
    return _buttonSize.height();
  case ButtonWidth:
    return _buttonSize.width();
  case ButtonOffset:
    return _buttonOffset;
  case ButtonRadius:
    return _buttonRadius;
  case ButtonCheckedIndicatorWidth:
    return _buttonCheckedIndicatorWidth;

  case RadioHeight:
    return _radiobuttonSize.height();
  case RadioWidth:
    return _radiobuttonSize.width();
  case RadioOffset:
    return _radiobuttonOffset;

  case CheckBoxHeight:
    return _checkboxSize.height();
  case CheckBoxWidth:
    return _checkboxSize.width();
  case CheckBoxOffset:
    return _checkboxOffset;
  case CheckBoxRadius:
    return _checkboxRadius;

  case ComboBoxRadius:
    return _comboboxRadius;
  case ComboBoxButtonWidth:
    return _comboboxButtonWidth;

  case ProgressBarHeight:
    return _progressbarSize.height();
  case ProgressBarWidth:
    return _progressbarSize.width();
  case ProgressBarRadius:
    return _progressbarRadius;

  case SliderHeight:
    return _sliderSize.height();
  case SliderWidth:
    return _sliderSize.width();
  case SliderRadius:
    return _sliderRadius;
  case SliderFrameHeight:
    return _sliderFrameHeight;

  case ScrollBarHeight:
    return _scrollbarSize.height();
  case ScrollBarWidth:
    return _scrollbarSize.width();
  case ScrollBarRadius:
    return _scrollbarRadius;

  case ScrollBarButtonHeight:
    return _scrollbarSize.height();
  case ScrollBarButtonWidth:
    return _scrollbarButtonWidth;

  case TabPanelHeight:
    return _tabPanelSize.height();
  case TabPanelWidth:
    return _tabPanelSize.width();
  case TabPanelButtonHeight:
    return _tabPanelButtonHeight;

  default:
    return -1;
    }
}

Font*
Designer::defaultFont(DesignerFontType type) const
{
  switch (type)
    {
  case ButtonFont:
    return _buttonFont;
  case TitleFont:
    return _titleFont;
  case InputFont:
    return _inputFont;
  default:
    return _defaultFont;
    }
}

Image*
Designer::defaultIcon(DesignerIconType type) const
{
  switch (type)
    {
  case InformationMessage:
    return _dialogInfo;
  case QuestionMessage:
    return _dialogQuestion;
  case WarningMessage:
    return _dialogWarning;
  case CriticalMessage:
    return _dialogCritical;
    }
}

void
Designer::drawImage(Painter* painter, Image* image, const Rectangle& rect,
    bool disabled, const DFBSurfaceBlittingFlags& flags)
{
  painter->drawImage(image, rect, flags, disabled, 127, 127, 127, 127);
}

void
Designer::drawImage(Painter* painter, Image* image, int x, int y,
    bool disabled, const DFBSurfaceBlittingFlags& flags)
{
  painter->drawImage(image, x, y, flags, disabled, 127, 127, 127, 127);
}
