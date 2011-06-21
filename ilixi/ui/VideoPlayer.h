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

#ifndef ILIXI_VIDEOPLAYER_H_
#define ILIXI_VIDEOPLAYER_H_

#include "ui/BorderBase.h"
#include "types/Video.h"

namespace ilixi
{

  class HBoxLayout;
  class ToolButton;
  class Label;
  class Slider;
  class VideoWidget;

  //! Simple video player widget.
  class VideoPlayer : public BorderBase
  {
  public:
    /*!
     * Constructor.
     */
    VideoPlayer(Widget* parent = 0);

    /*!
     * Destructor.
     */
    virtual
    ~VideoPlayer();

    /*!
     * Returns preferred size.
     */
    virtual Size
    preferredSize() const;

    /*!
     * Loads video for playback.
     *
     * @param path to video file.
     */
    void
    load(const std::string& path);

  private:
    //! Subsurface for blitting video.
    Surface* _videoSurface;
    //! Video object.
    Video* _video;

    //! Rewinds video.
    ToolButton* _rewind;
    //! Starts/stops playback.
    ToolButton* _play;
    //! Switches to fullscreen.
    ToolButton* _fullscreen;
    //! Playback time.
    Label* _time;

    //! Controls video position.
    Slider* _position;
    //! Controls video volume.
    Slider* _volume;

    HBoxLayout* _line1;
    HBoxLayout* _line2;

    Rectangle _videoRect;

    //! Draws video player.
    virtual void
    compose(const Rectangle& rect);

    //! Updates widget geometry.
    void
    updateVPGeometry();

    //! Blits frame to video surface.
    void
    updateVideo(IDirectFBSurface* frame);

    //! Starts video playback.
    void
    playVideo();

    //! Seeks video to start.
    void
    rewindVideo();

    //! Seeks video to given position.
    void
    seekVideo(int position);

    //! Sets video volume.
    void
    setVolume(int volume);

    //! Converts seconds to HMS format.
    void
    toHMS(double secs, char* buffer);
  };

}

#endif /* ILIXI_VIDEOPLAYER_H_ */
