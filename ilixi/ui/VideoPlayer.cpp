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

#include "ui/VideoPlayer.h"
#include "graphics/Painter.h"
#include "ui/HBoxLayout.h"
#include "ui/Slider.h"
#include "ui/ToolButton.h"
#include "ui/Label.h"
#include "core/Logger.h"
#include <sstream>
#include <iomanip>

using namespace ilixi;

VideoPlayer::VideoPlayer(Widget* parent) :
  BorderBase(parent), _videoSurface(NULL), _video(NULL)
{
  setBorderStyle(StyledBorder);
  setInputMethod(PointerInputOnly);
  setConstraints(MinimumExpandingConstraint, MinimumConstraint);

  _line1 = new HBoxLayout();
  addChild(_line1);

  _line2 = new HBoxLayout();
  addChild(_line2);

  _rewind = new ToolButton("Rewind");
  _rewind->setIcon(ILIXI_DATADIR"rewind.png", Size(16, 16));
  _rewind->setToolButtonStyle(ToolButton::IconOnly);
  _rewind->setDisabled();
  _rewind->sigClicked.connect(sigc::mem_fun(this, &VideoPlayer::rewindVideo));
  _line1->addWidget(_rewind);

  _play = new ToolButton("Play");
  _play->setIcon(ILIXI_DATADIR"play.png", Size(16, 16));
  _play->setToolButtonStyle(ToolButton::IconOnly);
  _play->setDisabled();
  _play->sigClicked.connect(sigc::mem_fun(this, &VideoPlayer::playVideo));
  _line1->addWidget(_play);

  _position = new Slider();
  _position->setDisabled();
  _line2->addWidget(_position);
  _position->sigValueChanged.connect(
      sigc::mem_fun(this, &VideoPlayer::seekVideo));

  _time = new Label("00:00:00");
  _line2->addWidget(_time);

  _fullscreen = new ToolButton("FullScreen");
  _fullscreen->setIcon(ILIXI_DATADIR"fullscreen.png", Size(16, 16));
  _fullscreen->setToolButtonStyle(ToolButton::IconOnly);
  _fullscreen->setDisabled();
  //  _fullscreen->sigClicked.connect(sigc::mem_fun(this, &VideoPlayer::fullscreen));
  _line1->addWidget(_fullscreen);

  _line1->addWidget(new Spacer(Horizontal));

  _volume = new Slider();
  _volume->setValue(100);
  _volume->setDisabled();
  _volume->setHConstraint(MinimumConstraint);
  _volume->sigValueChanged.connect(sigc::mem_fun(this, &VideoPlayer::setVolume));
  _line1->addWidget(_volume);

  setConstraints(MinimumConstraint, MinimumConstraint);
  sigGeometryUpdated.connect(
      sigc::mem_fun(this, &VideoPlayer::updateVPGeometry));
}

VideoPlayer::~VideoPlayer()
{
  delete _video;
  delete _videoSurface;
}

Size
VideoPlayer::preferredSize() const
{
  Size s1 = _line1->preferredSize();
  Size s2 = _line2->preferredSize();
  int w = 0, h = 0;
  if (s2.width() < s1.width())
    w = s1.width();
  else
    w = s2.width();
  w += 2 * borderWidth();
  h = s1.height() + s2.height() + 15;

  return Size(w, h);
}

void
VideoPlayer::load(const std::string& path)
{
  delete _video;
  _video = new Video(path);
  _video->frameUpdated.connect(sigc::mem_fun(this, &VideoPlayer::updateVideo));
  _play->setEnabled();
}

void
VideoPlayer::compose(const Rectangle& rect)
{
  Painter painter(this);
  painter.begin(rect);
  designer()->drawFrame(&painter, this, 0, 0, width(), height(), true,
      BottomCorners);
  painter.end();
}

void
VideoPlayer::updateVPGeometry()
{
  int borderR = borderHorizontalOffset();
  Size s1 = _line1->preferredSize();
  _line1->setGeometry(borderR, height() - s1.height() - borderR,
      width() - 2 * borderR, s1.height());

  Size s2 = _line2->preferredSize();
  _line2->setGeometry(borderR, _line1->y() - s2.height() - 5, _line1->width(),
      s2.height());

  if (surface() && !_videoSurface)
    {
      _videoSurface = new Surface();
      _videoSurface->createDFBSubSurface(_surfaceGeometry,
          surface()->DFBSurface());
    }
  if (_videoSurface)
    {
      _videoRect = Rectangle(borderWidth(), borderWidth(),
          width() - 2 * borderWidth(),
          height() - s1.height() - s2.height() - 15);
      _videoSurface->setGeometry(borderWidth(), borderWidth(),
          width() - 2 * borderWidth(),
          height() - s1.height() - s2.height() - 15);
    }
}

void
VideoPlayer::updateVideo(IDirectFBSurface* frame)
{
  _videoSurface->DFBSurface()->StretchBlit(_videoSurface->DFBSurface(), frame,
      NULL, NULL);
  _videoSurface->flip();

  if (!_position->pressed())
    {
      char buffer[9];
      toHMS(_video->position(), buffer);
      _time->setText(buffer);
      _position->setValue(100 * (_video->position() / _video->length()), false);
    }

  if (_video->status() == DVSTATE_FINISHED)
    {
      _play->setIcon(ILIXI_DATADIR"play.png", Size(16, 16));
      _play->update();
    }
}

void
VideoPlayer::playVideo()
{
  switch (_video->status())
    {
  case DVSTATE_STOP:
    _video->play();
    _play->setIcon(ILIXI_DATADIR"pause.png", Size(16, 16));
    //    _play->update();
    _rewind->setEnabled();
    _fullscreen->setEnabled();
    _position->setEnabled();
    _volume->setEnabled();
    break;
  case DVSTATE_FINISHED:
    _video->seek(0);
    _video->play();
    _play->setIcon(ILIXI_DATADIR"pause.png", Size(16, 16));
    //    _play->update();
    _rewind->setEnabled();
    _fullscreen->setEnabled();
    _position->setEnabled();
    _volume->setEnabled();
    break;

  case DVSTATE_BUFFERING:
  case DVSTATE_PLAY:
    _video->stop();
    _play->setIcon(ILIXI_DATADIR"play.png", Size(16, 16));
    //    _play->update();
    break;

  default:
    _play->setDisabled();
    }
}

void
VideoPlayer::rewindVideo()
{
  _video->seek(0);
}

void
VideoPlayer::seekVideo(int position)
{
  double secs = (position * _video->length()) / 100;
  _video->seek(secs);
  char buffer[9];
  toHMS(secs, buffer);
  _time->setText(buffer);
}

void
VideoPlayer::setVolume(int volume)
{
  _video->setVolume(volume / 100.0);
}

void
VideoPlayer::toHMS(double secs, char* buffer)
{
  int hour, min, sec;
  hour = secs / 3600;
  secs = (int) secs % 3600;
  min = secs / 60;
  secs = (int) secs % 60;
  sec = secs;
  sprintf(buffer, "%02d:%02d:%02d", hour, min, sec);
}
