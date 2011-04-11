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

#include "Monitor.h"
#include "ui/VBoxLayout.h"
#include "ui/HBoxLayout.h"
#include "ui/GridLayout.h"
#include "ui/GroupBox.h"
#include "ui/TabPanel.h"
#include "ui/Icon.h"
#include "ui/Graph.h"
#include "ui/ScrollArea.h"
#include <sstream>
#include "ilixiConfig.h"

template<class T>
  inline std::string
  toString(const T& t)
  {
    std::stringstream ss;
    ss << t;
    return ss.str();
  }

MonitorThread::MonitorThread(Monitor* parent) :
  Thread(), _parent(parent)
{

}

MonitorThread::~MonitorThread()
{

}

int
MonitorThread::run()
{
  CPUMonitor* cpuMon = _parent->_cpuMon;
  FSMonitor* fsMon = _parent->_fsMon;
  MEMMonitor* memMon = _parent->_memMon;
  NETMonitor* netMon = _parent->_netMon;
  OSMonitor* osMon = _parent->_osMon;
  while (1)
    {
      pthread_testcancel();
      cpuMon->refresh();
      fsMon->refresh();
      memMon->refresh();
      netMon->refresh();
      osMon->refresh();
      _parent->_uptime->setMarkup("<b>Uptime:</b> " + osMon->getUptime());
      _parent->_fsUsage->setValue(fsMon->getUsage());
      _parent->_memUsage->setValue(memMon->getUsage());
      _parent->_netRXC->setText(netMon->getReceiving());
      _parent->_netTXC->setText(netMon->getTransmitting());
      _parent->_netRXT->setText(netMon->getTotalReceived());
      _parent->_netTXT->setText(netMon->getTotalTransmitted());

      _parent->_cpuUsa->push(cpuMon->getCpu(0).getUsage());
      //      _parent->_cpuIdl->push(cpuMon->getCpu(0).getIdle());
      _parent->_cpuUse->push(cpuMon->getCpu(0).getUser());
      _parent->_cpuSys->push(cpuMon->getCpu(0).getSystem());
      _parent->_netGraph->replot();
      sleep(1);
    }
}

Monitor::Monitor(int argc, char* argv[]) :
  Application(argc, argv)
{
  setTitle("System Monitor");
  _cpuMon = new CPUMonitor();
  _cpuMon->refresh();

  _fsMon = new FSMonitor();
  _fsMon->refresh();

  _memMon = new MEMMonitor();
  _memMon->refresh();

  _netMon = new NETMonitor();
  _netMon->refresh();

  _osMon = new OSMonitor();
  _osMon->refresh();

  setLayout(new VBoxLayout());
  TabPanel* tabPanel = new TabPanel();
  addWidget(tabPanel);

  //**************************************
  VBoxLayout* overviewLayout = new VBoxLayout();
  tabPanel->addPage(overviewLayout, "Overview");

  _uptime = new Label("<b>Uptime:</b> " + _osMon->getUptime());
  overviewLayout->addWidget(_uptime);
  overviewLayout->addWidget(new Label("<b>System:</b> " + _osMon->getSysName()));
  overviewLayout->addWidget(new Label("<b>Node:</b> " + _osMon->getNodeName()));
  overviewLayout->addWidget(
      new Label("<b>Release:</b> " + _osMon->getRelease()));
  overviewLayout->addWidget(
      new Label("<b>Version:</b> " + _osMon->getVersion()));
  overviewLayout->addWidget(
      new Label("<b>Machine:</b> " + _osMon->getMachine()));

  overviewLayout->addWidget(new Label("<b>CPU Model:</b> "
      + _cpuMon->getModel()));
  overviewLayout->addWidget(new Label("<b>CPU Cache:</b> " + toString(
      _cpuMon->getCacheSize()) + " KB"));

  overviewLayout->addWidget(new Spacer(Vertical));

  //**************************************

  VBoxLayout* resLayout = new VBoxLayout();
  tabPanel->addPage(resLayout, "CPU");

  _netGraph = new Graph("", 60);
  //  netGraph->setXLabels();
  _cpuUsa = new GraphCurve("Usage", 60, Color(.74, .18, .18));
  _cpuUsa->setLineStyle(GraphCurve::LineArea);
  _cpuUsa->attach(_netGraph);

  //  _cpuIdl = new GraphCurve("Idle", 60, Color(0, 1, 0));
  //  _cpuIdl->setLineStyle(GraphCurve::Line);
  //  _cpuIdl->attach(_netGraph);

  _cpuUse = new GraphCurve("User", 60, Color(.16, .27, .5));
  _cpuUse->setLineStyle(GraphCurve::LineArea);
  _cpuUse->attach(_netGraph);

  _cpuSys = new GraphCurve("System", 60, Color(.73, .74, .18));
  _cpuSys->setLineStyle(GraphCurve::LineArea);
  _cpuSys->attach(_netGraph);

  resLayout->addWidget(_netGraph);

  //**************************************
  HBoxLayout* fm = new HBoxLayout();
  tabPanel->addPage(fm, "Resources");

  GroupBox* memGroup = new GroupBox("Memory");
  memGroup->setLayout(new VBoxLayout());
  fm->addWidget(memGroup);

  memGroup->addWidget(new Label("<b>Total:</b> " + toString(
      _memMon->getTotal()) + " MB"));
  memGroup->addWidget(new Label("<b>Cached:</b> " + toString(
      _memMon->getCached()) + " MB"));
  memGroup->addWidget(new Label("<b>Free:</b> " + toString(
      _memMon->getFree()) + " MB"));
  memGroup->addWidget(new Label("<b>Buffers:</b> " + toString(
      _memMon->getBuffers()) + " MB"));
  memGroup->addWidget(new Spacer(Vertical));

  memGroup->addWidget(new Label("<b>Usage:</b>"));
  _memUsage = new ProgressBar();
  memGroup->addWidget(_memUsage);

  GroupBox* fsGroup = new GroupBox("FileSystem");
  fsGroup->setLayout(new VBoxLayout());
  fm->addWidget(fsGroup);

  fsGroup->addWidget(new Label("<b>Total:</b> " + toString(
      _fsMon->getTotal()) + " GB"));
  fsGroup->addWidget(new Label("<b>Free:</b> " + toString(
      _fsMon->getFree()) + " GB"));
  fsGroup->addWidget(new Spacer(Vertical));

  fsGroup->addWidget(new Label("<b>Usage:</b>"));
  _fsUsage = new ProgressBar();
  fsGroup->addWidget(_fsUsage);

  //**************************************
  GridLayout* netGroupLayout = new GridLayout(2, 6);
  tabPanel->addPage(netGroupLayout, "Network");

  netGroupLayout->addWidget(new Icon(ILIXI_DATADIR"download.png"), 0, 0, -1, 1);
  netGroupLayout->addWidget(new Label("<b>Receiving:</b>"));
  _netRXC = new Label(_netMon->getReceiving());
  netGroupLayout->addWidget(_netRXC);

  netGroupLayout->addWidget(new Icon(ILIXI_DATADIR"upload.png"), 0, 3, -1, 1);
  netGroupLayout->addWidget(new Label("<b>Sending:</b>"));
  _netTXC = new Label(_netMon->getTransmitting());
  netGroupLayout->addWidget(_netTXC);

  netGroupLayout->addWidget(new Label("<b>Total Received:</b>"));
  _netRXT = new Label(_netMon->getTotalReceived());
  netGroupLayout->addWidget(_netRXT);

  netGroupLayout->addWidget(new Label("<b>Total Sent:</b>"));
  _netTXT = new Label(_netMon->getTotalTransmitted());
  netGroupLayout->addWidget(_netTXT);
  //**************************************


  _thread = new MonitorThread(this);
}

Monitor::~Monitor()
{
  _thread->cancel();
  delete _cpuMon;
  delete _fsMon;
  delete _memMon;
  delete _netMon;
  delete _osMon;
}

void
Monitor::onShow()
{
  _thread->start();
}

void
Monitor::onHide()
{
  _thread->cancel();
}

int
main(int argc, char* argv[])
{
  Monitor app(argc, argv);
  app.exec();
  return 0;
}
