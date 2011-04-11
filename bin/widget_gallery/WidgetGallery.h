#ifndef TESTWIDGETS_H_
#define TESTWIDGETS_H_

#include "ui/Application.h"
#include "ui/Dialog.h"

using namespace ilixi;

class WidgetGallery : public Application
{
public:
  WidgetGallery(int argc, char* argv[]);

  virtual
  ~WidgetGallery();

private:
  Dialog* dia;
};

#endif /* TESTWIDGETS_H_ */
