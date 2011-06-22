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

#ifndef HOME_H_
#define HOME_H_

#include "ui/Application.h"
#include "ui/ToolButton.h"
#include <vector>
#include "lib/TweenAnimation.h"

using namespace ilixi;

class AppButton : public ToolButton
{
public:
  AppButton(std::string text, Widget* parent = 0);

  ~AppButton();

protected:
  virtual void
  compose(const Rectangle& rect);

  /*!
   * Starts/stops tweens.
   */
  virtual void
  focusInEvent();

  /*!
   * Starts/stops tweens.
   */
  virtual void
  focusOutEvent();

private:
  float _ch;
  float _iconT;
  TweenAnimation* _focusIn;
  TweenAnimation* _focusOut;
};

class Home : public Application
{
public:
  Home(int argc, char* argv[]);

  virtual
  ~Home();

private:
  struct AppStruct
  {
    std::string title;
    std::string iconPath;
    std::string basename;
  };

  std::vector<AppStruct> _apps;

  void
  spawnApp(int index);

  void
  parseApps();
};

#endif /* HOME_H_ */
