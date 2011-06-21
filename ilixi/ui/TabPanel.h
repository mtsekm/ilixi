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

#ifndef ILIXI_TABPANEL_H_
#define ILIXI_TABPANEL_H_

#include "ui/Button.h"
#include "ui/BorderBase.h"
#include <vector>

namespace ilixi
{

  class TabPanelButton : public Button
  {
  public:
    TabPanelButton(std::string label, Widget* parent);

    virtual
    ~TabPanelButton();

    virtual Size
    preferredSize() const;

    virtual void
    toggleState();

  protected:
    void
    compose(const Rectangle& rect);

    virtual void
    updateTextLayoutGeometry();
  };

  class TabPanel : public BorderBase
  {
  public:
    TabPanel(Widget* parent = 0);

    virtual
    ~TabPanel();

    /*!
     * Returns a height for tab panel given a width.
     *
     * @param width Desired width of tab panel.
     */
    virtual int
    heightForWidth(int width) const;

    /*!
     * Calculates the preferred size of contents and returns an updated size using canvas, margins and borders.
     */
    virtual Size
    preferredSize() const;

    int
    count() const;

    void
    clear();

    int
    currentIndex() const;

    Widget*
    currentWidget() const;

    int
    indexOf(Widget* widget) const;

    int
    margin() const;

    Image*
    pageIcon(int index) const;

    bool
    pageEnabled(int index) const;

    std::string
    pageLabel(int index) const;

    void
    addPage(Widget* widget, std::string label, std::string iconPath = "");

    void
    insertPage(int index, Widget* widget, std::string label,
        std::string iconPath = "");

    void
    removePage(int index);

    void
    setCurrentPage(int index);

    void
    setMargin(int margin);

    void
    setPageIcon(int index, std::string iconPath);

    void
    setPageLabel(int index, std::string label);

    void
    setPageEnabled(int index, bool enabled);

    virtual void
    doLayout();

  protected:
    virtual void
    compose(const Rectangle& rect);

  private:
    //! This property stores current page index.
    int _currentIndex;
    //! This property stores the margins around pages.
    int _margin;

    struct TabPage
    {
      Widget* widget;
      TabPanelButton* button;
      Size widgetSize;
    };

    typedef std::vector<TabPage> PageList;
    PageList _pages;

    virtual void
    updateChildrenFrameGeometry();

    virtual void
    paintChildren(const Rectangle& rect);
  };
}

#endif /* ILIXI_TABPANEL_H_ */
