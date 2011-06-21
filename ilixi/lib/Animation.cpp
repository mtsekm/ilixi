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

#include "lib/Animation.h"
#include "ui/Widget.h"
#include <sys/time.h>

using namespace ilixi;

//*********************************************************************
// AnimationThread
//*********************************************************************
AnimationThread::AnimationThread(Animation* animation) :
  Thread(), _animation(animation)
{
}

AnimationThread::~AnimationThread()
{
}

int
AnimationThread::run()
{
  pthread_testcancel();
  while (_animation->state() == Animation::Running)
    {
      if (!_animation->animate())
        break;
    }
  _animation->sigFinished();
  return 1;
}

//*********************************************************************
// Animation
//*********************************************************************
Animation::Animation(Widget* target) :
  _targetWidget(target), _state(Stopped), _duration(0), _currentTime(0),
      _lastTime(0), _loops(1), _currentLoop(0)
{
  _thread = new AnimationThread(this);
}

Animation::~Animation()
{
  delete _thread;
}

Animation::AnimationState
Animation::state() const
{
  return _state;
}

int
Animation::duration() const
{
  return _duration;
}

int
Animation::currentTime() const
{
  return _currentTime;
}

int
Animation::loops() const
{
  return _loops;
}

int
Animation::currentLoop() const
{
  return _currentLoop;
}

void
Animation::start()
{
  setCurrentTime();
  setState(Running);
  _thread->start();
}

void
Animation::stop()
{
  setState(Stopped);
  _thread->cancel();
}

void
Animation::resume()
{
  setCurrentTime();
  setState(Running);
}

void
Animation::pause()
{
  setState(Paused);
}

void
Animation::setDuration(int ms)
{
  _duration = ms;
}

void
Animation::setLoops(int loopCount)
{
  _loops = loopCount;
}

void
Animation::setCurrentTime(int ms)
{
  if (_duration && ms)
    {
      _currentTime = ms;
      struct timeval tv;
      gettimeofday(&tv, NULL);
      _lastTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
    }
  else
    {
      _currentTime = 0;
      struct timeval tv;
      gettimeofday(&tv, NULL);
      _lastTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
    }
}

void
Animation::setState(AnimationState state)
{
  _state = state;
}

bool
Animation::animate()
{
  if (!_duration)
    {
      setState(Stopped);
      return 0;
    }

  struct timeval tv;
  long curTime, stepTime;
  gettimeofday(&tv, NULL);
  curTime = (tv.tv_sec * 1000 + tv.tv_usec / 1000.0) + 0.5;
  stepTime = curTime - _lastTime;

  if (_currentTime < _duration)
    {
      step(stepTime);
      setCurrentTime(_currentTime + stepTime);

      if (_targetWidget)
        _targetWidget->repaint();

      return 1;
    }
  stop();
  return 0;
}
