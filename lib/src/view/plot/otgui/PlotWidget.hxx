//                                               -*- C++ -*-
/**
 *  @brief QwtPlot for all the graphics of the interface
 *
 *  Copyright 2015-2017 EDF-Phimeca
 *
 *  This library is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  along with this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifndef OTGUI_PLOTWIDGET_HXX
#define OTGUI_PLOTWIDGET_HXX

#include <qwt_plot_curve.h>
#include <qwt_plot.h>
#include <qwt_symbol.h>

#include <openturns/Distribution.hxx>
#include "otgui/OTGuiprivate.hxx"

namespace OTGUI
{
class OTGUI_API PlotWidget : public QwtPlot
{
  Q_OBJECT

public:

  static const QColor DefaultHistogramColor;

  /// constructor
  PlotWidget(const QString plotTypeName = "", const bool isIndicesPlot = false, QWidget * parent = 0);

  /// plot a curve
  void plotCurve(double * x, double * y, int size, const QPen pen = QPen(Qt::black, 2),
                 QwtPlotCurve::CurveStyle style = QwtPlotCurve::Lines, QwtSymbol* symbol = 0, QString title = "");
  void plotCurve(const OT::Sample & data, const QPen pen = QPen(Qt::black, 2),
                 QwtPlotCurve::CurveStyle style = QwtPlotCurve::Lines, QwtSymbol* symbol = 0, QString title = "");

  void plotPDFCurve(const OT::Distribution & distribution, const QPen pen = QPen(Qt::black, 2));
  void plotCDFCurve(const OT::Distribution & distribution, const QPen pen = QPen(Qt::black, 2));
  void plotHistogram(const OT::Sample & sample, const OT::UnsignedInteger graphType = 0, int barNumber = 0, QString title = "");
  void plotScatter(const OT::Sample & input, const OT::Sample & output,
                   QPen pen = QPen(Qt::blue, 4), QString Xtitle = "", QString Ytitle = "");
  void plotBoxPlot(const double median,
                   const double lowerQuartile,
                   const double upperQuartile,
                   const double lowerBound,
                   const double upperBound,
                   const OT::Point& outliers);
  void plotSensitivityIndices(const OT::Point& firstOrderIndices,
                              const OT::Point& totalIndices,
                              const OT::Description& inputNames,
                              const OT::Interval& firstOrderIndicesIntervals = OT::Interval(),
                              const OT::Interval& totalIndicesIntervals = OT::Interval());
  void plotContour(const OT::Distribution& distribution, const bool isPdf = true);

  /// clear plot
  void clear();
  void replot();

  static QVector<PlotWidget*> GetListScatterPlots(const OT::Sample& inS,
      const OT::Sample& notValidInS,
      const OT::Sample& outS,
      const QStringList inNames,
      const QStringList inAxisNames,
      const QStringList outNames,
      const QStringList outAxisNames);

public slots:
  void contextMenu(const QPoint & pos);
  void exportPlot();

signals:
  void plotChanged();
private:
  void updateScaleParameters(const OT::Distribution & distribution);

private:
// TODO  QwtPlotGrid * grid_;
  QString plotTypeName_;
  QAction * exportPlotAction_;
};
}
#endif
