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

#include "AppBase.h"
#include "core/Logger.h"
#include <fstream> // Required for reading maestro.conf
using namespace ilixi;

// Global pointer to AppBase instance used in callback wrappers.
AppBase* AppBase::appInstance = NULL;

//*************************************************************
// Callback wrappers
//*************************************************************
int
AppBase::joinFusionCBW(FusionArena *arena, void *ctx)
{
  if (appInstance)
    return appInstance->enterArenaCB(arena, ctx);
  return 0;
}

int
AppBase::cleanFusionCBW(FusionArena *arena, void *ctx, bool emergency)
{
  if (appInstance)
    return appInstance->cleanArenaCB(arena, ctx, emergency);
  return 0;
}

ReactionResult
AppBase::reactionCBW(const void *msgData, void *ctx)
{
  ReactorMessage *msg = (ReactorMessage*) msgData;
  if (appInstance)
    return appInstance->reactorCB(msg, ctx);
  return RS_REMOVE;
}

FusionCallHandlerResult
AppBase::dispatchCBW(int caller, int call_arg, void *call_ptr, void *ctx,
    unsigned int serial, int *ret_val)
{
  if (call_arg == appInstance->appRecord->fusionID)
    {
      ILOG_DEBUG("Message dispatched by FusionID %lu is received by Maestro.", call_arg)
        ;
      return FCHR_RETURN;
    }
  return FCHR_RETAIN;
}

//*************************************************************

AppBase::AppBase(int argc, char* argv[])
{
//  if (argc >= 2)
//    {
//      for (int i = 1; i < argc; i++)
//        if (strcmp(argv[i], "--install") == 0)
//          printf("argc: %d argv: %s", argc, argv[1]);
//    }

  appInstance = this;
  world = NULL;
  arena = NULL;

  // TODO: parseOptions(): --install, --uninstall, --help

  ILOG_OPEN(basename(argv[0]))
    ;
  if (joinFusion())
    {
      ILOG_DEBUG( "%s has joined fusion.", basename(argv[0]))
        ;
      appRecord->process = (char*) SHMALLOC(maestroObject->pool, sizeof(char)
          * strlen(argv[0]));
      sprintf(appRecord->process, "%s", argv[0]);
    }
  else
    {
      ILOG_ERROR("Please start Maestro...")
        ;
      exit( EXIT_FAILURE);
    }
}

AppBase::~AppBase()
{
  leaveFusion();
  ILOG_CLOSE()
    ;
}

AppMode
AppBase::appMode() const
{
  return appRecord->mode;
}

std::string
AppBase::title() const
{
  return appRecord->title;
}


void
AppBase::setTitle(std::string title)
{
  if (appRecord->title)
    SHFREE(maestroObject->pool, appRecord->title);
  appRecord->title = (char*) SHMALLOC(maestroObject->pool, sizeof(char)
      * (title.length() + 1));
  sprintf(appRecord->title, "%s", title.c_str());
}

void
AppBase::setAppMode(AppMode mode)
{
  appRecord->mode = mode;
}

char*
AppBase::getOSKText() const
{
  return maestroObject->OSK;
}

void
AppBase::setOSKText(const std::string& text)
{
  sprintf(maestroObject->OSK, "%s", text.c_str());
}

int
AppBase::getStatusBarHeight() const
{
  return maestroObject->statusBarHeight;
}

int
AppBase::enterArenaCB(FusionArena *arena, void *ctx)
{
  ILOG_DEBUG( "Joining ilixiArena...")
    ;
  fusion_arena_get_shared_field(arena, "MaestroObject",
      (void **) &maestroObject);
  fusion_arena_get_shared_field(arena, "AppVector", (void **) &appVector);

  if (appVector->count < appVector->capacity)
    {
      // TODO: lock app vector here.
      appRecord = (AppRecord*) SHMALLOC(maestroObject->pool, sizeof(AppRecord));
      appRecord->mode = Initialising;
      appRecord->fusionID = fusion_id(world);
      appRecord->pid = getpid();
      appRecord->title = NULL;
      appRecord->process = NULL;
      fusion_vector_add(appVector, appRecord);
    }

  ILOG_INFO( "Joined ilixiArena.")
    ;
  return 0;
}

int
AppBase::cleanArenaCB(FusionArena *arena, void *ctx, bool emergency)
{
  ILOG_DEBUG( "Cleaning ilixiArena...")
    ;
  if (appRecord)
    {
      int appIndex = fusion_vector_index_of(appVector, appRecord);
      fusion_vector_remove(appVector, appIndex);
      SHFREE(maestroObject->pool, appRecord->title);
      SHFREE(maestroObject->pool, appRecord);
      fusion_reactor_detach(maestroObject->reactor, &reaction);
    }
  ILOG_INFO( "ilixiArena is cleaned.")
    ;
  return 0;
}

void
AppBase::callMaestro(ReactorMessageType type, AppMode mode, unsigned int appID)
{
  if (type == Notification)
    {
      ILOG_DEBUG("Sending Notification via reactor...")
        ;
    }
  else if (type == ModeRequest)
    ILOG_DEBUG("Sending ModeRequest via reactor...")
      ;
  else if (type == OSKEvent)
    {
      if (appRecord->mode != Visible)
        return;
      ILOG_DEBUG("Sending OSKEvent mode %d via reactor...", mode)
        ;
    }
else    ILOG_DEBUG("Sending SwitchMode via reactor...");

    if (appID == 0)
      {
        ReactorMessage message(appRecord->fusionID, type, mode,
            appRecord->fusionID);
        fusion_reactor_dispatch(maestroObject->reactor, &message, false, NULL);
      }
    else
      {
        ReactorMessage message(appRecord->fusionID, type, mode, appID);
        fusion_reactor_dispatch(maestroObject->reactor, &message, false, NULL);
      }
  }

ReactionResult
AppBase::reactorCB(ReactorMessage *msg, void *ctx)
{
  switch (msg->type)
    {
  case SwitchMode:
    if (msg->senderFusionID == 1)
      {
        ILOG_INFO("Received SwitchMode message from Maestro.")
          ;
        if (msg->mode == Terminated)
          {
            appRecord->mode = Terminated;
            return RS_OK;
          }
else          ILOG_ERROR("Message mode is not supported!");
        }
      else
        {
          ILOG_ERROR("Sender is not authorised!");
          return RS_REMOVE;
        }
      break;

      default:
      ILOG_ERROR("Message type is not supported!");
    }
  return RS_DROP;
}

AppRecord* const
AppBase::getAppRecord(unsigned int appID) const
{
  AppRecord* record;
  for (int i = 0; i < appVector->count; i++)
    {
      record = (AppRecord *) fusion_vector_at(appVector, i);
      if (record->fusionID == appID)
        return record;
    }
  return NULL;
}

AppRecord* const
AppBase::getAppRecord(std::string process) const
{
  AppRecord* record;
  for (int i = 0; i < appVector->count; i++)
    {
      record = (AppRecord *) fusion_vector_at(appVector, i);
      if (strcmp(record->process, process.c_str()) == 0)
        return record;
    }
  return NULL;
}

//Designer*
//AppBase::getDesigner() const
//{
//  LOG_DEBUG("Designer: %s", maestroObject->designer->title());
//  return maestroObject->designer;
//}
//
//IDirectFB*
//AppBase::MDFBInterface() const
//{
//  return maestroObject->dfbInterface;
//}

bool
AppBase::joinFusion()
{
  // parse maestro config file
  int ilixi_world = 0;
  char lineBuffer[1024], tag[24], value[1000];
  std::ifstream infile;
  std::string file = ILIXI_DATADIR"maestro.conf";
  infile.open(file.c_str(), std::ifstream::in);
  if (!infile.good())
    {
      ILOG_FATAL("Unable to open configuration file: %s", file.c_str())
        ;
      exit( EXIT_FAILURE);
    }
  while (infile.good())
    {
      infile.getline(lineBuffer, 1024);
      if (lineBuffer[0] != '#' && lineBuffer[0] != '\n' && lineBuffer[0]
          != '\0')
        {
          sscanf(lineBuffer, "%s %*s %s", tag, value);
          if (strcmp(tag, "WORLD_INDEX") == 0)
            {
              ilixi_world = atoi(value);
              break;
            }
        }
    }
  infile.close();

  ILOG_DEBUG( "Attempting to enter fusion world as Slave")
    ;
  DirectResult result = fusion_enter(ilixi_world, 45, FER_ANY, &world);
  if (result == DR_OK)
    {
      bool master = fusion_master(world);
      ILOG_INFO("Entered Fusion world[%d] with FusionID: %lu, pid: %d as %s",
          fusion_world_index(world), fusion_id(world), getpid(),
          master ? "Master" : "Slave")
        ;

      if (master)
        {
          ILOG_FATAL("Maestro is not running.")
            ;
          leaveFusion();
          return false;
        }

      int ret;
      if (fusion_arena_enter(world, "ilixiArena", NULL, joinFusionCBW, NULL,
          &arena, &ret) != DR_OK)
        {
          ILOG_FATAL("Could not enter ilixiArena!")
            ;
          return false;
        }

      ILOG_DEBUG( "Initialising call mechanism...")
        ;
      if (fusion_call_init(&call, dispatchCBW, NULL, world) != DR_OK)
        {
          ILOG_ERROR("Unable to initialise dispatch_callback!")
            ;
          return false;
        }

      ILOG_DEBUG("Setting reactor dispatch callback...")
        ;
      if (fusion_reactor_set_dispatch_callback(maestroObject->reactor, &call,
          NULL) != DR_OK)
        {
          ILOG_ERROR("Unable to set dispatch_callback!")
            ;
          return false;
        }

      ILOG_DEBUG("Attaching reaction to global channel: 1")
        ;
      if (fusion_reactor_attach_channel(maestroObject->reactor, 1, reactionCBW,
          NULL, &reaction) != DR_OK)
        {
          ILOG_ERROR("Unable to attach reaction to channel!")
            ;
          return false;
        }

      ILOG_DEBUG("Attaching reaction to local channel: %d", appRecord->fusionID)
        ;
      if (fusion_reactor_attach_channel(maestroObject->reactor,
          appRecord->fusionID, reactionCBW, NULL, &reaction) != DR_OK)
        {
          ILOG_ERROR("Unable to attach reaction to channel!")
            ;
          return false;
        }
      ILOG_NOTICE( "Entered fusion world and completed initialising.")
        ;
      callMaestro(Notification, Initialising);
      return true;
    }
  else
    {
      ILOG_ERROR("Unable to enter fusion! Do you have read/write access to fusion node?")
        ;
      return false;
    }
}

void
AppBase::leaveFusion()
{
  if (arena)
    {
      int ret;
      void *ctx;
      DirectResult result = fusion_arena_exit(arena, NULL, cleanFusionCBW,
          &ctx, 1, &ret);
      if (result != DR_OK)
        ILOG_FATAL("Unable to exit ilixiArena")
          ;
    }
  if (world)
    {
      ILOG_DEBUG( "Leaving world...")
        ;
      DirectResult result = fusion_exit(world, true);
      if (result != DR_OK)
        ILOG_FATAL("Unable to exit ilixiWorld")
          ;
    }
}
