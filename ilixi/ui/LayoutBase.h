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

#ifndef ILIXI_LAYOUT_H_
#define ILIXI_LAYOUT_H_

#include "types/Margin.h"
#include "types/RadioGroup.h"
#include "ui/Widget.h"

namespace ilixi
{
  //! This base class provides an absolute layout.
  /*!
   * Each container stores its child widgets inside a layout. A layout's responsibility
   * is to set the geometry of child widgets and arrange them inside its canvas.
   */
  class LayoutBase : public Widget
  {
    friend class ContainerBase;
    friend class ScrollArea;
  public:
    /*!
     * Constructor.
     *
     * @param parent Pointer to parent Widget.
     */
    LayoutBase(Widget* parent = 0);

    /*!
     * Destructor
     */
    virtual
    ~LayoutBase();

    virtual int
    heightForWidth(int width) const;

    virtual Size
    preferredSize() const;

    /*!
     * Returns the bounding rectangle around children.
     */
    Rectangle
    contentsRect();

    /*!
     * Returns the number of items in the layout.
     */
    int
    count();

    /*!
     * Returns container's margin.
     */
    Margin
    margin() const;

    /*!
     * Returns spacing between widgets.
     */
    unsigned int
    spacing() const;

    /*!
     * Sets spacing between widgets.
     *
     * @param spacing
     */
    void
    setSpacing(unsigned int spacing);

    virtual void
    setVisible(bool visible = true);

    /*!
     * Adds a new widget to this layout.
     * New widget receives focus is if it is focusable.
     *
     * @param widget Pointer to widget.
     */
    virtual void
    addWidget(Widget* widget);

    /*!
     * Aligns widgets inside layout.
     * You should reimplement this method in your layout classes.
     */
    virtual void
    tile();

    virtual void
    doLayout();

    virtual void
    paint(const Rectangle& targetArea);

  protected:

    //! This struct is used by Layouts during tiling as a cache for each widget.
    struct ActiveWidget
    {
      Widget* widget;
      Size size;
    };

    typedef std::list<ActiveWidget> ActiveWidgetList;
    typedef ActiveWidgetList::iterator ActiveWidgetListIterator;

    //! This list is updated every time the layout is tiling.
    ActiveWidgetList _activeWidgetList;

    //! This flag specifies whether layout is modified since latest tiling operation.
    bool _layoutModified;
    //! This property holds spacing between widgets inside the layout.
    unsigned int _spacing;
    //! This property holds the number of expanding widgets.
    int _numExpanding;

    RadioGroup* _group;

    void
    updateChildrenFrameGeometry();

    /*!
     * Does nothing.
     *
     * @param rect
     */
    void
    compose(const Rectangle& rect);

    /*!
     * Create a list of active widget i.e. visible and not ignored.
     * Set up preferred sizes and update widths using heighForWidth if possible.
     */
    virtual void
    updateActiveWidgetList();

  private:
    /*!
     * If pointer event occurs over widget handle it and return true.
     *
     * @param pointerEvent Reference to pointer event coming from parent widget.
     * @return True if event is consumed.
     */
    bool
    consumePointerEvent(const PointerEvent& pointerEvent);
  };

  class Spacer : public Widget
  {
  public:
    Spacer(Orientation orientation = Horizontal, Widget* parent = 0);

    ~Spacer();

    virtual Size
    preferredSize() const;

    void
    setOrientation(Orientation orientation);

    virtual void
    paint(const Rectangle& targetArea);

  private:
    Orientation _orientation;

    virtual void
    compose(const Rectangle& rect);
  };
}
#endif /* ILIXI_LAYOUT_H_ */
