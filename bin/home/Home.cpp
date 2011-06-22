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

#include "Home.h"
#include "ui/GridLayout.h"
#include "ui/ToolButton.h"
#include "graphics/Painter.h"
#include <spawn.h>
#include <sigc++/bind.h>
#include <sqlite3.h>
#include "core/Logger.h"

using namespace ilixi;

//*****************************************************************
// AppButton
//*****************************************************************
AppButton::AppButton(std::string text, Widget* parent) :
  ToolButton(text, parent), _ch(0), _iconT(0)
{
  _focusIn = new TweenAnimation(this);
  _focusIn->addTween(new Tween(Tween::SINE, Tween::EASE_OUT, _ch, .7));
  _focusIn->addTween(new Tween(Tween::BOUNCE, Tween::EASE_OUT, _iconT, 4));
  _focusIn->setDuration(1000);

  _focusOut = new TweenAnimation(this);
  _focusOut->addTween(new Tween(Tween::SINE, Tween::EASE_OUT, _ch, .7, 0));
  _focusOut->addTween(new Tween(Tween::BOUNCE, Tween::EASE_OUT, _iconT, 4, 0));
  _focusOut->setDuration(1000);
}

AppButton::~AppButton()
{
  delete _focusIn;
  delete _focusOut;
}

void
AppButton::compose(const Rectangle& rect)
{
  Painter p(this);
  p.begin(rect);
  if (_ch >= 0)
    {
      p.getBrush()->setColor(Color(1, .4, 0.2, _ch));
      p.drawRoundRectangle(0, 0, width(), height(), 6, Painter::FillPath);
    }
  p.getBrush()->setColor(Color(1, 1, 1));
  p.drawLayout(layout(), font(), layoutPosition().x(), layoutPosition().y());

  float icoS = _iconT * 2;
  p.drawImage(
      icon(),
      Rectangle(iconPosition().x() - _iconT, iconPosition().y() - _iconT,
          icon()->width() + icoS, icon()->height() + icoS));

  p.end();
}

void
AppButton::focusInEvent()
{
  _focusOut->stop();
  _focusIn->start();
}

void
AppButton::focusOutEvent()
{
  _focusIn->stop();
  _focusOut->start();
}

//*****************************************************************
// HOME
//*****************************************************************
Home::Home(int argc, char* argv[]) :
  Application(argc, argv)
{
  setTitle("Home");
  parseApps();
  setLayout(new GridLayout(2, 4));

  for (unsigned int i = 0; i < _apps.size(); i++)
    {
      AppButton* appButton = new AppButton(_apps[i].title);
      appButton->setToolButtonStyle(ToolButton::IconAboveText);
      appButton->setIcon(_apps[i].iconPath, Size(48, 48));
      appButton->sigClicked.connect(
          sigc::bind<int>(sigc::mem_fun(this, &Home::spawnApp), i));
      addWidget(appButton);
    }
}

Home::~Home()
{
}

void
Home::spawnApp(int index)
{
  AppRecord* app = getAppRecord(_apps[index].basename);
  if (app)
    {
      callMaestro(SwitchMode, Visible, app->fusionID);
      hide(); // NOTE Home hides itself.
      return;
    }

  // if app is not running spawn it.
  char* path = const_cast<char*> (_apps[index].basename.c_str());
  char *arg[] =
    { path, NULL };
  pid_t id;
  int err;
  // FIXME NO_ZOMBIE here!!!
  if ((err = posix_spawn(&id, arg[0], NULL, NULL, arg, environ)) != 0)
    {
      ILOG_ERROR("posix_spawn() error: %d", err);
      exit(EXIT_FAILURE);
    }
}

void
Home::parseApps()
{
  sqlite3 *db;
  char *errorMsg;
  char **appTable;
  int nrow, ncol;

  if (sqlite3_open(ILIXI_DATADIR"ilixi.db", &db) != SQLITE_OK)
    {
      ILOG_ERROR("Can not open ilixi database.");
      return;
    }

  if (sqlite3_get_table(db, "select * from APPS", &appTable, &nrow, &ncol,
      &errorMsg) != SQLITE_OK)
    {
      ILOG_ERROR("SQL Error: %s", errorMsg);
      sqlite3_free(errorMsg);
    }
  else
    {
      for (int row = 1; row <= nrow; row++)
        {
          AppStruct record;
          record.basename = appTable[3 * row];
          record.title = appTable[3 * row + 1];
          if (strcmp(appTable[3 * row + 2], "default") == 0)
            record.iconPath = ILIXI_DATADIR"default.png";
          else
            record.iconPath = appTable[3 * row + 2];
          _apps.push_back(record);
        }
      sqlite3_free_table(appTable);
    }
  sqlite3_close(db);
}

int
main(int argc, char* argv[])
{
  Home app(argc, argv);
  app.exec();
  return 0;
}
