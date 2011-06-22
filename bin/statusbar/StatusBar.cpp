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

#include "StatusBar.h"
#include "ui/HBoxLayout.h"
#include "ui/VBoxLayout.h"
#include "ui/Line.h"
#include "graphics/Painter.h"
#include "types/LinearGradient.h"
#include <sigc++/sigc++.h>
#include "core/Logger.h"

const char* days[7] =
  { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };

const char* months[12] =
  { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct",
      "Nov", "Dec" };

DateThread::DateThread(StatusBar* parent) :
  Thread(), _parent(parent)
{
}

DateThread::~DateThread()
{
}

int
DateThread::run()
{
  ILOG_DEBUG("Date thread is running...");
  while (1)
    {
      pthread_testcancel();
      _parent->updateTime();
      sleep(1);
    }
  return 0;
}

//*********************************************************

StatusBar::StatusBar(int argc, char* argv[]) :
  Application(argc, argv), _app(0), _time(0), _shutDown(0)
{
  setTitle("StatusBar");
  setBackgroundFilled(true);
  initDFBWindow(0, 0, 0, getStatusBarHeight());

  setLayout(new HBoxLayout());
  setMargins(3, 3, 5, 5);

  _home = new ToolButton("Home");
  _home->setInputMethod(PointerInputOnly);
  _home->setToolButtonStyle(ToolButton::IconOnly);
  _home->setIcon(ILIXI_DATADIR"home.png", Size(32, 32));
  addWidget(_home);

  addWidget(new Line(Line::Vertical));

  _app = new Label("ilixi UI");
  _app->setWeight(PANGO_WEIGHT_BOLD);
  _app->setFontSize(13);
  _app->setHorizontalAlignment(PANGO_ALIGN_CENTER);
  _app->setHConstraint(ExpandingConstraint);
  addWidget(_app);

  addWidget(new Line(Line::Vertical));

  _time = new Label("00:00:00");
  _time->setHorizontalAlignment(PANGO_ALIGN_CENTER);
  _time->setFontSize(11);
  //  _time->setMinimumSize(150, 20);

  _date = new Label("Mon 001 Jan");
  _date->setHorizontalAlignment(PANGO_ALIGN_CENTER);

  Frame* dateBox = new Frame();
  dateBox->setBorderStyle(NoBorder);
  dateBox->setSpacing(0);
  dateBox->setMargin(0);
  dateBox->setLayout(new VBoxLayout());
  dateBox->addWidget(_time);
  dateBox->addWidget(_date);
  addWidget(dateBox);

  addWidget(new Line(Line::Vertical));

  _shutDown = new ToolButton("STANDBY");
  _shutDown->setToolButtonStyle(ToolButton::IconOnly);
  _shutDown->setInputMethod(PointerInputOnly);
  _shutDown->setIcon(ILIXI_DATADIR"shutdown.png", Size(32, 32));
  addWidget(_shutDown);

  _sDialog = new SDialog("Choose an action", this);

  dateThread = new DateThread(this);

  _home->sigClicked.connect(sigc::mem_fun(this, &StatusBar::showHome));
  _shutDown->sigClicked.connect(sigc::mem_fun(_sDialog, &SDialog::execute));
  _sDialog->sigFinished.connect(sigc::mem_fun(this, &StatusBar::handleDialog));
}

StatusBar::~StatusBar()
{
  delete _sDialog;
  delete dateThread;
}

void
StatusBar::onHide()
{
  dateThread->cancel();
}

void
StatusBar::onShow()
{
  dateThread->start();
}

void
StatusBar::updateTime()
{
  struct timeval tv;
  struct tm* tm;
  gettimeofday(&tv, NULL);
  tm = localtime(&tv.tv_sec);

  char time[9];
  sprintf(time, "%02d:%02d:%02d", tm->tm_hour, tm->tm_min, tm->tm_sec);
  _time->setText(time);

  char date[11];
  sprintf(date, "%s %d %s", days[tm->tm_wday], tm->tm_mday, months[tm->tm_mon]);
  if (strcmp(date, _date->text().c_str()))
    _date->setText(date);
}

ReactionResult
StatusBar::reactorCB(ReactorMessage *msg, void *ctx)
{
  if (msg->senderFusionID != 1)
    {
      ILOG_ERROR("Sender is not authorised!");
      return RS_REMOVE;
    }

  switch (msg->type)
    {
  case SwitchMode:
    ILOG_INFO("Received SwitchMode message from Maestro.");
    if (msg->mode == Visible)
      {
        show();
        return RS_OK;
      }
    else if (msg->mode == Hidden)
      {
        hide();
        return RS_OK;
      }
    else if (msg->mode == Terminated)
      {
        quit();
        return RS_OK;
      }
    else
      ILOG_ERROR("Message mode is not supported!");
    break;

  case Notification:

    if (msg->mode == Visible)
      {
        AppRecord* app = getAppRecord(msg->appID);
        _currentAppID = msg->appID;
        _app->setText((std::string) app->title);
        ILOG_INFO("%s (ID: %d) is visible.", app->title, app->fusionID);

        if (_currentAppID == 4) // Notification comes from Home app.

          {
            _home->setDisabled();
            _sDialog->setKillVisibility(false);
          }
        else
          {
            _home->setEnabled();
            _sDialog->setKillVisibility(true);
            _sDialog->setAppName((std::string) app->title);
          }
      }
    break;

  default:
    ILOG_ERROR("Message type is not supported!");
    }
  return RS_DROP;
}

void
StatusBar::compose(const Rectangle& rect)
{
  Painter painter(this);
  painter.begin(rect);
  LinearGradient background(0, 0, 0, height());
  background.addStop(.5, .5, .5, 1, 0);
  background.addStop(.3, .3, .3, 1, 0.5);
  painter.getBrush()->setGradient(background);
  painter.drawRectangle(rect, Painter::FillPath);
  painter.getPen()->setColor(Color(0, 0, 0));
  painter.drawLine(0, height(), width(), height());
  painter.end();
}

void
StatusBar::handleDialog(int result)
{
  if (result == 0)
    return;
  else if (result == 2)
    {
      _sDialog->setKillVisibility(false);
      callMaestro(SwitchMode, Terminated, _currentAppID);
      return;
    }
  else if (result == 3)
    return;
  else if (result == 4)
    return;
  else if (result == 5)
    quit();
}

void
StatusBar::showHome()
{
  ILOG_DEBUG("Show home");
  callMaestro(SwitchMode, Visible, 4);
}

int
main(int argc, char* argv[])
{
  StatusBar app(argc, argv);
  app.exec();
  return 0;
}
