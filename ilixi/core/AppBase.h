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

#ifndef ILIXI_APPBASE_H_
#define ILIXI_APPBASE_H_

#include <string>
#include <pthread.h>
#include "core/IFusion.h"

namespace ilixi
{
  //! Base class for ilixi applications.
  /*!
   * Each ilixi application is derived from this base class except Maestro.
   * AppBase manages a connection to the ilixi dfb-fusion world using
   * joinFusion() and leaveFusion() methods. If your intention is to create a simple
   * ilixi background process without access to event buffer (e.g. input events) and
   * any visual elements, you could derive from this class.
   *
   * AppBase initiates a connection attempt to ilixi dfb-fusion world during construction;
   * the process is terminated if the attempt was unsuccessful. If successful a new
   * application record is created inside shared memory pool and initialised using joinFusionCB().
   */
  class AppBase
  {
    friend class UIManager;
    friend class Window;
  public:
    /*!
     * Constructor.
     */
    AppBase(int argc, char* argv[]);

    /*!
     * Destructor.
     */
    virtual
    ~AppBase();

    AppMode
    appMode() const;

    std::string
    title() const;

    void
    setTitle(std::string title);

    void
    setAppMode(AppMode mode);

  protected:
    //    std::string processName;

    char*
    getOSKText() const;

    void
    setOSKText(const std::string& text);

    int
    getStatusBarHeight() const;

    /*!
     * This callback method is called as the applications joins ilixiArena.
     * Memory for storing an application record is allocated inside the shared pool
     * and the record is initialised.
     *
     * @param arena Pointer to ilixiArena.
     * @param ctx
     */
    int
    enterArenaCB(FusionArena* arena, void* ctx);

    /*!
     * This callback method is called as the application leaves ilixiArena.
     * Previously allocated memory in shared pool is freed and the application
     * record is removed.
     *
     * @param arena Pointer to ilixiArena.
     * @param ctx
     * @param emergency
     */
    int
    cleanArenaCB(FusionArena* arena, void* ctx, bool emergency);

    /*!
     * This callback method is called if application receives a message either on
     * global or direct channel from Maestro.
     *
     * @param msg a message struct.
     * @param ctx
     */
    virtual ReactionResult
    reactorCB(ReactorMessage *msg, void *ctx);

    AppRecord* const
    getAppRecord(unsigned int appID) const;

    AppRecord* const
    getAppRecord(std::string process) const;

    //    Designer*
    //    getDesigner() const;
    //
    //    IDirectFB*
    //    MDFBInterface() const;

    /*!
     * This method is used to send a mode request or other notify messages from
     * the application to Maestro.
     *
     * @param type of message to send to maestro.
     * @param mode
     */
    void
    callMaestro(ReactorMessageType type, AppMode mode = Terminated,
        unsigned int appID = 0);

  private:
    //! Pointer to ilixi dfb-fusion world
    FusionWorld *world;
    //! Pointer to ilixi dfb-fusion arena
    FusionArena *arena;
    //! Fusion call object
    FusionCall call;
    //! Fusion reaction
    Reaction reaction;
    //! Pointer to MaestroObject instance inside shared memory pool
    MaestroObject *maestroObject;
    //! Pointer to shared dfb-FusionVector instance inside shared memory pool
    FusionVector *appVector;
    //! Pointer to AppRecord instance of this application inside shared memory pool
    AppRecord *appRecord;

    static AppBase* appInstance;

    static int
    joinFusionCBW(FusionArena *arena, void *ctx);

    static int
    cleanFusionCBW(FusionArena *arena, void *ctx, bool emergency);

    static ReactionResult
    reactionCBW(const void *msgData, void *ctx);

    static FusionCallHandlerResult
    dispatchCBW(int caller, int call_arg, void *call_ptr, void *ctx,
        unsigned int serial, int *ret_val);

    /*!
     * This method is used to connect to the fusion world as a slave.
     * Upon successful connection an application record is formed inside
     * the shared pool. Furthermore, RPC mechanism which is used for
     * controlling the application's state is set up.
     *
     * \sa enterArenaCB
     *
     * @return true if operation is successful.
     */
    bool
    joinFusion();

    /*!
     * Leaves ilixi dfb-fusion world and arena.
     * \sa cleanArenaCB
     */
    void
    leaveFusion();

  };
}
#endif /* ILIXI_APPBASE_H_ */
