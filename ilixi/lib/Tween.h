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

#ifndef ILIXI_TWEEN_H_
#define ILIXI_TWEEN_H_

namespace ilixi
{

  class Tween
  {
    friend class TweenAnimation;

  public:

    enum Transition
    {
      LINEAR,
      SINE,
      QUINT,
      QUART,
      QUAD,
      EXPO,
      ELASTIC,
      CUBIC,
      CIRC,
      BOUNCE,
      BACK
    };

    enum Equation
    {
      EASE_IN, EASE_OUT, EASE_IN_OUT
    };

    Tween(Transition transition, Equation equation, float& variable,
        float endValue);

    Tween(Transition transition, Equation equation, float& variable,
        float initialValue, float endValue);

    virtual
    ~Tween();

  private:
    Transition _transition;
    Equation _equation;
    float* _variable;
    float _initialValue;
    float _endValue;
  };

}

/*
 * AnimationGroup* g = new AnimationGroup();
 *      ...
 * TweenAnimation *t = new TweenAnimation(Widget* target); // target is repainted. if null no repaint.
 * t->addTween(new Tween(LINEAR, EaseIn, &x, 100);
 * t->addTween(new Tween(QUAD, EaseOut, &y, 200);
 * t->setDuration(100); // Duration in milisecons.
 *
 * g->addAnimation(t);
 *      ...
 * g->start(); // Starts all animations...
 *
 */

#endif /* ILIXI_TWEEN_H_ */
