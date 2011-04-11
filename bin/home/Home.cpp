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

#include "Home.h"
#include "ui/GridLayout.h"
#include "ui/ToolButton.h"
#include "graphics/Painter.h"
#include <spawn.h>
#include <sigc++/bind.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include "core/Logger.h"

using namespace ilixi;

AppButton::AppButton(std::string text, Widget* parent) :
  ToolButton(text, parent)
{
}

AppButton::~AppButton()
{
}

void
AppButton::compose(const Rectangle& rect)
{
  Painter p(this);
  p.begin(rect);

  if (hasIcon())
    p.drawImage(icon(), iconPosition().x(), iconPosition().y());

  p.getBrush()->setColor(Color(0, 0, 0, 0.5));
  p.drawRoundRectangle(0, layoutPosition().y(), width(), textLayoutHeight(), 6,
      Painter::FillPath);
  p.getBrush()->setColor(Color(1, 1, 1));
  p.drawLayout(layout(), font(), layoutPosition().x(), layoutPosition().y());

  p.end();
}

Home::Home(int argc, char* argv[]) :
  Application(argc, argv)
{
  setTitle("Home");
  parseApps();
  setLayout(new GridLayout(1, 2));

  for (unsigned int i = 0; i < _apps.size(); i++)
    {
      AppButton* app = new AppButton(_apps[i].title);
      app->setToolButtonStyle(ToolButton::IconAboveText);
      app->setIcon(_apps[i].iconPath, Size(48, 48));
      app->sigClicked.connect(sigc::bind<int>(sigc::mem_fun(this,
          &Home::spawnApp), i));
      addWidget(app);
      ILOG_DEBUG("APP %s", app->text().c_str());
    }
}

Home::~Home()
{
}

void
Home::spawnApp(int index)
{
  AppRecord* app = getAppRecord(_apps[index].path);
  if (app)
    {
      callMaestro(SwitchMode, Visible, app->fusionID);
      hide(); // NOTE Home hides itself.
      return;
    }

  // if app is not running spawn it.
  char* path = const_cast<char*> (_apps[index].path.c_str());
  char *arg[] =
    { path, NULL };
  pid_t id;
  int err;
  if ((err = posix_spawn(&id, arg[0], NULL, NULL, arg, environ)) != 0)
    {
      ILOG_ERROR("posix_spawn() error: %d", err);
      exit( EXIT_FAILURE);
    }
}

void
Home::parseApps()
{
  xmlDocPtr doc;
  xmlNodePtr cur;

  doc = xmlParseFile(ILIXI_DATADIR"apps.conf");

  if (doc == NULL)
    {
      ILOG_ERROR("Apps file not parsed successfully!");
      return;
    }

  cur = xmlDocGetRootElement(doc);

  if (cur == NULL)
    {
      ILOG_ERROR("Empty document!");
      xmlFreeDoc(doc);
      return;
    }

  if (xmlStrcmp(cur->name, (const xmlChar *) "applications"))
    {
      ILOG_ERROR("Not a valid apps file!");
      xmlFreeDoc(doc);
      return;
    }

  cur = cur->xmlChildrenNode;
  while (cur != NULL)
    {

      xmlChar *titleAttr = xmlGetProp(cur, (const xmlChar *) "title");
      xmlChar *iconAttr = xmlGetProp(cur, (const xmlChar *) "icon");
      xmlChar *pathAttr = xmlGetProp(cur, (const xmlChar *) "path");

      if (titleAttr && pathAttr)
        {
          AppStruct record;
          record.title = (char*) titleAttr;
          record.iconPath = std::string(ILIXI_DATADIR).append((char*) iconAttr);
          record.path = std::string(ILIXI_BINDIR).append((char*) pathAttr);
          _apps.push_back(record);
        }

      xmlFree(titleAttr);
      xmlFree(iconAttr);
      xmlFree(pathAttr);

      cur = cur->next;
    }

  xmlFreeDoc(doc);
}

int
main(int argc, char* argv[])
{
  Home app(argc, argv);
  app.exec();
  return 0;
}
