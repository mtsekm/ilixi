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

#ifndef ILIXI_WINDOW_H_
#define ILIXI_WINDOW_H_

#include <directfb.h>
#include "core/Thread.h"
#include "core/UIManager.h"

namespace ilixi
{
  //! Creates DirectFB window.
  /*!
   * This class is used to create and maintain a DirectFB window.
   */
  class Window
  {
    friend class Application;

  public:

    enum TransitionStyle
    {
      None, Fade, Slide, FadeAndSlide
    };

    /*!
     * Constructor.
     */
    Window();

    /*!
     * Destructor.
     */
    virtual
    ~Window();

    /*!
     * Returns DirectFB interface.
     */
    static IDirectFB*
    DFBInterface();

    /*!
     * Returns DirectFB window interface.
     */
    IDirectFBSurface*
    DFBSurface() const;

    void
    flipSurface(const DFBRegion& region);

    /*!
     * Returns and interface to the focus manager.
     */
    UIManager* const
    windowFocusManager() const;

    /*!
     * Returns pixel format of window surfaces.
     */
    static DFBSurfacePixelFormat
    pixelFormat();

    static bool
    doubleBuffered();

    /*!
     * Makes the window visible and starts input event thread.
     * If style is Fade, value1 is target alpha value [0-255].
     * If style is Slide, value1 is target Y coordinate.
     * If style is FadeAndSlide, value1, value2 are target alpha and target Y coordinates respectively
     *
     * @param style default is None.
     * @param value1
     * @param value2
     */
    void
    showWindow(TransitionStyle style = Fade, int value1 = 0, int value2 = 0);

    /*!
     * Hides the window and terminates input event thread.
     */
    void
    hideWindow(TransitionStyle style = Fade, int value1 = 0, int value2 = 0);

    void
    closeWindow();

    void
    moveWindowTo(int x, int y);

    void
    resizeWindow(int width, int height);

    void
    setBounds(int x, int y, int width, int height);

    void
    setRotation(int rotation);

  protected:
    DFBWindowDescription _windowDesc;

    //! Interface to DirectFB window.
    IDirectFBWindow* _window;

    //! Pointer to window's DirectFB surface.
    IDirectFBSurface* _windowSurface;

    //! Pointer to FocusManager.
    UIManager* _windowUIManager;

    //! DirectFB interface is initialised by first window.
    static IDirectFB* _dfb;

    //! Interface to DirectFB layer.
    static IDirectFBDisplayLayer* _layer;

    //! Event buffer for window.
    static IDirectFBEventBuffer* _buffer;

    //!
    static DFBDisplayLayerConfig _layerConfig;

    //! Pixel format used for window surface
    static const DFBSurfacePixelFormat _pixelFormat;

    static const bool _doubleBuffered;

    /*!
     * Initialise DirectFB using command line parameters. This method is executed
     * only once by main Application during its construction.
     *
     * @param argc
     * @param argv
     */
    static void
    initDFB(int argc, char **argv);

    /*!
     * Create and initialise DirectFB window interfaces.
     */
    void
    initDFBWindow(int x = 0, int y = -1, int w = 0, int h = 0, bool dialog =
        false);

    /*!
     * Release DirectFB window interfaces.
     */
    void
    releaseWindow();

    /*!
     * Behaviour is dependant on incoming event type. Key events are sent to focused widget.
     * Pointer events are consumed by the widget which contains the event coordinates. If two or more
     * widgets contain the event coordinates, event is sent to the widget at the top.
     *
     * @param event parsed by DirectFB interfaces.
     * @return True if event is consumed.
     */
    virtual bool
    handleWindowEvent(const DFBWindowEvent& event);

    void
    cfc();

  private:
    //! This property holds the number of existing windows inside a main application, e.g. dialogs.
    static int _windowCount;

    Window* _parentWindow;

    DFBWindowID _id;

    static Window* _activeWindow;

    static pthread_mutex_t _windowMutex;

    /*!
     * Release DirectFB interface if it exists.
     */
    static void
    releaseDFB();
  };

}

#endif /* ILIXI_WINDOW_H_ */
