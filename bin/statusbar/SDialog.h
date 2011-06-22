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

#ifndef SDIALOG_H_
#define SDIALOG_H_

#include "ui/Dialog.h"
#include "ui/PushButton.h"

using namespace ilixi;

class SDialog : public Dialog
{
public:
  SDialog(std::string title, Widget* parent);

  virtual
  ~SDialog();

  void
  setAppName(std::string appName);

  void
  setKillVisibility(bool visible);

private:
  PushButton* _kill;
  PushButton* _lock;
  PushButton* _restart;
  PushButton* _shutdown;
};

#endif /* SDIALOG_H_ */
