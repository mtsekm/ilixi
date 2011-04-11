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

#ifndef ILIXI_WIDGET_H_
#define ILIXI_WIDGET_H_

#include "graphics/Surface.h"
#include "graphics/Designer.h"
#include "types/Enums.h"
#include "types/Event.h"
#include <list>
#include <string>
#include <sigc++/signal.h>

namespace ilixi
{
  class UIManager;
  class Window;

  //! Base class of all UI objects.
  class Widget : virtual public sigc::trackable
  {
    friend class Application; // sets UIManager and _designer.
    friend class Dialog; // sets UIManager
    friend class UIManager;
  public:
    /*!
     * Constructor.
     *
     *
     *
     * @param parent widget's parent.
     */
    Widget(Widget* parent = 0);

    /*!
     * Destructor.
     *
     * Releases its surface. Deletes children widgets. Removes itself
     * and its children from the UIManager.
     */
    virtual
    ~Widget();

    /*!
     * Returns x coordinate of the widget relative to its parent.
     */
    int
    x() const;

    /*!
     * Returns y coordinate of the widget relative to its parent.
     */
    int
    y() const;

    /*!
     * Returns absolute x coordinate of the widget in window coordinates.
     * Value depends on widget's geometry flag and whether it has a parent.
     */
    int
    absX() const;

    /*!
     * Returns absolute y coordinate of the widget in window coordinates.
     * Value depends on widget's geometry flag and whether it has a parent.
     */
    int
    absY() const;

    /*!
     * Returns position (x, y) of the widget's surface relative to its parent widget.
     */
    Point
    position() const;

    /*!
     * Returns height of widget's surface.
     */
    int
    height() const;

    /*!
     * Returns width of widget's surface.
     */
    int
    width() const;

    /*!
     *  Returns size (width, height) of widget's surface.
     */
    Size
    size() const;

    /*!
     * Returns absolute geometry of the widget.
     */
    Rectangle
    frameGeometry() const;

    /*!
     * Returns surface geometry of the widget.
     */
    Rectangle
    surfaceGeometry() const;

    /*!
     * Returns minimum height for widget.
     *
     * @sa setMinSize()
     */
    int
    minHeight() const;

    /*!
     * Returns minimum width for widget.
     *
     * @sa setMinSize()
     */
    int
    minWidth() const;

    /*!
     * Returns the widget's minimum size.
     */
    Size
    minimumSize() const;

    /*!
     * Returns maximum height for widget.
     *
     * @sa setMaxSize()
     */
    int
    maxHeight() const;

    /*!
     * Returns maximum width for widget.
     *
     * @sa setMaxSize()
     */
    int
    maxWidth() const;

    /*!
     * Returns the widget's maximum size.
     */
    Size
    maximumSize() const;

    /*!
     * Returns the height of the widget given its width.
     *
     * This method is usually used by a layout strategy in order to shrink or enlarge
     * widgets if they allow it. By default this method returns -1 which indicates that
     * widget does not approve resizing on y-axis.
     *
     * If widget does not have FixedConstraint on y-axis and can shrink or enlarge on x-axis,
     * you should reimplement it in derived classes.
     *
     * @param width Proposed width for widget.
     *
     * @sa preferredSize()
     */
    virtual int
    heightForWidth(int width) const;

    /*!
     * Returns the preferred size for the widget.
     *
     * This method is used by a layout strategy in order to determine a sensible
     * initial size for widget. By default this method returns an empty Size() which
     * indicates that widget does not care about its size.
     *
     * You should reimplement this method if you want your widget to behave properly
     * inside containers and layouts.
     *
     * @sa heightForWidth()
     */
    virtual Size
    preferredSize() const;

    /*!
     * Returns true if widget is enabled.
     *
     * A widget is considered disabled if its parent is disabled.
     *
     * An enabled widget can receive focus and input events.
     */
    bool
    enabled() const;

    /*!
     * Returns true if widget is visible.
     *
     * A widget is considered invisible if its parent is invisible.
     *
     * A visible widget can receive focus and input events.
     */
    bool
    visible() const;

    /*!
     * Returns true if widget has focus.
     *
     * A widget can receive incoming key events, If it is enabled, visible and has focus.
     */
    bool
    hasFocus() const;

    /*!
     * Returns true if pointer is over widget.
     */
    bool
    exposed() const;

    /*!
     * Returns true if pointer is pressed over widget.
     */
    bool
    pressed() const;

    /*!
     * Returns current widget state.
     *
     * @sa enabled(), visible(), hasFocus()
     */
    WidgetState
    state() const;

    /*!
     * Returns true if widget is enabled, visible and has pointer input capability.
     */
    bool
    acceptsPointerInput() const;

    /*!
     * Returns true if widget is enabled, visible and has key input capability.
     */
    bool
    acceptsKeyInput() const;

    /*!
     * Returns input method flag for widget.
     *
     * This flag is used to determine which input events a widget is
     * capable of processing. By default a widget may not accept any input
     * events at all.
     */
    WidgetInputMethod
    inputMethod() const;

    /*!
     * Returns widget's resize constraint on x-axis.
     */
    virtual WidgetResizeConstraint
    hConstraint() const;

    /*!
     * Returns widget's resize constraint on y-axis.
     */
    virtual WidgetResizeConstraint
    vConstraint() const;

    /*!
     * Returns an interface to the current designer.
     *
     * @sa Designer
     */
    static Designer* const
    designer();

    /*!
     * Returns the pointer to the parent widget.
     * NULL if widget has no parent.
     */
    Widget*
    parent() const;

    /*!
     * Returns the pointer to widget's surface.
     */
    Surface*
    surface() const;

    /*!
     * Returns the pointer to widget's UIManager.
     */
    UIManager*
    uiManager() const;

    /*!
     * Sets the widget's surface x-coordinate (top-left) inside its parent.
     *
     * @param x coordinate for top-left point.
     */
    void
    setX(int x);

    /*!
     * Sets the widget's surface y-coordinate (top-left) inside its parent.
     *
     * @param y coordinate for top-left point.
     */
    void
    setY(int y);

    /*!
     * Moves widget to given coordinates inside its parent.
     *
     * @param x coordinate for top-left point.
     * @param y coordinate for top-left point.
     * @sa setX(), setY()
     */
    void
    moveTo(int x, int y);

    /*!
     * Sets the widget's surface width in pixels.
     *
     * @param width in pixels.
     */
    void
    setWidth(int width);

    /*!
     * Sets the widget's surface height in pixels.
     *
     * @param height in pixels
     */
    void
    setHeight(int height);

    /*!
     * Sets the widget's dimensions.
     *
     * @param width in pixels.
     * @param height in pixels
     */
    void
    setSize(int width, int height);

    /*!
     * Sets the widget's dimensions.
     *
     * @param size in pixels.
     */
    void
    setSize(const Size &size);

    /*!
     * Sets widget's surface geometry.
     *
     * @param x coordinate for top-left point.
     * @param y coordinate for top-left point.
     * @param width in pixels.
     * @param height in pixels
     */
    void
    setGeometry(int x, int y, int width, int height);

    /*!
     * Sets widget's minimum size.
     *
     * Widget's size can not be smaller than this.
     *
     * @param minSize in pixels.
     */
    void
    setMinimumSize(const Size &minSize);

    /*!
     * Sets widget's minimum size.
     *
     * Widget's size can not be smaller than this.
     *
     * @param minWidth in pixels.
     * @param minHeight in pixels
     */
    void
    setMinimumSize(int minWidth, int minHeight);

    /*!
     * Sets widget's maximum size.
     *
     * Widget's size can not be larger than this.
     *
     * @param maxSize in pixels.
     */
    void
    setMaximumSize(const Size &maxSize);

    /*!
     * Sets widget's maximum size.
     *
     * Widget's size can not be larger than this.
     *
     * @param maxWidth in pixels.
     * @param maxHeight in pixels
     */
    void
    setMaximumSize(int maxWidth, int maxHeight);

    /*!
     * Sets widget's horizontal resize constraint.
     *
     * @param constraint Default is NoConstraint.
     */
    void
    setHConstraint(WidgetResizeConstraint constraint = NoConstraint);

    /*!
     * Sets widget's vertical resize constraint.
     *
     * @param constraint Default is NoConstraint.
     */
    void
    setVConstraint(WidgetResizeConstraint constraint = NoConstraint);

    /*!
     * Sets widget's horizontal and vertical resize constraints.
     *
     * @param horizontal Default is NoConstraint.
     * @param vertical Default is NoConstraint.
     */
    void
    setConstraints(WidgetResizeConstraint horizontal = NoConstraint,
        WidgetResizeConstraint vertical = NoConstraint);

    /*!
     * Sets widget's state to enabled and calls update() if widget is disabled.
     */
    void
    setEnabled();

    /*!
     * Sets widget's enabled flag to false and calls update() if widget is enabled.
     */
    void
    setDisabled();

    /*!
     * Sets widget's visibility. Hidden widgets are not drawn whether they
     * are changed or not.
     *
     * @param visible False if Widget is hidden, true otherwise. Default true.
     */
    virtual void
    setVisible(bool visible = true);

    /*!
     * Assigns key input focus to widget if the widget accepts key inputs.
     *
     * @sa acceptsKeyInput()
     */
    void
    setFocus();

    /*!
     * Sets widget's input method.
     *
     * @param method
     */
    void
    setInputMethod(WidgetInputMethod method);

    /*!
     * This method is used to enable or disable pointer motion events.
     * By default pointer tracking is disabled.
     *
     * @param pointerTracking if true widget acquires pointer tracking capability.
     */
    void
    setPointerTracking(bool pointerTracking);

    /*!
     * Invokes widget's compose method only if widget is visible and target rectangle
     * intersects with widget's paint geometry.
     *
     * Warning: You should not override this method unless you know what you are doing.
     *
     * @param targetArea
     * @param forceRepaint If true widget will paint
     *
     * @sa compose()
     */
    virtual void
    paint(const Rectangle& targetArea, bool forceRepaint = false);

    /*!
     * Repaints widget without any clipping. Result will not be displayed
     * until surface is flipped if it is double-buffered.
     *
     * @sa update()
     */
    void
    repaint();

    /*!
     * Repaints widget and updates display.
     *
     * Widgets intersecting this widget's frame geometry
     * are also painted.
     */
    void
    update();

    /*!
     * This method will repaint inside given rectangle.
     *
     * @param rect Bounding rectangle to update.
     */
    void
    update(const Rectangle& rect);

    /*!
     * Invalidates parent layout and force it to tile again.
     */
    virtual void
    doLayout();

    /*!
     * Maps the given rectangle in absolute coordinates to local coordinates.
     *
     * @param rect Rectangle in absolute coordinates.
     */
    Rectangle
    mapToSurface(const Rectangle& rect) const;

    /*!
     * Maps the given values in absolute coordinates to local coordinates.
     *
     * @param x absolute coordinate in pixels.
     * @param y absolute coordinate in pixels.
     * @param width in pixels.
     * @param height in pixels.
     */
    Rectangle
    mapToSurface(int x, int y, int width, int height) const;

    /*!
     * Maps the given point in absolute coordinates to local coordinates.
     *
     * @param point in absolute coordinates.
     */
    Point
    mapToSurface(const Point& point) const;

    /*!
     * Maps the given rectangle in local coordinates to absolute coordinates.
     *
     * @param rect Rectangle in local coordinates.
     */
    Rectangle
    mapFromSurface(const Rectangle& rect) const;

    /*!
     * Maps the given values in local coordinates to absolute coordinates.
     *
     * @param x local coordinate in pixels.
     * @param y local coordinate in pixels.
     * @param width in pixels.
     * @param height in pixels.
     */
    Rectangle
    mapFromSurface(int x, int y, int width, int height) const;

    /*!
     * Maps the given point in local coordinates to absolute coordinates.
     *
     * @param point in local coordinates.
     */
    Point
    mapFromSurface(const Point& point) const;

    /*!
     * Consumes a pointer event and calls event handling methods if the event is inside widget.
     *
     * @param pointerEvent in frame (absolute) coordinates.
     *
     * @return True if event is consumed, false otherwise.
     */
    virtual bool
    consumePointerEvent(const PointerEvent& pointerEvent);

    /*!
     * Calls appropriate key event handler method and returns true.
     *
     * @param keyEvent Reference to key event.
     */
    virtual bool
    consumeKeyEvent(const KeyEvent& keyEvent);

    /*!
     * This signal is emitted before widget is painted if its geometry is modified.
     * By default widget's updateFrameGeometry() method is connected to this signal.
     */
    sigc::signal<void> sigGeometryUpdated;

  protected:
    /*!
     * Surface geometry stores widget's relative position (x, y) and size (width, height) of its surface.
     */
    Rectangle _surfaceGeometry;

    /*!
     * Frame geometry stores widget's absolute position (x, y) and size (width, height) of its visible
     * rectangle.
     *
     * Primary use of frame geometry is to determine if a pointer event should be consumed by the widget.
     * If x and y coordinates of an event are inside a widget's absolute geometry, event is consumed and the widget's input
     * events are executed accordingly. Note that if the event coordinates are contained by more than
     * one widget, events are consumed by widget which is on top.
     *
     * In most cases surface geometry and absolute geometry are equal, e.g. a Button inside a Frame. However,
     * widgets which have their own surfaces can keep independent geometries to specify a visible region. For example,
     * a widget inside a ScrollArea could have a larger surface size than its frame size.
     * In that case, the frame's absolute size is used as a clip rectangle during blit operations and events will be
     * consumed only if they are inside the visible region.
     */
    Rectangle _frameGeometry;

    /*!
     * This property holds widget's current state, e.g. Pressed or Exposed. It is set to DefaultState by default.
     */
    WidgetState _state;

    /*!
     * This property controls the allocation of widget's surface and specifies how its
     * frame and surface geometry should be used. It is set to DefaultDescription by default.
     */
    SurfaceDescription _surfaceDesc;

    /*!
     * This property stores widget's input capabilities. It is set to NoInput by default.
     */
    WidgetInputMethod _inputMethod;

    typedef std::list<Widget*> WidgetList;
    typedef WidgetList::iterator WidgetListIterator;
    typedef WidgetList::const_iterator WidgetListConstIterator;
    typedef WidgetList::reverse_iterator WidgetListReverseIterator;
    typedef WidgetList::const_reverse_iterator WidgetListConstReverseIterator;

    //! This list holds the children of the widget.
    WidgetList _children;

    /*!
     * Adds a child to this widget.
     *
     * Child is owned by the widget.
     *
     * @param child widget.
     */
    void
    addChild(Widget* child);

    /*!
     * If given widget is a child, deletes widget and removes it from children list.
     *
     * @param child widget.
     */
    void
    removeChild(Widget* child);

    /*!
     * Puts the child on the front of the children list.
     *
     * @param child of this widget.
     */
    void
    raiseChildToFront(Widget* child);

    /*!
     * Puts the child on the back of the children list.
     *
     * @param child of this widget.
     */
    void
    lowerChildToBottom(Widget* child);

    /*!
     * Raises the child by one within the children list.
     *
     * @param child of this widget.
     */
    void
    raiseChild(Widget* child);

    /*!
     * Lowers the child by one within the children list.
     *
     * @param child of this widget.
     */
    void
    lowerChild(Widget* child);

    /*!
     * Paints children.
     *
     * @param rect bounding rectangle in absolute coordinates.
     */
    virtual void
    paintChildren(const Rectangle& rect);

    /*!
     * Sets widget's SurfaceModified geometry flag.
     */
    void
    setSurfaceGeometryModified();

    /*!
     * This method is called within widget's paint method in order to initialise or
     * modify widget's surface and update the geometry of widget's frame and children.
     */
    void
    updateSurface();

    /*!
     * This method updates widget's absolute geometry and it is called when
     * sigGeometryUpdated is triggered.
     */
    virtual void
    updateFrameGeometry();

    /*!
     * Flips area inside surface.
     *
     * @param rect bounding rectangle.
     */
    void
    flip(const Rectangle& rect);

    /*!
     * This method is used to blit a widget's surface (source) onto this widget's surface.
     *
     * @param widget Source.
     * @param clip clip rectangle
     * @param x X-coordinate
     * @param y Y-coordinate
     */
    void
    blit(Widget* widget, const Rectangle& clip, int x, int y);

    /*!
     * Draws a widget on its surface.
     *
     * Reimplement this method in derived classes.
     */
    virtual void
    compose(const Rectangle& rect)=0;

    /*!
     * This method is called if a key is pressed while the widget has focus.
     *
     * @param keyEvent
     */
    virtual void
    keyDownEvent(const KeyEvent& keyEvent);

    /*!
     * This method is called if a key is released while the widget has focus.
     *
     * @param keyEvent
     */
    virtual void
    keyUpEvent(const KeyEvent& keyEvent);

    /*!
     * This method is called if pointer's button is pressed over widget.
     *
     * @param pointerEvent
     */
    virtual void
    pointerButtonDownEvent(const PointerEvent& pointerEvent);

    /*!
     * This method is called if pointer's button is released over widget.
     *
     * @param pointerEvent
     */
    virtual void
    pointerButtonUpEvent(const PointerEvent& pointerEvent);

    /*!
     * This method is called every time the pointer is moved over widget.
     *
     * @param pointerEvent
     */
    virtual void
    pointerMotionEvent(const PointerEvent& pointerEvent);

    /*!
     * This method is called if pointer's wheel is rotated over widget.
     *
     * @param pointerEvent
     */
    virtual void
    pointerWheelEvent(const PointerEvent& pointerEvent);

    /*!
     * This method is called when widget receives key input focus.
     */
    virtual void
    focusInEvent();

    /*!
     * This method is called when widget loses key input focus.
     */
    virtual void
    focusOutEvent();

    /*!
     * This method is called when pointer enters widget's surface.
     *
     * @param pointerEvent
     */
    virtual void
    enterEvent(const PointerEvent& pointerEvent);

    /*!
     * This method is called when pointer leaves widget's surface.
     *
     * @param pointerEvent
     */
    virtual void
    leaveEvent(const PointerEvent& pointerEvent);

    /*!
     * Given a rectangle calculates an intersection for painting.
     *
     * @param rect Area to paint.
     * @param forceRepaint If true whole surface is returned.
     * @return An intersection on widget's surface.
     */
    Rectangle
    getIntersectionForPaint(const Rectangle& rect, bool forceRepaint);

  private:
    //! widget's parent.
    Widget* _parent;

    //! widget's surface.
    Surface* _surface;

    //! Pointer to UIManager of this widget.
    UIManager* _uiManager;

    //! This property defines widget's behaviour inside a layout. Default is NoConstraint.
    WidgetResizeConstraint _horizontalResizeConstraint;

    //! This property defines widget's behaviour inside a layout. Default is NoConstraint.
    WidgetResizeConstraint _verticalResizeConstraint;

    //! This property holds the widget's minimum allowed size that is specified by the user.
    /*!
     * Note that minimum size overrides layout resize constraints.
     *
     * In order to set a minimum size use setMinimumSize() member method. If you wish to
     * disable minimum size for width or height use 0 for that value.
     */
    Size _minSize;

    //! This property holds the widget's maximum allowed size that is specified by the user.
    /*!
     * Note that maximum size overrides layout resize constraints.
     *
     * In order to set a maximum size use setMaximumSize() member method. If you wish to
     * disable maximum size for width or height use 0 for that value.
     */
    Size _maxSize;

    //! Pointer to application wide Designer instance.
    /*!
     * The designer is set and maintained by the main application. It is used for acquiring default
     * size hints and drawing widgets according to a particular style.
     *
     * @sa Application::setDesigner()
     */
    static Designer* _designer;

    /*!
     * This method attaches widget to given UIManager.
     *
     * @param uiManager to be attached to.
     */
    void
    setUIManager(UIManager* uiManager);

    /*!
     * Deletes surface and unsets InitialiseSurface flag.
     *
     * Should be used when parent window surface is released.
     */
    void
    invalidateSurface();
  };
}

#endif /* ILIXI_WIDGET_H_ */
