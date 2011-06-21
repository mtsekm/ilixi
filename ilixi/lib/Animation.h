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

#ifndef ILIXI_ANIMATION_H_
#define ILIXI_ANIMATION_H_

#include "core/Thread.h"

namespace ilixi
{

  class Widget;
  class Animation;

  class AnimationThread : public Thread
  {
  public:
    AnimationThread(Animation* animation);

    ~AnimationThread();

    int
    run();

  private:
    Animation* _animation;
  };

  //! Base class for animations.
  /*!
   * This class implements basic animation functionality.
   */
  class Animation
  {
  public:

    friend class AnimationThread;

    enum AnimationState
    {
      Running, Paused, Stopped
    };

    /*!
     * Constructor.
     *
     * Initialises a new animation.
     *
     * @param target widget to repaint at each step.
     */
    Animation(Widget* target = 0);

    /*!
     * Destructor.
     */
    virtual
    ~Animation();

    /*!
     * Returns current state of animation.
     */
    AnimationState
    state() const;

    /*!
     * Returns duration of animation in milliseconds.
     */
    int
    duration() const;

    /*!
     * Returns current animation time in milliseconds.
     */
    int
    currentTime() const;

    /*!
     * Returns number of loops.
     */
    int
    loops() const;

    /*!
     * Returns current loop number.
     */
    int
    currentLoop() const;

    /*!
     * Starts animation.
     */
    void
    start();

    /*!
     * Stops animation.
     */
    void
    stop();

    /*!
     * Resumes animation.
     */
    void
    resume();

    /*!
     * Pauses animation.
     */
    void
    pause();

    /*!
     * Sets duration of animation.
     *
     * @param ms duration in milliseconds.
     */
    void
    setDuration(int ms);

    /*!
     * Sets number of loops.
     *
     * @param loopCount
     */
    void
    setLoops(int loopCount);

    /*!
     * This signal is emitted when animation is finished playing.
     */
    sigc::signal<void> sigFinished;

    /*!
     * This signal is emitted when animation finishes a loop.
     */
    sigc::signal<int> sigLoopChanged;

  protected:
    //! Sets current time of animation.
    void
    setCurrentTime(int ms = 0);

    //! Sets animation state.
    virtual void
    setState(AnimationState state);

    //! Iterates animation.
    bool
    animate();

    //! Reimplement this method...
    virtual void
    step(long ms)=0;

  private:
    //! Target widget to repaint at each step of animation.
    Widget* _targetWidget;

    //! Current state of animation.
    AnimationState _state;

    //! Duration of animation in milliseconds.
    int _duration;
    //! Current animation time in milliseconds.
    int _currentTime;
    //! Last measured time in milliseconds.
    long _lastTime;

    //! Loop count of animation.
    int _loops;
    //! Current loop in animation.
    int _currentLoop;

    AnimationThread* _thread;

    //AnimationGroup* _group;
  };

}

#endif /* ILIXI_ANIMATION_H_ */
