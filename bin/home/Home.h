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

#ifndef HOME_H_
#define HOME_H_

#include "ui/Application.h"
#include "ui/ToolButton.h"
#include <vector>

using namespace ilixi;

class AppButton : public ToolButton
{
public:
  AppButton(std::string text, Widget* parent = 0);

  ~AppButton();

protected:
  virtual void
  compose(const Rectangle& rect);
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
    std::string path;
  };

  std::vector<AppStruct> _apps;

  void
  spawnApp(int index);

  void
  parseApps();
};

#endif /* HOME_H_ */
