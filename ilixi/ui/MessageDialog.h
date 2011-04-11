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

#ifndef ILIXI_MESSAGEDIALOG_H_
#define ILIXI_MESSAGEDIALOG_H_

#include "ui/Dialog.h"

namespace ilixi
{
  class MessageDialog : public Dialog
  {
  public:

    enum DialogType
    {
      Plain, //! Only message is shown.
      Information, //!< Information icon.
      Question, //!< Question icon.
      Warning, //!< Warning icon.
      Critical, //!< Critical icon.
      Custom
    //!< Custom icon.
    };

    /*!
     * Constructor.
     *
     * @param title Dialog's title.
     * @param message Dialog's message.
     * @param option Designates buttons on the dialog.
     * @param type Type of dialog.
     * @param parent Pointer to dialog's parent.
     */
    MessageDialog(std::string title, std::string message, ButtonOption option =
        OKButtonOption, DialogType type = Information, Widget* parent = 0);

    /*!
     * Destructor.
     */
    virtual
    ~MessageDialog();

    /*!
     * Returns the type of the dialog.
     */
    DialogType
    dialogType() const;

  private:
    //! This property stores type of the dialog.
    DialogType _dialogType;
  };
}

#endif /* ILIXI_MESSAGEDIALOG_H_ */
