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

#ifndef STATUSBAR_H_
#define STATUSBAR_H_

#include "core/Thread.h"
#include "ui/Application.h"
#include "ui/Label.h"
#include "ui/ToolButton.h"
#include "SDialog.h"

using namespace ilixi;

class StatusBar;

class DateThread : public Thread
{
public:
  DateThread(StatusBar* parent);

  ~DateThread();

  int
  run();

private:
  StatusBar* _parent;
};

class StatusBar : public Application
{
public:
  StatusBar(int argc, char* argv[]);

  virtual
  ~StatusBar();

  virtual void
  onHide();

  virtual void
  onShow();

  void
  updateTime();

protected:

  virtual ReactionResult
  reactorCB(ReactorMessage *msg, void *ctx);

private:
  unsigned int _currentAppID;
  Label* _app;
  Label* _time;
  Label* _date;
  ToolButton* _home;
  ToolButton* _shutDown;
  SDialog* _sDialog;

  DateThread* dateThread;

  void
  compose(const Rectangle& rect);

  void
  handleDialog(int result);

  void
  showHome();
};

#endif /* STATUSBAR_H_ */
