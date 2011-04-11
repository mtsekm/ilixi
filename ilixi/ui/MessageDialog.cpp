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

#include "ui/MessageDialog.h"
#include "ui/HBoxLayout.h"
#include "ui/Label.h"
#include "ui/Icon.h"

using namespace ilixi;

MessageDialog::MessageDialog(std::string title, std::string message,
    ButtonOption option, DialogType type, Widget* parent) :
  Dialog(title, option, parent), _dialogType(type)
{
  setLayout(new HBoxLayout());
  if (_dialogType != Plain)
    {
      Icon* icon;
      switch (_dialogType)
        {
      case Information:
        icon = new Icon(designer()->defaultIcon(InformationMessage));
        break;
      case Question:
        icon = new Icon(designer()->defaultIcon(QuestionMessage));
        break;
      case Warning:
        icon = new Icon(designer()->defaultIcon(WarningMessage));
        break;
      case Critical:
        icon = new Icon(designer()->defaultIcon(CriticalMessage));
        break;
        }
      addWidget(icon);
    }
  Label* msg = new Label(message);
  addWidget(msg);
}

MessageDialog::~MessageDialog()
{
}

MessageDialog::DialogType
MessageDialog::dialogType() const
{
  return _dialogType;
}
