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

#ifndef ILIXI_SPINBOX_H_
#define ILIXI_SPINBOX_H_

#include "core/Thread.h"
#include "ui/LineEdit.h"

namespace ilixi
{
  class DirectionalButton;
  class SpinBox;

  class SpinBoxThread : public Thread
  {
  public:
    SpinBoxThread(SpinBox* parent);

    ~SpinBoxThread();

    int
    run();

    void
    setSpinAmount(int amount);

  private:
    SpinBox* _parent;
    int _amount;
  };

  class SpinBox : public Widget
  {
    friend class SpinBoxThread;
  public:
    SpinBox(Widget* parent = 0);

    virtual
    ~SpinBox();

    virtual Size
    preferredSize() const;

    bool
    accelerated() const;

    int
    max() const;

    int
    min() const;

    std::string
    postfix() const;

    std::string
    prefix() const;

    std::string
    special() const;

    int
    value() const;

    bool
    wrapping() const;

    void
    setAccelerate(bool accelerate);

    void
    setMax(int max);

    void
    setMin(int min);

    void
    setRange(int min, int max);

    void
    setPostfix(std::string postfix);

    void
    setPrefix(std::string prefix);

    void
    setSpecial(std::string special);

    void
    setValue(int value);

    void
    setWrap(bool wrap);

    /*!
     * This signal is emitted when the value of SpinBox is changed.
     */
    sigc::signal<void, int> sigValueChanged;

  private:
    int _min;
    int _max;
    int _value;
    int _step;bool _accelerate;bool _wrap;

    std::string _prefix;
    std::string _postfix;
    std::string _special;

    SpinBoxThread _thread;
    DirectionalButton* button1;
    DirectionalButton* button2;
    LineEdit* _lineEdit;

    void
    compose(const Rectangle& rect);

    void
    startSpin(bool increment);

    void
    stopSpin();

    void
    disableButtons(int value);

    void
    updateSpinBoxGeometry();
  };

}

#endif /* ILIXI_SPINBOX_H_ */
