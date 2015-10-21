// PlotWidget.hxx


#ifndef PLOTWIDGET_HXX
#define PLOTWIDGET_HXX

#include <qwt_plot_curve.h>
#include <qwt_plot.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_zoomer.h>
#include <qwt_symbol.h>
#include <QLabel>

#include "NumericalSample.hxx"
#include "Distribution.hxx"


namespace OTGUI {

class ImageEditionDialog;

class PlotWidget : public QwtPlot
{
  Q_OBJECT

public:

  static const QPen DefaultCurvePen;

  /// constructor
  PlotWidget(QWidget * parent = 0);

  /// plot a curve
  void plotCurve(double * x, double * y, int size, const QPen pen=DefaultCurvePen, QwtPlotCurve::CurveStyle style=QwtPlotCurve::Lines, QwtSymbol* symbol=0);
  void plotCurve(const OT::NumericalSample & data, const QPen pen=DefaultCurvePen);

  void plotPDFCurve(const OT::Distribution & distribution, const QPen pen=DefaultCurvePen);
  void plotScatter(const OT::NumericalSample & input, const OT::NumericalSample & output);
  void drawBoxPlot(double median, double lowerQuartile, double upperQuartile,
   double lowerBound, double upperBound, OT::NumericalPoint outliers_);

  /// clear plot
  void clear();
  void replot();
  void updatePlotLabel();
  QLabel * getPlotLabel() const;

public slots:
  void contextMenu(const QPoint & pos);
  void editImage();

protected:
  QLabel * plotLabel_;
  ImageEditionDialog * dialog_;
  /// the grid
  QwtPlotGrid * grid_;
  QwtPlotZoomer * zoomer_;
  QAction * copyImageAction_;
};
}
#endif
