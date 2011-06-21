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

#ifndef ILIXI_TWEENANIMATION_H_
#define ILIXI_TWEENANIMATION_H_

#include "lib/Animation.h"
#include "lib/Tween.h"
#include "lib/Easing.h"
#include <list>

namespace ilixi
{
  class TweenAnimation : public Animation
  {
  public:
    TweenAnimation(Widget* target = 0);

    virtual
    ~TweenAnimation();

    void
    addTween(Tween* tween);

    void
    removeTween(Tween* tween);

  protected:
    virtual void
    step(long ms);

  private:
    static Easing* _eTable[11];

    typedef std::list<Tween*> TweenList;
    typedef std::list<Tween*>::iterator TweenIterator;
    TweenList _tweens;

    float
    runEquation(Tween::Transition transition, Tween::Equation equation,
        float t, float b, float c, float d);

  };

}

#endif /* ILIXI_TWEENANIMATION_H_ */
