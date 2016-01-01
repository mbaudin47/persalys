// PlotWidget.hxx


#ifndef PLOTWIDGET_H
#define PLOTWIDGET_H

#include <qwt_plot_curve.h>
#include <qwt_plot.h>
#include <qwt_plot_grid.h>
#include <qwt_symbol.h>
#include <qwt_scale_draw.h>

#include "Distribution.hxx"

namespace OTGUI {
class PlotWidget : public QwtPlot
{
  Q_OBJECT

public:

  static const QColor DefaultHistogramColor;

  /// constructor
  PlotWidget(bool isIndicesPlot = false, QWidget * parent = 0);

  /// plot a curve
  void plotCurve(double * x, double * y, int size, const QPen pen=QPen(Qt::black, 2),
                 QwtPlotCurve::CurveStyle style=QwtPlotCurve::Lines, QwtSymbol* symbol=0, QString title="");
  void plotCurve(const OT::NumericalSample & data, const QPen pen=QPen(Qt::black, 2));

  void plotPDFCurve(const OT::Distribution & distribution, const QPen pen=QPen(Qt::black, 2));
  void plotCDFCurve(const OT::Distribution & distribution, const QPen pen=QPen(Qt::black, 2));
  void plotHistogram(const OT::NumericalSample & sample, bool cdf=false, int barNumber=0);
  void plotScatter(const OT::NumericalSample & input, const OT::NumericalSample & output);
  void plotBoxPlot(double median, double lowerQuartile, double upperQuartile,
                   double lowerBound, double upperBound, OT::NumericalPoint outliers_);
  void plotSensitivityIndices(const OT::NumericalPoint firstOrder, const OT::NumericalPoint totalOrder,
                              const OT::Description inputNames);

  /// clear plot
  void clear();
  void replot();

public slots:
  void contextMenu(const QPoint & pos);
  void exportPlot();
signals:
  void plotChanged();

private:
  void updateScaleParameters(const OT::Distribution & distribution);

private:
  QwtPlotGrid * grid_;
  QAction * exportPlotAction_;
};
}
#endif
