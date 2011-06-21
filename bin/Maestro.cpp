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

#include "core/IFusion.h"
#include "core/Logger.h"
#include <sys/stat.h>
#include <spawn.h>
#include <sys/wait.h>
#include <fstream>
#include <libgen.h>
#include <sqlite3.h>
#include <syslog.h>

using namespace ilixi;

/*
 * Maestro provides a simple mechanism to communicate foreground permissions to applications. Upon start, Maestro
 * creates a Fusion world and a shared memory pool for storing a record of each active application inside a vector (AppVector).
 * A Fusion arena is also set up and used for providing access to the pool and a message reactor by all applications.
 *
 * Each application is responsible for creating and setting up its own record and placing the record in AppVector when it
 * is initialised. Therefore, it is also an application's responsibility to remove its record when it is destroyed. This
 * is done automatically in classes derived from ilixi::Application. During the course of operation these records are
 * accessed by Maestro for controlling application behaviour, e.g. sending them to background or setting properties of their windows.
 * (At most one application can have foreground permission and receive incoming DirectFB input events at any time.)
 *
 * A Fusion reactor is used for sending display commands to active applications and receiving back status information.
 * A command could be sent to a single application using the channel which corresponds to the application's FusionID
 * or globally using channel 1. Applications should use channel 0 to send messages to Maestro.
 */

// This structure is used to store parameters.
struct Parameters
{
  // fusion
  int WORLD_INDEX;
  int POOL_SIZE;
  int APP_VECTOR_SIZE;
  int OSK_SIZE;
  int CLIPBOARD_SIZE;
  // layer background
  bool BG_MODE_IMAGE;
  char BG_IMAGE[150];
  short BG_COLOR_R;
  short BG_COLOR_G;
  short BG_COLOR_B;
  // statusbar
  char STATUSBAR[150];
  short STATUSBAR_HEIGHT;
  // path home application
  char HOME[150];
  // path to osk application
  char OSK[150];
};

Parameters config;

// Pointer to ilixi dfb-fusion world
FusionWorld *world = NULL;
// Pointer to ilixi dfb-fusion shared memory pool
FusionSHMPoolShared *pool = NULL;
// Pointer to ilixi dfb-fusion arena
FusionArena *arena = NULL;
// Fusion call object
FusionCall call;
// Maestro's reaction to incoming messages
Reaction reaction;
// Pointer to MaestroObject instance inside shared memory pool
MaestroObject *maestro;
// Pointer to shared dfb-FusionVector instance inside shared memory pool
FusionVector *appVector;

// DirectFB interface
IDirectFB *dfb;
// DirectFB graphics device description
DFBGraphicsDeviceDescription deviceDescription;
// DirectFB layer interface
IDirectFBDisplayLayer *layer;
// DirectFB layer configuration
DFBDisplayLayerConfig layerConfig;
// Background surface of application
IDirectFBSurface *bgsurface;
// Pointer to application window
IDirectFBWindow *window;
// Pointer to surface of application window
IDirectFBSurface *window_surface;
// DirectFB window identification of this application
DFBWindowID id;
// DirectFB Event buffer of application window
IDirectFBEventBuffer *buffer;

AppRecord *currentApp = NULL;

bool
initFusion(bool slaveMode = false);

void
exitFusion(bool slaveMode = false);

void
initDFB(int argc, char **argv);

void
releaseDFB();

void
callApp(int channel, ReactorMessageType type, AppMode mode = Terminated);

void
notifyStatusBar(unsigned int AppIndex, AppMode mode = Terminated);

void
createFork(int argc, char **argv);

int
initArenaCB(FusionArena*, void*);

int
joinArenaCB(FusionArena *arena, void *ctx);

int
cleanArenaCB(FusionArena *arena, void *ctx, bool emergency);

ReactionResult
reactionCB(const void *msgData, void *ctx);

ReactionResult
reaction2StatusBar(ReactorMessage* msg);

ReactionResult
reaction2OSK(ReactorMessage* msg);

ReactionResult
reaction2Application(ReactorMessage* msg);

FusionCallHandlerResult
dispatchCB(int caller, int call_arg, void *call_ptr, void *ctx,
    unsigned int serial, int *ret_val);

void
parseConfig();

//*****************************************************************
// Start Maestro.
//*****************************************************************
void
start(int argc, char **argv)
{
  parseConfig();
  if (initFusion(true))
    {
      printf("Maestro is already running!\n");
      ILOG_CLOSE();
    }
  else
    createFork(argc, argv);
}

//*****************************************************************
// Stop Maestro.
//*****************************************************************
void
stop()
{
  parseConfig();
  if (initFusion(true))
    {
      callApp(1, SwitchMode, Terminated);
      exitFusion(true);
    }
}

//*****************************************************************
// Print list of running applications.
//*****************************************************************
void
status()
{
  parseConfig();
  if (initFusion(true))
    {
      int appCount = fusion_vector_size(appVector);
      if (appCount > 0)
        {
          printf("#\tPID\t%-20s\tStatus\n", "Title");
          printf(
              "------------------------------------------------------------\n");
          for (int i = 0; i < appCount; i++)
            {
              AppRecord *app = (AppRecord *) fusion_vector_at(appVector, i);
              printf("%d\t%d\t%-20s\t%s\n", i, app->pid, app->title,
                  (app->mode == Hidden) ? "Background" : "Foreground");
            }
        }
      else
        printf("There aren't any applications running...\n");
      exitFusion(true);
    }
  else
    printf("Please start Maestro\n");
  ILOG_CLOSE();
}

//*****************************************************************
// Connect to ilixi dfb-fusion world and create objects in shared
// memory.
// If slaveMode is true and process is master returns false.
//*****************************************************************
bool
initFusion(bool slaveMode)
{
  ILOG_DEBUG("Attempting to enter DFB-World as %s...", slaveMode ? "Slave"
      : "Master");
  // FIXME Fusion ABI version = 45
  DirectResult result = fusion_enter(config.WORLD_INDEX, 45, FER_ANY, &world);
  if (result == DR_OK)
    {
      bool master = fusion_master(world);
      ILOG_INFO("Entered DFB-World[%d] with ID: %lu, pid: %d as %s",
          fusion_world_index(world), fusion_id(world), getpid(),
          master ? "Master" : "Slave");

      if (slaveMode && master)
        {
          ILOG_WARNING("Maestro is not running.");
          exitFusion(true);
          return false;
        }

      if (master)
        {
          ILOG_DEBUG("Maestro-Master is creating DFB-Pool...");
          if (fusion_shm_pool_create(world, "ilixiPool", config.POOL_SIZE, 0,
              &pool) != DR_OK)
            {
              ILOG_FATAL("fusion_shm_pool_create() failed!");
              return false;
            }
        }

      int ret;
      if (fusion_arena_enter(world, "ilixiArena", initArenaCB, joinArenaCB,
          NULL, &arena, &ret) != DR_OK)
        {
          ILOG_FATAL("fusion_arena_enter() failed!");
          return false;
        }

      if (master)
        {
          ILOG_DEBUG("Maestro-Master is setting up DFB-Reactor...");
          maestro->reactor = fusion_reactor_new(sizeof(ReactorMessage),
              "ilixiReactor", world);
          if (!maestro->reactor)
            {
              ILOG_FATAL("fusion_reactor_new() failed!");
              return false;
            }
          // Attach a local reaction to the reactor at channel 0
          if (fusion_reactor_attach(maestro->reactor, reactionCB, NULL,
              &reaction) != DR_OK)
            {
              ILOG_FATAL("fusion_reactor_attach() failed!");
              return false;
            }
          if (fusion_call_init(&call, dispatchCB, NULL, world) != DR_OK)
            {
              ILOG_FATAL("fusion_call_init() failed!");
              return false;
            }
          // Have the call executed when a dispatched message has been processed by all recipients.
          if (fusion_reactor_set_dispatch_callback(maestro->reactor, &call,
              NULL) != DR_OK)
            {
              ILOG_FATAL("fusion_reactor_set_dispatch_callback() failed!");
              return false;
            }
        }

      return true;
    }
  else
    {
      ILOG_FATAL("Could not create DFB-World!");
      return false;
    }
}

//*****************************************************************
// Try to leave world and arena. If slaveMode is false (i.e. Master)
// destroy pool.
//*****************************************************************
void
exitFusion(bool slaveMode)
{
  if (arena)
    {
      ILOG_DEBUG("Maestro-%s (pid: %d) is leaving DFB-Arena...", slaveMode ? "Slave"
          : "Master", getpid());
      int ret;
      void *ctx;
      if (slaveMode)
        fusion_arena_exit(arena, NULL, NULL, &ctx, 0, &ret);
      else
        fusion_arena_exit(arena, cleanArenaCB, NULL, &ctx, 0, &ret);
      arena = NULL;
    }
  if (pool && !slaveMode)
    {
      ILOG_DEBUG("Maestro-Master (pid: %d) is destroying DFB-Pool...", getpid());
      fusion_shm_pool_destroy(world, pool);
      pool = NULL;
    }
  if (world)
    {
      ILOG_DEBUG("Maestro-%s (pid: %d) is leaving DFB-World...", slaveMode ? "Slave"
          : "Master", getpid());
      fusion_exit(world, false);
      world = NULL;
    }
  ILOG_INFO("Maestro (pid: %d) has left DFB-Fusion!", getpid());
}

//*****************************************************************
// Allocate shared memory space and create fields.
//*****************************************************************
int
initArenaCB(FusionArena *arena, void *ctx)
{
  ILOG_DEBUG("Maestro-Master is initialising DFB-Arena...");
  maestro = (MaestroObject*) SHMALLOC(pool, sizeof(MaestroObject));
  fusion_arena_add_shared_field(arena, "MaestroObject", (void *) maestro);
  maestro->statusBarHeight = config.STATUSBAR_HEIGHT;

  maestro->pool = pool;
  maestro->OSK = (char*) SHMALLOC(pool, sizeof(char) * config.OSK_SIZE);
  sprintf(maestro->OSK, "ilixi OSK");

  appVector = (FusionVector*) SHMALLOC(pool, sizeof(FusionVector));
  fusion_arena_add_shared_field(arena, "AppVector", (void *) appVector);
  fusion_vector_init(appVector, config.APP_VECTOR_SIZE, pool);

  ILOG_INFO("Maesto-Master initialised DFB-Arena!");
  return 0;
}

//*****************************************************************
// Map shared memory objects.
//*****************************************************************
int
joinArenaCB(FusionArena *arena, void *ctx)
{
  ILOG_DEBUG("Maestro-Slave is joining DFB-Arena...");
  fusion_arena_get_shared_field(arena, "MaestroObject", (void **) &maestro);
  fusion_arena_get_shared_field(arena, "AppVector", (void **) &appVector);
  ILOG_INFO("Maestro-Slave joined DFB-Arena!");
  return 0;
}

//*****************************************************************
// Deallocate objects in shared memory.
//*****************************************************************
int
cleanArenaCB(FusionArena *arena, void *ctx, bool emergency)
{
  ILOG_DEBUG("Maestro-Master is cleaning DFB-Arena...");
  fusion_vector_destroy(appVector);
  SHFREE(pool, appVector);
  if (maestro->reactor)
    {
      fusion_reactor_destroy(maestro->reactor);
      fusion_reactor_free(maestro->reactor);
    }
  SHFREE(pool, maestro->OSK);
  SHFREE(pool, maestro);
  ILOG_INFO("Maestro-Master cleaned DFB-Arena!");
  return 0;
}

//*****************************************************************
// Initialise DFB interfaces.
//*****************************************************************
void
initDFB(int argc, char **argv)
{
  ILOG_DEBUG("Initialising DirectFB...");
  DFBCHECK(DirectFBInit(&argc, &argv));
  DFBCHECK(DirectFBCreate(&dfb));
  dfb->GetDeviceDescription(dfb, &deviceDescription);
  DFBCHECK(dfb->GetDisplayLayer(dfb, DLID_PRIMARY, &layer));
  layer->SetCooperativeLevel(layer, DLSCL_ADMINISTRATIVE);

  if (!((deviceDescription.blitting_flags & DSBLIT_BLEND_ALPHACHANNEL)
      && (deviceDescription.blitting_flags & DSBLIT_BLEND_COLORALPHA)))
    {
      layerConfig.flags = DLCONF_BUFFERMODE;
      layerConfig.buffermode = DLBM_BACKSYSTEM;
      layer->SetConfiguration(layer, &layerConfig);
    }

  layer->GetConfiguration(layer, &layerConfig);
  layer->EnableCursor(layer, true);

  if (config.BG_MODE_IMAGE)
    {
      DFBSurfaceDescription bgsurfaceDesc;
      IDirectFBImageProvider *provider;
      DFBCHECK(dfb->CreateImageProvider(dfb, config.BG_IMAGE, &provider));
      bgsurfaceDesc.flags = (DFBSurfaceDescriptionFlags)(
          DSDESC_WIDTH | DSDESC_HEIGHT | DSDESC_CAPS);
      bgsurfaceDesc.width = layerConfig.width;
      bgsurfaceDesc.height = layerConfig.height;
      bgsurfaceDesc.caps = DSCAPS_SHARED;
      dfb->CreateSurface(dfb, &bgsurfaceDesc, &bgsurface);
      provider->RenderTo(provider, bgsurface, NULL);
      provider->Release(provider);
      layer->SetBackgroundImage(layer, bgsurface);
      layer->SetBackgroundMode(layer, DLBM_IMAGE);
    }
  else
    {
      layer->SetBackgroundColor(layer, config.BG_COLOR_R, config.BG_COLOR_G,
          config.BG_COLOR_B, 1);
    }
}

//*****************************************************************
// Release DFB interfaces.
//*****************************************************************
void
releaseDFB()
{
  ILOG_DEBUG("Releasing DFB interfaces...");
  layer->Release(layer);
  if (bgsurface)
    {
      bgsurface->Release(bgsurface);
      bgsurface->ReleaseSource(bgsurface);
    }
  dfb->Release(dfb);
  ILOG_INFO("DFB interfaces are released.");
}

//*****************************************************************
// Called when a dispatched message has been processed by all recipients.
//*****************************************************************
FusionCallHandlerResult
dispatchCB(int caller, int call_arg, void *call_ptr, void *ctx,
    unsigned int serial, int *ret_val)
{
  if (call_arg == 1)
    {
      ILOG_DEBUG("Message received by fusionee.");
      return FCHR_RETURN;
    }
  return FCHR_RETAIN;
}

//*****************************************************************
// Called when there is a new message to be consumed in reactor.
//*****************************************************************
ReactionResult
reactionCB(const void *msgData, void *ctx)
{
  ReactorMessage* msg = (ReactorMessage*) msgData;
  if (msg->senderFusionID == 2)
    return reaction2StatusBar(msg);
  else if (msg->senderFusionID == 3)
    return reaction2OSK(msg);
  else
    return reaction2Application(msg);
}

//*****************************************************************
// Handle incoming messages from StatusBar.
//*****************************************************************
ReactionResult
reaction2StatusBar(ReactorMessage* msg)
{
  switch (msg->type)
    {
  case ModeRequest:
    {
      ILOG_INFO("Received ModeRequest from StatusBar.");
      if (msg->mode == Hidden)
        callApp(msg->senderFusionID, SwitchMode, Hidden);
      else if (msg->mode == Terminated)
        callApp(msg->senderFusionID, SwitchMode, Terminated);
      else if (msg->mode == Visible)
        callApp(msg->senderFusionID, SwitchMode, Visible);
      else
        {
          ILOG_ERROR("Requested mode is not supported for StatusBar!");
          return RS_DROP;
        }
      break;
    }

  case SwitchMode:
    {
      // NOTE msg->appIndex refers to applications Fusion ID.
      ILOG_INFO("Received SwitchMode from StatusBar, target: %d.", msg->appID);
      if (msg->mode == Hidden)
        callApp(msg->appID, SwitchMode, Hidden);
      else if (msg->mode == Terminated)
        callApp(msg->appID, SwitchMode, Terminated);
      else if (msg->mode == Visible)
        {
          if (currentApp)
            callApp(currentApp->fusionID, SwitchMode, Hidden);
          callApp(msg->appID, SwitchMode, Visible);
        }
      else
        {
          ILOG_ERROR("Requested mode is not supported for StatusBar!");
          return RS_DROP;
        }
      break;
    }

  case Notification:
    ILOG_INFO("Received Notification (%d) from StatusBar.", msg->mode);
    break;

  default:
    ILOG_ERROR("StatusBar message type is not supported!");
    return RS_DROP;
    }
  return RS_OK;
}

//*****************************************************************
// Handle incoming messages from OSK.
//*****************************************************************
ReactionResult
reaction2OSK(ReactorMessage* msg)
{
  switch (msg->type)
    {
  case ModeRequest:
    {
      ILOG_INFO("Received ModeRequest from OSK.");
      if (msg->mode == Hidden)
        callApp(3, SwitchMode, Hidden);
      else if (msg->mode == Terminated)
        callApp(3, SwitchMode, Terminated);
      else if (msg->mode == Visible)
        {
          static bool startHidden = false;
          if (!startHidden)
            startHidden = true;
          else
            callApp(3, SwitchMode, Visible);
        }
      else
        {
          ILOG_ERROR("Requested mode (%d) is not supported for OSK!", msg->mode);
          return RS_DROP;
        }
      break;
    }

  case Notification:
    ILOG_INFO("Received Notification (%d) from OSK.", msg->mode);
    break;

  case OSKEvent:
    ILOG_INFO("Received OSKEvent from OSK.");
    if (currentApp)
      callApp(currentApp->fusionID, OSKEvent, Ready);
    break;

  default:
    ILOG_ERROR("OSK message type is not supported!");
    return RS_DROP;
    }
  return RS_OK;
}

//*****************************************************************
// Handle incoming messages from application.
//*****************************************************************
ReactionResult
reaction2Application(ReactorMessage* msg)
{

  AppRecord* app;
  for (int i = 0; i < appVector->count; i++)
    {
      AppRecord* record = (AppRecord *) fusion_vector_at(appVector, i);
      if (record->fusionID == msg->appID)
        app = record;
    }

  switch (msg->type)
    {
  case ModeRequest:
    {
      ILOG_INFO("Received ModeRequest from %s (ID: %d).", app->title, msg->senderFusionID);
      switch (msg->mode)
        {
      case Visible:
        // if there is a visible app hide it.
        if (currentApp)
          callApp(currentApp->fusionID, SwitchMode, Hidden);

        callApp(msg->senderFusionID, SwitchMode, Visible);
        return RS_OK;

      default:
        ILOG_ERROR("ModeRequest mode (%d) is not supported!", msg->mode);
        return RS_DROP;
        }
    }

  case SwitchMode:
    // This mode is only available for Home.
    if (msg->senderFusionID != 4)
      {
        ILOG_ERROR("Message type is not supported!");
        return RS_DROP;
      }

    if (msg->mode == Visible)
      {
        // NOTE Home should hide itself.
        callApp(msg->appID, SwitchMode, Visible);
        return RS_OK;
      }
    ILOG_ERROR("Requested mode is not supported for Home!");
    return RS_DROP;

  case Notification:
    {
      switch (msg->mode)
        {
      case Initialising:
        ILOG_INFO( "ID: %d is initialising.", msg->senderFusionID);
        return RS_OK;

      case Ready:
        ILOG_INFO( "%s (ID: %d) is ready.", app->title, msg->senderFusionID);
        return RS_OK;

      case Visible:
        ILOG_INFO( "ID: %d is visible.", msg->senderFusionID);
        currentApp = app;
        notifyStatusBar(currentApp->fusionID, Visible);
        return RS_OK;

      case Hidden:
        ILOG_INFO( "%s (ID: %d) is hidden.", app->title, msg->senderFusionID);
        return RS_OK;

      case Terminated:
        {
          ILOG_INFO( "ID: %d is terminating.", msg->senderFusionID);
          // if Home and StatusBar is still running show it.
          if (appVector->count >= 2 && msg->senderFusionID != 4)
            callApp(4, SwitchMode, Visible);
          else
            currentApp = NULL;
          return RS_OK;
        }
      default:
        ILOG_ERROR("Notification mode is not supported!");
        return RS_DROP;
        }
    }

  case OSKEvent:
    switch (msg->mode)
      {
    case Visible:
      {
        ILOG_DEBUG("Let OSK be Visible!");
        callApp(3, SwitchMode, Visible);
      }
      break;
    case Hidden:
      {
        ILOG_DEBUG("Let OSK be Hidden!");
        callApp(3, SwitchMode, Hidden);
      }
      break;
    default:
      ILOG_ERROR("Notification mode is not supported!");
      return RS_DROP;
      }
    return RS_OK;

  default:
    ILOG_ERROR("Message type is not supported!");
    return RS_DROP;
    }
}

//*****************************************************************
// Sends a message to an application.
//*****************************************************************
void
callApp(int channel, ReactorMessageType type, AppMode mode)
{
  ILOG_DEBUG("Sending %s mode via reactor using channel %d...",
      (mode==Visible) ? "Visible" : (mode == Hidden) ? "Hidden": "Terminated", channel);
  ReactorMessage message(1, type, mode);
  fusion_reactor_dispatch_channel(maestro->reactor, channel, &message,
      sizeof(message), false, NULL);
}

//*****************************************************************
// Sends a message to the Statusbar notifying an AppMode change.
//*****************************************************************
void
notifyStatusBar(unsigned int AppIndex, AppMode mode)
{
  ILOG_DEBUG("Sending message to Statusbar ");
  ReactorMessage message(1, Notification, mode, AppIndex);
  fusion_reactor_dispatch_channel(maestro->reactor, 2, &message,
      sizeof(message), false, NULL);
}

//*****************************************************************
// Create a background process and run Maestro.
//*****************************************************************
void
createFork(int argc, char **argv)
{
  pid_t child_pid = fork();
  if (child_pid < 0)
    {
      ILOG_FATAL("Unable to start Maestro using fork()");
      exit( EXIT_FAILURE);
    }
  else if (child_pid > 0)
    {
      ILOG_DEBUG("Parent is terminating...");
      exit( EXIT_SUCCESS);
    }
  else
    {
      sleep(1);
      umask(0);
      pid_t sid = setsid();
      if (sid < 0)
        {
          ILOG_FATAL("Unable to create a Session ID for Maestro!");
          exit( EXIT_FAILURE);
        }

      if ((chdir(ILIXI_DATADIR)) < 0)
        {
          ILOG_FATAL("Unable to change current working directory to %s!", ILIXI_DATADIR);
          exit( EXIT_FAILURE);
        }

      //      close( STDIN_FILENO);
      //      close( STDOUT_FILENO);
      //      close( STDERR_FILENO);

      if (initFusion())
        {

          initDFB(argc, argv);
          ILOG_NOTICE("Maestro (pid: %d) is running now!", getpid());

          int err;
          pid_t statusbar_id, osk_id, home_id;

          char *statArg[] =
            { config.STATUSBAR, NULL };
          if ((err = posix_spawn(&statusbar_id, statArg[0], NULL, NULL,
              statArg, environ)) != 0)
            {
              ILOG_ERROR("posix_spawn() error: %d", err);
              exit( EXIT_FAILURE);
            }
          ILOG_DEBUG("StatusBar (%d) is running...", statusbar_id);

          while (fusion_vector_size(appVector) < 1)
            sleep(1);

          char *oskArg[] =
            { config.OSK, NULL };
          if ((err = posix_spawn(&osk_id, oskArg[0], NULL, NULL, oskArg,
              environ)) != 0)
            {
              ILOG_ERROR("posix_spawn() error: %d", err);
              exit( EXIT_FAILURE);
            }
          ILOG_DEBUG("OSK (%d) is running...", osk_id);

          while (fusion_vector_size(appVector) < 2)
            sleep(1);

          char *homeArg[] =
            { config.HOME, NULL };
          if ((err = posix_spawn(&home_id, homeArg[0], NULL, NULL, homeArg,
              environ)) != 0)
            {
              ILOG_ERROR("posix_spawn() error: %d", err);
              exit( EXIT_FAILURE);
            }
          ILOG_DEBUG("Home (%d) is running...", home_id);

          int childExitStatus;
          pid_t ws = waitpid(statusbar_id, &childExitStatus, 0);

          if (!WIFEXITED(childExitStatus))
            ILOG_ERROR("StatusBar exited with an error: %d", WEXITSTATUS(childExitStatus));
          else if (WIFSIGNALED(childExitStatus))
            ILOG_ERROR("StatusBar exited due to a signal: %d", WTERMSIG(childExitStatus));
          else
            ILOG_NOTICE("StatusBar is terminated successfully; now terminating remaining applications.");

          // Send message to all applications and wait until they are all terminated.
          callApp(1, SwitchMode, Terminated);
          while (fusion_vector_has_elements(appVector))
            sleep(1);

          releaseDFB();
          exitFusion();
          ILOG_NOTICE("Maestro ends.");
        }
      else
        exit( EXIT_FAILURE);
      ILOG_CLOSE();
    }
}

void
parseConfig()
{
  // initialise all parameters before parsing them...
  config.WORLD_INDEX = 0;
  config.POOL_SIZE = 1024;
  config.APP_VECTOR_SIZE = 10;
  config.OSK_SIZE = 10;
  config.CLIPBOARD_SIZE = 10;
  config.BG_MODE_IMAGE = false;
  sprintf(config.BG_IMAGE, "");
  config.BG_COLOR_R = 0;
  config.BG_COLOR_G = 0;
  config.BG_COLOR_B = 0;
  sprintf(config.STATUSBAR, ILIXI_BINDIR"ilixi_statusbar");
  config.STATUSBAR_HEIGHT = 50;
  sprintf(config.OSK, ILIXI_BINDIR"ilixi_osk");
  sprintf(config.HOME, ILIXI_BINDIR"ilixi_home");

  char lineBuffer[1024], tag[24], value[1000];
  std::ifstream infile;
  std::string file = ILIXI_DATADIR"maestro.conf";
  infile.open(file.c_str(), std::ifstream::in);
  if (infile.good())
    ILOG_DEBUG("Parsing configuration file: %s", file.c_str());
  else
    {
      ILOG_FATAL("Unable to open configuration file: %s", file.c_str());
      exit( EXIT_FAILURE);
    }
  while (infile.good())
    {
      infile.getline(lineBuffer, 1024);
      if (lineBuffer[0] != '#' && lineBuffer[0] != '\n' && lineBuffer[0]
          != '\0')
        {
          sscanf(lineBuffer, "%s %*s %s", tag, value);
          ILOG_DEBUG("%s: %s", tag, value);
          if (strcmp(tag, "WORLD_INDEX") == 0)
            config.WORLD_INDEX = atoi(value);
          else if (strcmp(tag, "POOL_SIZE") == 0)
            config.POOL_SIZE = atoi(value);
          else if (strcmp(tag, "APP_VECTOR_SIZE") == 0)
            config.APP_VECTOR_SIZE = atoi(value);
          else if (strcmp(tag, "OSK_SIZE") == 0)
            config.OSK_SIZE = atoi(value);
          else if (strcmp(tag, "CLIPBOARD_SIZE") == 0)
            config.CLIPBOARD_SIZE = atoi(value);

          else if (strcmp(tag, "BACKGROUND_MODE") == 0)
            {
              if (strcmp(value, "IMAGE") == 0)
                config.BG_MODE_IMAGE = true;
              else
                config.BG_MODE_IMAGE = false;
            }
          else if (strcmp(tag, "BACKGROUND_IMAGE") == 0)
            sprintf(config.BG_IMAGE, ILIXI_DATADIR"%s", value);
          else if (strcmp(tag, "BACKGROUND_COLOR_R") == 0)
            config.BG_COLOR_R = atoi(value);
          else if (strcmp(tag, "BACKGROUND_COLOR_G") == 0)
            config.BG_COLOR_G = atoi(value);
          else if (strcmp(tag, "BACKGROUND_COLOR_B") == 0)
            config.BG_COLOR_B = atoi(value);

          else if (strcmp(tag, "STATUSBAR") == 0)
            sprintf(config.STATUSBAR, ILIXI_BINDIR"%s", value);
          else if (strcmp(tag, "STATUSBAR_HEIGHT") == 0)
            config.STATUSBAR_HEIGHT = atoi(value);

          else if (strcmp(tag, "HOME") == 0)
            sprintf(config.HOME, ILIXI_BINDIR"%s", value);

          else if (strcmp(tag, "OSK") == 0)
            sprintf(config.OSK, ILIXI_BINDIR"%s", value);
        }
    }
  infile.close();
}

//*****************************************************************
// Prints a list of command line options
//*****************************************************************
void
printUsage()
{
  printf("\nUsage: ilixi_maestro [OPTION...]\n");
  printf("      --start                              Starts Maestro\n");
  printf("      --stop                               Stops Maestro\n");
  printf("      --restart                            Restarts Maestro\n");
  printf(
      "      --status                             List running applications\n");
  printf(
      "      --install <Path> <Title> <IconPath>  Adds an application to Home\n");
  printf(
      "      --uninstall <Path>                   Removes an application from Home\n\n");
}

//*****************************************************************
int
main(int argc, char **argv)
{
  if (argc < 2)
    printUsage();
  else
    {
      ilixi_log_init(basename(argv[0]), LOG_DAEMON);
      if (strcmp(argv[1], "--start") == 0)
        start(argc, argv);
      else if (strcmp(argv[1], "--stop") == 0)
        {
          stop();
          ILOG_CLOSE();
        }
      else if (strcmp(argv[1], "--restart") == 0)
        {
          stop();
          sleep(1);
          start(argc, argv);
        }
      else if (strcmp(argv[1], "--status") == 0)
        status();
      else if (!strcmp(argv[1], "--install") && argc >= 4)
        {
          if (access(argv[2], F_OK) != 0)
            {
              ILOG_FATAL("File (%s) is not accessible!\n", argv[2]);
              exit( EXIT_FAILURE);
            }

          if (argc == 5 && access(argv[4], F_OK) != 0)
            {
              ILOG_FATAL("Icon (%s) is not accessible!\n", argv[2]);
              exit( EXIT_FAILURE);
            }

          sqlite3 *db;
          char *errorMsg;
          char query[1024];

          if (sqlite3_open(ILIXI_DATADIR"ilixi.db", &db) != SQLITE_OK)
            {
              ILOG_FATAL("Can not open ilixi database.");
              exit( EXIT_FAILURE);
            }

          sprintf(query, "delete from APPS where PATH like '%s'", argv[2]);
          if (sqlite3_exec(db, query, NULL, NULL, &errorMsg) != SQLITE_OK)
            {
              ILOG_ERROR("SQL Error: %s", errorMsg);
              sqlite3_free(errorMsg);
            }

          if (argc == 5)
            sprintf(
                query,
                "insert into APPS (PATH, TITLE, ICON) VALUES('%s', '%s', '%s')",
                argv[2], argv[3], argv[4]);
          else if (argc == 4)
            sprintf(
                query,
                "insert into APPS (PATH, TITLE, ICON) VALUES('%s', '%s', 'default')",
                argv[2], argv[3]);

          if (sqlite3_exec(db, query, NULL, NULL, &errorMsg) != SQLITE_OK)
            {
              ILOG_ERROR("SQL Error: %s", errorMsg);
              sqlite3_free(errorMsg);
            }

          sqlite3_close(db);
          ILOG_CLOSE();

        }
      else if (!strcmp(argv[1], "--uninstall") && argc == 3)
        {
          sqlite3 *db;
          char *errorMsg;
          char query[1024];

          if (sqlite3_open(ILIXI_DATADIR"ilixi.db", &db) != SQLITE_OK)
            {
              ILOG_FATAL("Can not open ilixi database.");
              exit( EXIT_FAILURE);
            }

          sprintf(query, "delete from APPS where PATH like '%s'", argv[2]);
          if (sqlite3_exec(db, query, NULL, NULL, &errorMsg) != SQLITE_OK)
            {
              ILOG_ERROR("SQL Error: %s", errorMsg);
              sqlite3_free(errorMsg);
            }

          sqlite3_close(db);
          ILOG_CLOSE();
        }
      else
        {
          printUsage();
          ILOG_CLOSE();
        }
    }
}
