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

#ifndef ILIXI_GRAPH_H_
#define ILIXI_GRAPH_H_

#include "ui/Frame.h"
#include "ui/Label.h"
#include "types/Pen.h"
#include <vector>

namespace ilixi
{

  class Graph;

  class GraphCurve
  {
  public:

    enum SymbolStyle
    {
      NoSymbol, Circle, Cross
    };

    enum LineStyle
    {
      NoLine, Line, Bezier, Step, Bar, Stick, LineArea
    };

    GraphCurve(std::string title, int size, Color penColor);

    GraphCurve(std::string title, int size, const Pen& pen);

    GraphCurve(std::string title, double* values, int size, const Pen& pen);

    GraphCurve(const GraphCurve& curve);

    virtual
    ~GraphCurve();

    void
    attach(Graph* graph);

    int
    size() const;

    LineStyle
    lineStyle() const;

    SymbolStyle
    symbolStyle() const;

    std::string
    title() const;

    double*
    values() const;

    Pen*
    pen();

    void
    push(double value);

    void
    resize(int size);

    void
    setLineStyle(LineStyle lineStyle);

    void
    setSymbolStyle(SymbolStyle symbolStyle);

    void
    setTitle(std::string title);

    void
    setPen(const Pen& pen);

  private:
    std::string _title;
    SymbolStyle _symbolStyle;
    LineStyle _lineStyle;
    int _size;
    double* _values;
    Pen _curvePen;
    Graph* _graph;
  };

  class GraphArea : public Widget
  {
  public:
    GraphArea(Widget* parent = 0);

    virtual
    ~GraphArea();

    void
    setGraph(Graph* graph);

  protected:
    virtual void
    compose(const Rectangle& rect);

    double _yScale;
    Pen* _gridPen;
    Graph* _graph;
  };

  class GraphLegend : public Widget
  {
    friend class Graph;
  public:
    GraphLegend(Widget* parent = 0);

    virtual
    ~GraphLegend();

    virtual Size
    preferredSize() const;

    void
    setGraph(Graph* graph);

  protected:
    Graph* _graph;
    virtual void
    compose(const Rectangle& rect);
  };

  class Graph : public Widget
  {
    friend class GraphCurve;
    friend class GraphArea;
    friend class GraphLegend;
  public:
    enum Axis
    {
      yAxis, xAxis
    };

    Graph(std::string title, int windowSize, Widget* parent = 0);

    //    Graph(const Graph& graph);

    virtual
    ~Graph();

    virtual Size
    preferredSize() const;

    void
    appendValue(int curve, double value);

    void
    addCurve(GraphCurve* curve);

    void
    removeCurve(GraphCurve* curve);

    void
    clear();

    void
    replot();

    bool
    autoScale() const;

    std::string
    axisTitle(Axis axis) const;

    bool
    legendShown() const;

    std::string
    title() const;

    void
    setAutoScale(bool autoScale);

    void
    setAxisTitle(Axis axis, std::string title);

    void
    setLegendShown(bool shown);

    void
    setTitle(std::string title);

    void
    setXLabels(const std::vector<std::string>& xLabels);

  protected:
    virtual void
    compose(const Rectangle& rect);

  private:
    //! If true graph area is updated when the min. and max. values changes.
    bool _autoScale;
    //! If true a legend is shown beside graph.
    bool _showLegend;
    //! Number of maximum ticks on x-axis.
    unsigned int _xMaxTicks;
    //! Number of maximum ticks on y-axis.
    unsigned int _yMaxTicks;
    //! Number of points visible on graph.
    int _windowSize;
    //! Minimum value on y-axis.
    double _yMin;
    //! Maximum value on y-axis.
    double _yMax;

    //! Title of graph.
    Label* _title;
    //! Title for y-axis.
    Label* _yTitle;
    //! Title for x-axis.
    Label* _xTitle;
    //! Legend.
    GraphLegend* _legend;
    //! Curves are drawn onto this widget.
    GraphArea* _graphArea;

    std::string _yLabelFormat;
    //! Labels on x-axis.
    std::vector<std::string> _xLabels;

    std::vector<GraphCurve*> _curves;

    void
    scale();

    void
    updateGraphGeometry();
  };

}

#endif /* ILIXI_GRAPH_H_ */
