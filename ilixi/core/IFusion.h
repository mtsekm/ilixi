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

#ifndef ILIXI_FUSION_HPP_
#define ILIXI_FUSION_HPP_

// dfb-Fusion definitions
extern "C"
{
#include <linux/fusion.h>
#include <fusion/fusion.h>
#include <fusion/shmalloc.h>
#include <fusion/call.h>
#include <fusion/arena.h>
#include <fusion/vector.h>
#include <fusion/reactor.h>
}

// DirectFB
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <directfb.h>

namespace ilixi
{

#define DFBCHECK(x...) \
        {                                                                      \
           DFBResult err = x;                                                  \
           if (err != DFB_OK) {                                                \
              fprintf( stderr, "%s <%d>:\n\t", __FILE__, __LINE__ );           \
              DirectFBErrorFatal( #x, err );                                   \
           }                                                                   \
        }

  //! Maestro creates a unique instance of this object in ilixi fusion world.
  struct MaestroObject
  {
    //! Pointer to ilixi dfb-fusion shared memory pool
    FusionSHMPoolShared* pool;
    //! Pointer to dfb-fusion reactor
    FusionReactor* reactor;
    //! Pointer to OSK input.
    char* OSK;
    //! Height of statusbar application in pixels.
    int statusBarHeight;
    // TODO: Add clipboard.
  };

  /*!
   * This enum is used to specify the state of an application.
   */
  enum AppMode
  {
    None, //!< Not set.
    Initialising, //!< Application is in the process of accessing/modifying shared resources.
    Ready, //!< Application successfully initialised and ready to start.
    Visible, //!< Application window has access to events and is visible.
    Hidden, //!< Application window has no access to events and is hidden.
    Terminated, //!< Application should terminate shortly.
    Background
  //!< Application is a background process with no window and no access to events.
  };

  /*!
   * This enum is used to specify the type of message that is sent over reactor.
   */
  enum ReactorMessageType
  {
    SwitchMode, //!< Switch mode (Maestro -> Application).
    ModeRequest, //!< Request a mode change (Application -> Maestro).
    Notification, //!< The notification of a state change (Application -> Maestro).
    OSKEvent
  };

  //! An active application has a record in ilixi fusion world.
  struct AppRecord
  {
    //! Current mode of application.
    AppMode mode;
    //! FusionID of application in ilixi fusion world.
    unsigned int fusionID;
    //! pid of task.
    pid_t pid;
    //! Title of application.
    char* title;
    //! Path to application.
    char* process;
  };

  //! Maestro and applications communicate over reactor using this message format.
  struct ReactorMessage
  {
    /*!
     * Constructor.
     *
     * @param ID corresponding to fusionID of sender.
     * @param messageType type of message.
     * @param appMode makes sense only if type is SwitchMode or ModeRequest.
     * @param index of application inside appVector.
     */
    ReactorMessage(int fusionID, ReactorMessageType messageType,
        AppMode appMode, int index = -1) :
      senderFusionID(fusionID), type(messageType), mode(appMode), appID(index)
    {
    }

    //! FusionID of sender.
    unsigned int senderFusionID;
    //! Type of this message.
    ReactorMessageType type;
    //! If type is mode related, this field is not null.
    AppMode mode;
    //! This is the index of sender in appVector. If sender is Maestro it is -1.
    unsigned int appID;
  };
}
#endif /* ILIXI_FUSION_HPP_ */
