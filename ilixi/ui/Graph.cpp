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

#include "ui/Graph.h"
#include "graphics/Painter.h"
#include <algorithm>
#include "types/FontMetrics.h"

using namespace ilixi;

//******************************************************
// GraphCurve
//******************************************************
GraphCurve::GraphCurve(std::string title, int size, Color penColor) :
  _title(title), _symbolStyle(NoSymbol), _lineStyle(Line), _size(size),
      _values(new double[size]), _curvePen(penColor), _graph(NULL)
{
  for (int i = 0; i < size; ++i)
    _values[i] = 0;
}

GraphCurve::GraphCurve(std::string title, int size, const Pen& pen) :
  _title(title), _symbolStyle(NoSymbol), _lineStyle(Line), _size(size),
      _values(new double[size]), _curvePen(pen), _graph(NULL)
{
  for (int i = 0; i < size; ++i)
    _values[i] = 0;
}

GraphCurve::GraphCurve(std::string title, double* values, int size,
    const Pen& pen) :
  _title(title), _symbolStyle(NoSymbol), _lineStyle(Line), _size(size),
      _values(new double[size]), _curvePen(pen), _graph(NULL)
{
  std::copy(values, values + size, _values);
}

GraphCurve::GraphCurve(const GraphCurve& curve) :
  _title(curve._title), _symbolStyle(curve._symbolStyle), _lineStyle(
      curve._lineStyle), _size(curve._size), _values(new double[curve._size]),
      _curvePen(curve._curvePen)
{
  std::copy(curve._values, curve._values + curve._size, _values);
}

GraphCurve::~GraphCurve()
{
  delete[] _values;
}

void
GraphCurve::attach(Graph* graph)
{
  if (_graph)
    _graph->removeCurve(this);
  _graph = graph;
  _graph->addCurve(this);
}

int
GraphCurve::size() const
{
  return _size;
}

GraphCurve::LineStyle
GraphCurve::lineStyle() const
{
  return _lineStyle;
}

GraphCurve::SymbolStyle
GraphCurve::symbolStyle() const
{
  return _symbolStyle;
}

std::string
GraphCurve::title() const
{
  return _title;
}

double*
GraphCurve::values() const
{
  return _values;
}

Pen*
GraphCurve::pen()
{
  return &_curvePen;
}

void
GraphCurve::push(double value)
{
  memmove(_values, _values + 1, (_size - 1) * sizeof(double));
  _values[_size - 1] = value;
}

void
GraphCurve::resize(int size)
{
  double* temp = new double[size];
  for (int i = 0; i < size; ++i)
    temp[i] = _values[i];
  delete[] _values;
  _values = temp;
  _size = size;
}

void
GraphCurve::setLineStyle(LineStyle lineStyle)
{
  _lineStyle = lineStyle;
}

void
GraphCurve::setSymbolStyle(SymbolStyle symbolStyle)
{
  _symbolStyle = symbolStyle;
}

void
GraphCurve::setTitle(std::string title)
{
  _title = title;
}

void
GraphCurve::setPen(const Pen& pen)
{
  _curvePen = pen;
}

//******************************************************
// GraphArea
//******************************************************
GraphArea::GraphArea(Widget* parent) :
  Widget(parent)
{
  _gridPen = new Pen();
  _gridPen->setColor(Color(1, 1, 1));
  _gridPen->setLineWidth(1);
  double* dashes = new double[2];
  dashes[0] = 1;
  dashes[1] = 1;
  _gridPen->setDash(dashes, 2, 0);
}

GraphArea::~GraphArea()
{
  delete _gridPen;
}

void
GraphArea::setGraph(Graph* graph)
{
  _graph = graph;
}

void
GraphArea::compose(const Rectangle& rect)
{
  Size yLabelSize = Size(40, 20);
  Size xLabelSize = Size(40, 20);
  char labelBuffer[20];

  Painter p(this);
  p.begin(rect);

  int x = yLabelSize.width() + 5;
  int y = yLabelSize.height() / 2;
  int w = width() - xLabelSize.width() / 2 - x;
  int h = height() - xLabelSize.height() - y;
  double x2 = width() - xLabelSize.width() / 2;
  double y2 = height() - xLabelSize.height();

  // positions for vertical grid lines
  double vLines[_graph->_xMaxTicks];
  vLines[0] = x;
  double xTickStep = w / (_graph->_xMaxTicks + .0);
  for (int i = 1; i <= _graph->_xMaxTicks; ++i)
    vLines[i] = vLines[i - 1] + xTickStep;

  // positions for horizontal grid lines
  double hLines[_graph->_yMaxTicks];
  hLines[0] = y;
  double yTickStep = h / (_graph->_yMaxTicks + .0);
  for (int i = 1; i <= _graph->_yMaxTicks; ++i)
    hLines[i] = hLines[i - 1] + yTickStep;

  // draw background
  p.getBrush()->setColor(Color(1, 1, 1, 0.2));
  p.drawRectangle(x, y, w, h, Painter::FillPath);

  // draw grid
  p.setPen(_gridPen);
  p.setAntiAliasMode(Painter::AliasNone);

  for (int i = 0; i <= _graph->_xMaxTicks; ++i)
    p.drawLine(vLines[i], y, vLines[i], y2, Painter::StrokePath);

  for (int i = 0; i <= _graph->_yMaxTicks; ++i)
    p.drawLine(x, hLines[i], x2, hLines[i], Painter::StrokePath);

  // draw labels x-axis tick labels
  p.getBrush()->setColor(Color(1, 1, 1, 0.5));
  p.setAntiAliasMode(Painter::AliasSubPixel);
  p.setLayoutAlignment(PANGO_ALIGN_CENTER);
  double xLabelOffset = xTickStep / 2.0;
  int step = _graph->_windowSize / _graph->_xMaxTicks;
  for (int i = 0; i <= _graph->_xMaxTicks; i++)
    {
      sprintf(labelBuffer, "%d", _graph->_windowSize - (i * step));
      p.drawText(labelBuffer, vLines[i] - xLabelOffset, y2 + 5, xTickStep, 1);
    }

  // draw labels y-axis tick labels
  int yStep = 100 / _graph->_yMaxTicks;
  p.setLayoutAlignment(PANGO_ALIGN_RIGHT);
  for (int i = 0; i <= _graph->_yMaxTicks; ++i)
    {
      sprintf(labelBuffer, "%d %%", 100 - (i * yStep));
      p.drawText(labelBuffer, 0, hLines[i] - 5, yLabelSize.width(), 1);
    }

  // draw plot data

  _yScale = h / (_graph->_yMax - _graph->_yMin + 0.0);
  double xOffset = 0;
  double cXP = 0;
  double cXC = 0;
  double cYP = 0;
  double cYC = 0;
  cairo_t* context = p.cairoContext();
  for (int curve = 0; curve < _graph->_curves.size(); ++curve)
    {
      p.setPen(_graph->_curves[curve]->pen());

      xOffset = w / (_graph->_curves[curve]->size() + .0);
      cXP = x;
      cXC = cXP + xOffset;
      cYP = y2 - (_graph->_curves[curve]->values()[0] * _yScale);
      cYC = y2 - (_graph->_curves[curve]->values()[1] * _yScale);
      if (_graph->_curves[curve]->lineStyle() == GraphCurve::Line)
        p .drawLine(cXP, cYP, cXC, cYC);
      else if (_graph->_curves[curve]->lineStyle() == GraphCurve::Step)
        {
          p .drawLine(cXP, cYP, cXP, cYC);
          p .drawLine(cXP, cYC, cXC, cYC);
        }
      else if (_graph->_curves[curve]->lineStyle() == GraphCurve::Bar)
        {
          p.setAntiAliasMode(Painter::AliasNone);
          p.getBrush()->setColor(_graph->_curves[curve]->pen()->getColor());
          p.drawRectangle(cXP, cYC, xOffset, y2 - cYC, Painter::FillPath);
        }
      else if (_graph->_curves[curve]->lineStyle() == GraphCurve::LineArea)
        {
          cairo_line_to(context, cXP, cYP);
        }

      cXP = cXC;
      cYP = cYC;
      for (int point = 1; point < _graph->_curves[curve]->size(); ++point)
        {
          cXC = cXP + xOffset;
          cYC = y2 - (_graph->_curves[curve]->values()[point] * _yScale);

          if (_graph->_curves[curve]->lineStyle() == GraphCurve::Line)
            p.drawLine(cXP, cYP, cXC, cYC);

          else if (_graph->_curves[curve]->lineStyle() == GraphCurve::Step)
            {
              p.drawLine(cXP, cYP, cXP, cYC);
              p.drawLine(cXP, cYC, cXC, cYC);
            }

          else if (_graph->_curves[curve]->lineStyle() == GraphCurve::Bar)
            p.drawRectangle(cXP, cYC, xOffset, y2 - cYC, Painter::FillPath);

          else if (_graph->_curves[curve]->lineStyle() == GraphCurve::Stick)
            {
              //              p.getPen()->setLineWidth(
              //                  _graph->_curves[curve]->pen()->getLineWidth());
              p.drawLine(cXC, cYP, cXC, cYC);
              //              p.getPen()->setLineWidth(1);
              p.drawEllipse(cXC - 2.5, cYC - 2.5, 5, 5, Painter::StrokeAndFill);
            }

          else if (_graph->_curves[curve]->lineStyle() == GraphCurve::LineArea)
            {
              cairo_line_to(context, cXC, cYC);
            }

          cXP = cXC;
          cYP = cYC;
        } // end for points

      if (_graph->_curves[curve]->lineStyle() == GraphCurve::LineArea)
        {

          cairo_line_to(context, cXP, cYP);
          cairo_line_to(context, cXP, y2);
          cairo_line_to(context, x, y2);
          cairo_line_to(context, x, y2 - (_graph->_curves[curve]->values()[0]
              * _yScale));
//          cairo_close_path(context);
          cairo_fill(context);
        }

    } // end for curve
  p.end();
}

//******************************************************
// GraphLegend
//******************************************************
GraphLegend::GraphLegend(Widget* parent) :
  Widget(parent)
{
}

GraphLegend::~GraphLegend()
{
}

Size
GraphLegend::preferredSize() const
{
  if (_graph->_curves.size() == 0)
    return Size();

  Size ts;
  int w = 0;
  int h = 0;
  for (int i = 0; i < _graph->_curves.size(); i++)
    {
      ts = FontMetrics::getSize(designer()->defaultFont(DefaultFont),
          _graph->_curves[i]->title(), true);
      if (ts.width() > w)
        w = ts.width();
      h += ts.height();
    }
  return Size(w + 15, h);
}

void
GraphLegend::setGraph(Graph* graph)
{
  _graph = graph;
}

void
GraphLegend::compose(const Rectangle& rect)
{
  if (_graph->_curves.size() == 0)
    return;

  Painter p(this);
  p.begin(rect);

  int h = FontMetrics::getSize(designer()->defaultFont(DefaultFont),
      _graph->_curves[0]->title(), true).height();
  for (int i = 0; i < _graph->_curves.size(); i++)
    {
      p.getBrush()->setColor(_graph->_curves[i]->pen()->getColor());
      p.drawRectangle(0, i * h, 10, 10, Painter::FillPath);
      p.getBrush()->setColor(Color(1, 1, 1));
      p.drawText(_graph->_curves[i]->title(), 15, i * h);
    }
  p.end();
}

//******************************************************
// Graph
//******************************************************
Graph::Graph(std::string title, int windowSize, Widget* parent) :
  Widget(parent), _autoScale(false), _showLegend(false), _xMaxTicks(5),
      _yMaxTicks(5), _windowSize(windowSize)
{
  setConstraints(MinimumConstraint, MinimumExpandingConstraint);

  _title = new Label(title);
  _title->setHorizontalAlignment(PANGO_ALIGN_CENTER);
  addChild(_title);

  _yTitle = new Label("");
  _yTitle->setHorizontalAlignment(PANGO_ALIGN_CENTER);
  addChild(_yTitle);
  //  _yTitle->setGravity(PANGO_GRAVITY_EAST);

  _xTitle = new Label("");
  _xTitle->setHorizontalAlignment(PANGO_ALIGN_CENTER);
  addChild(_xTitle);

  _legend = new GraphLegend();
  addChild(_legend);
  _legend->setGraph(this);

  _graphArea = new GraphArea();
  addChild(_graphArea);
  _graphArea->setGraph(this);

  sigGeometryUpdated.connect(sigc::mem_fun(this, &Graph::updateGraphGeometry));

  _xMaxTicks = 10;
  _yMaxTicks = 5;

  _yMin = 0;
  _yMax = 100;
}

//Graph::Graph(const Graph& graph)
////:  Widget(graph)
//{
//}

Graph::~Graph()
{
  for (int i = 0; i < _curves.size(); ++i)
    delete _curves[i];
  _curves.clear();
}

Size
Graph::preferredSize() const
{
  return Size(200, 100);
}

void
Graph::appendValue(int curve, double value)
{
  _curves.at(curve)->push(value);
}

void
Graph::addCurve(GraphCurve* curve)
{
  _curves.push_back(curve);
}

void
Graph::removeCurve(GraphCurve* curve)
{

}

void
Graph::clear()
{
  for (int i = 0; i < _curves.size(); ++i)
    delete _curves[i];
  _curves.clear();
}

void
Graph::replot()
{
  if (_autoScale)
    scale();
  _graphArea->update();
}

bool
Graph::autoScale() const
{
  return _autoScale;
}

std::string
Graph::axisTitle(Axis axis) const
{
  if (axis == yAxis)
    return _yTitle->text();
  return _xTitle->text();
}

bool
Graph::legendShown() const
{
  return _showLegend;
}

std::string
Graph::title() const
{
  return _title->text();
}

void
Graph::setAutoScale(bool autoScale)
{
  _autoScale = autoScale;
}

void
Graph::setAxisTitle(Axis axis, std::string title)
{
  if (axis == yAxis)
    _yTitle->setText(title);
  _xTitle->setText(title);
}

void
Graph::setLegendShown(bool shown)
{
  _showLegend = shown;
}

void
Graph::setTitle(std::string title)
{
  _title->setText(title);
}

void
Graph::setXLabels(const std::vector<std::string>& xLabels)
{
  _xLabels = xLabels;
}

void
Graph::compose(const Rectangle& rect)
{
  //  Painter p(this);
  //  p.begin(rect);
  //  p.end();
}

void
Graph::scale()
{
  //  _yMin = INT_MAX;
  //  _yMax = 0;
  //  int curveSize = 0;
  //  for (int i = 0; i < _curves.size(); ++i)
  //    {
  //
  //      if (windowSize < _size)
  //        windowSize = _size - windowSize;
  //      else
  //        windowSize = _size;
  //
  //      for (int i = curveSize; i > 0; --i)
  //        {
  //          if (_values[i] < min)
  //            min = _values[i];
  //          if (_values[i] > max)
  //            max = _values[i];
  //        }
  //    }
}

void
Graph::updateGraphGeometry()
{
  Size titleSize = _title->preferredSize();
  Size yTitleSize = _yTitle->preferredSize();
  Size xTitleSize = _xTitle->preferredSize();
  Size legendSize = _legend->preferredSize();

  _title->setGeometry(0, 0, width(), _title->textExtents().height());

  _yTitle->setGeometry(0, _title->height(), yTitleSize.width(),
      yTitleSize.height());

  _xTitle->setGeometry(0, height() - xTitleSize.height(), width(),
      _title->textExtents().height());

  _legend->setGeometry(width() - legendSize.width(), (height()
      - legendSize.height()) / 2, legendSize.width(), legendSize.height());

  _graphArea->setGeometry(_yTitle->width(), _title->height(), width()
      - (_yTitle->width() + _legend->width()), height() - (_title->height()
      + _xTitle->height()));

  //  _graphArea->setGeometry(_yTitle->width(), _title->height(), width()
  //      - _yTitle->width(), height() - (_title->height() + _xTitle->height()
  //      + _title->height()));
}
