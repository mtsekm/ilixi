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

#ifndef ILIXI_APPLICATION_H_
#define ILIXI_APPLICATION_H_

#include "core/AppBase.h"
#include "core/Window.h"
#include "ui/Frame.h"

namespace ilixi
{
  //! Main application.
  /*!
   * This class is used to create a new UI application.
   */
  // Window is inherited with virtual keyword in order to specify a destruction hierarchy.
  class Application : public AppBase, virtual public Window, public Frame
  {
  public:
    /*!
     * Constructor.
     *
     * @param argc
     * @param argv
     * @return
     */
    Application(int argc, char* argv[]);

    virtual
    ~Application();

    /*!
     * Invokes container's compose method.
     *
     * @sa Widget::drawBorder()
     */
    virtual void
    paint(const Rectangle& rect, bool forceRepaint = false);

    //! This method is called when application window becomes hidden.
    virtual void
    onHide();

    //! This method is called when application window becomes visible.
    virtual void
    onShow();

    //! Terminates application.
    void
    quit();

    /*!
     * Final!
     */
    void
    exec();

    /*!
     * This method executes update().
     */
    virtual void
    doLayout();

  protected:

    //! Shows application window.
    void
    show(Window::TransitionStyle style = None, int value1 = 0, int value2 = 0);

    //! Hides application window.
    void
    hide(Window::TransitionStyle style = None, int value1 = 0, int value2 = 0);

    /*!
     * Sets the designer and stylesheet
     *
     * @param designer
     * @param styleSheet
     */
    static void
        setDesigner(const char* designer, const char* styleSheet =
            "stylesheet.xml");

    //! Sets whether application's window is fullscreen.
    void
    setFullScreen(bool fullscreen);

    void
    setBackgroundImage(std::string imagePath);

  private:

    //! This flag specifies if application window should occupy whole screen.
    bool _fullscreen;
    //! This is the background image for application.
    Image* _backgroundImage;
    //! Title of this application.
    std::string _appTitle;

    //! Paints background using current designer.
    virtual void
    compose(const Rectangle& rect);

    /*!
     * Process upcoming event from the DFB windowing system.
     *
     * @param event
     */
    bool
    handleWindowEvent(const DFBWindowEvent& event);

    /*!
     * This callback is called when a new message arrives on application's reactor channel.
     * <b> Do not reimplement this method unless you know what you are doing. </b>
     *
     * @param msg
     * @param ctx
     * @return
     */
    virtual ReactionResult
    reactorCB(ReactorMessage *msg, void *ctx);
  };
}
#endif /* ILIXI_APPLICATION_H_ */
