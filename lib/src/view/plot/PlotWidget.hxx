// PlotWidget.hxx


#ifndef PLOTWIDGET_HXX
#define PLOTWIDGET_HXX

#include "OtguiPlotWidget.hxx"

#include <qwt_plot_curve.h>
#include <qwt_plot.h>
#include <qwt_plot_grid.h>
#include <qwt_symbol.h>

#include "Distribution.hxx"

namespace OTGUI {

class PlotWidget : public OtguiPlotWidget
{
  Q_OBJECT

public:

  static const QColor DefaultHistogramColor;

  /// constructor
  PlotWidget(QWidget * parent = 0);

  /// plot a curve
  void plotCurve(double * x, double * y, int size, const QPen pen=QPen(Qt::black, 2),
                 QwtPlotCurve::CurveStyle style=QwtPlotCurve::Lines, QwtSymbol* symbol=0);
  void plotCurve(const OT::NumericalSample & data, const QPen pen=QPen(Qt::black, 2));

  void plotPDFCurve(const OT::Distribution & distribution, const QPen pen=QPen(Qt::black, 2));
  void plotCDFCurve(const OT::Distribution & distribution, const QPen pen=QPen(Qt::black, 2));
  void plotHistogram(const OT::NumericalSample & sample, bool cdf=false, int barNumber=0);
  void plotScatter(const OT::NumericalSample & input, const OT::NumericalSample & output);
  void drawBoxPlot(double median, double lowerQuartile, double upperQuartile,
                   double lowerBound, double upperBound, OT::NumericalPoint outliers_);

private:
  void updateScaleParameters(const OT::Distribution & distribution);

};
}
#endif
