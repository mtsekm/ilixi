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

#include "ui/SpinBox.h"
#include "ui/DirectionalButton.h"
#include "graphics/Painter.h"
#include <sigc++/bind.h>
#include <sstream>

using namespace ilixi;

//*********************************************************************
// SpinBoxThread
//*********************************************************************
SpinBoxThread::SpinBoxThread(SpinBox* parent) :
  Thread(), _parent(parent), _amount(0)
{
}

SpinBoxThread::~SpinBoxThread()
{
}

int
SpinBoxThread::run()
{
  _parent->setValue(_parent->value() + _amount);
  int value = _parent->value();
  unsigned int sleepNS = 200000;
  usleep(sleepNS);
  pthread_testcancel();
  while (value > _parent->min() && value < _parent->max())
    {
      pthread_testcancel();
      _parent->setValue(value + _amount);
      value = _parent->value();
      if (sleepNS > 10000 && _parent->_accelerate)
        {
          sleepNS -= 10000;
          usleep(sleepNS);
        }
    }
  return 1;
}

void
SpinBoxThread::setSpinAmount(int amount)
{
  _amount = amount;
}

//*********************************************************************
// ScrollBarThread
//*********************************************************************
SpinBox::SpinBox(Widget* parent) :
  Widget(parent), _min(0), _max(100), _value(0), _step(1), _accelerate(true),
      _wrap(true), _thread(this)
{
  button1 = new DirectionalButton(DirectionalButton::PlusMinusButton, Left,
      this);
  button1->setDisabled();
  button1->sigPressed.connect(sigc::bind<bool>(sigc::mem_fun(this,
      &SpinBox::startSpin), false));
  button1->sigReleased.connect(sigc::mem_fun(this, &SpinBox::stopSpin));
  addChild(button1);

  button2 = new DirectionalButton(DirectionalButton::PlusMinusButton, Right,
      this);
  button2->sigPressed.connect(sigc::bind<bool>(sigc::mem_fun(this,
      &SpinBox::startSpin), true));
  button2->sigReleased.connect(sigc::mem_fun(this, &SpinBox::stopSpin));
  addChild(button2);

  std::stringstream ss;
  ss << _prefix << _value << _postfix;
  _lineEdit = new LineEdit(ss.str());
  _lineEdit->setReadOnly(true);
  _lineEdit->setInputMethod(PointerInputOnly);
  addChild(_lineEdit);

  sigValueChanged.connect(sigc::mem_fun(this, &SpinBox::disableButtons));
  setConstraints(MinimumConstraint, FixedConstraint);
  setInputMethod(OSKInputEnabled);
  sigGeometryUpdated.connect(sigc::mem_fun(this,
      &SpinBox::updateSpinBoxGeometry));
}

SpinBox::~SpinBox()
{
}

Size
SpinBox::preferredSize() const
{
  return Size(100, 30);
}

bool
SpinBox::accelerated() const
{
  return _accelerate;
}

int
SpinBox::max() const
{
  return _max;
}

int
SpinBox::min() const
{
  return _min;
}

std::string
SpinBox::postfix() const
{
  return _postfix;
}

std::string
SpinBox::prefix() const
{
  return _prefix;
}

std::string
SpinBox::special() const
{
  return _special;
}

int
SpinBox::value() const
{
  return _value;
}

bool
SpinBox::wrapping() const
{
  return _wrap;
}

void
SpinBox::setAccelerate(bool accelerate)
{
  _accelerate = accelerate;
}

void
SpinBox::setMax(int max)
{
  _max = max;
}

void
SpinBox::setMin(int min)
{
  _min = min;
}

void
SpinBox::setRange(int min, int max)
{
  _min = min;
  _max = max;
}

void
SpinBox::setPostfix(std::string postfix)
{
  _postfix = postfix;
  std::stringstream ss;
  ss << _prefix << _value << _postfix;
  _lineEdit->setText(ss.str());
}

void
SpinBox::setPrefix(std::string prefix)
{
  _prefix = prefix;
  std::stringstream ss;
  ss << _prefix << _value << _postfix;
  _lineEdit->setText(ss.str());
}

void
SpinBox::setSpecial(std::string special)
{
  _special = special;
}

void
SpinBox::setValue(int value)
{
  if (value != _value && value >= _min && value <= _max)
    {
      _value = value;
      std::stringstream ss;
      ss << _prefix << _value << _postfix;
      _lineEdit->setText(ss.str());
      sigValueChanged(_value);
      update();
    }
}

void
SpinBox::setWrap(bool wrap)
{
  _wrap = wrap;
}

void
SpinBox::compose(const Rectangle& rect)
{
}

void
SpinBox::startSpin(bool increment)
{
  if (increment)
    _thread.setSpinAmount(_step);
  else
    _thread.setSpinAmount(-_step);
  _thread.start();
}

void
SpinBox::stopSpin()
{
  _thread.cancel();
}

void
SpinBox::disableButtons(int value)
{
  if (value == _min)
    button1->setDisabled();
  else if (value == _max)
    button2->setDisabled();
  else if (value > _min && value < _max)
    {
      button1->setEnabled();
      button2->setEnabled();
    }
}

void
SpinBox::updateSpinBoxGeometry()
{
  _lineEdit->setSize(width() - 60, height());
  button1->setSize(30, height());
  button2->setSize(30, height());

  _lineEdit->moveTo(0, 0);
  button1->moveTo(_lineEdit->width(), 0);
  button2->moveTo(_lineEdit->width() + 30, 0);
}
