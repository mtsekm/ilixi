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

#include "lib/TweenAnimation.h"
#include <algorithm>
#include <stdio.h>

using namespace ilixi;

static Linear fLinear;
static Sine fSine;
static Quint fQuint;
static Quart fQuart;
static Quad fQuad;
static Expo fExpo;
static Elastic fElastic;
static Cubic fCubic;
static Circ fCirc;
static Bounce fBounce;
static Back fBack;

Easing* TweenAnimation::_eTable[11] =
  { &fLinear, &fSine, &fQuint, &fQuart, &fQuad, &fExpo, &fElastic, &fCubic,
      &fCirc, &fBounce, &fBack };

TweenAnimation::TweenAnimation(Widget* target) :
  Animation(target)
{
}

TweenAnimation::~TweenAnimation()
{
  for (TweenIterator it = _tweens.begin(); it != _tweens.end(); ++it)
    delete *it;
}

void
TweenAnimation::addTween(Tween* tween)
{
  _tweens.push_back(tween);
}

void
TweenAnimation::removeTween(Tween* tween)
{
  if (!tween)
    return;
  TweenIterator it = std::find(_tweens.begin(), _tweens.end(), tween);
  if (tween == *it)
    {
      delete *it;
      _tweens.erase(it);
    }
}

void
TweenAnimation::step(long ms)
{
  int t = currentTime();
  int d = duration();
  for (TweenIterator it = _tweens.begin(); it != _tweens.end(); ++it)
    {
      Tween* o = (Tween*) *it;
      *(o->_variable) = runEquation(o->_transition, o->_equation, t,
          o->_initialValue, (o->_endValue - o->_initialValue), d);
    }
}

float
TweenAnimation::runEquation(Tween::Transition transition,
    Tween::Equation equation, float t, float b, float c, float d)
{
  switch (equation)
    {
  case Tween::EASE_IN:
    return _eTable[transition]->easeIn(t, b, c, d);
  case Tween::EASE_OUT:
    return _eTable[transition]->easeOut(t, b, c, d);
  case Tween::EASE_IN_OUT:
    return _eTable[transition]->easeInOut(t, b, c, d);
    }
}
