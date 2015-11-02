#include "PlotWidget.hxx"
#include "DistributionScaleEngine.hxx"
#include <qwt_plot_panner.h>
#include <qwt_plot_histogram.h>
#include <qwt_plot_magnifier.h>
#include <qwt_plot_shapeitem.h>
#include <qpainterpath.h>
#include <qwt_legend.h>
#include <QHBoxLayout>

using namespace OT;

namespace OTGUI {

const QColor PlotWidget::DefaultHistogramColor = QColor(127, 172, 210);

PlotWidget::PlotWidget(QWidget * parent)
: OtguiPlotWidget(parent)
{
  // panning with the left mouse button
  ( void ) new QwtPlotPanner(canvas());

  // zoom in/out with the wheel
  ( void ) new QwtPlotMagnifier(canvas());
}


void PlotWidget::plotCurve(double * x, double * y, int size, const QPen pen, QwtPlotCurve::CurveStyle style, QwtSymbol* symbol)
{
  QwtPlotCurve * curve = new QwtPlotCurve;
  curve->setSamples(x, y, size);
  curve->setPen(pen);
  curve->setStyle(style);
  if (symbol)
    curve->setSymbol(symbol);
  curve->attach(this);
  replot();
}


void PlotWidget::plotCurve(const NumericalSample & data, const QPen pen)
{
  const int size = data.getSize();
  double * x = new double[size];
  double * y = new double[size];

  for (int i = 0; i < size; ++ i)
  {
    x[i] = data[i][0];
    y[i] = data[i][1];
  }
  plotCurve(x, y, size, pen);
  delete[] x;
  delete[] y;
}


void PlotWidget::plotScatter(const NumericalSample & input, const NumericalSample & output)
{
  int size = input.getSize();

  if (output.getSize()!=size)
    return;
  
  double *xData = new double[size];
  double *yData = new double[size];

  for (int i = 0 ; i < size ; ++i)
  {
    xData[i] = input[i][0];
    yData[i] = output[i][0];
    //qDebug() << "x= " << xData[i] << " , y= " << yData[i];
  }

  plotCurve(xData, yData, size, QPen(Qt::GlobalColor(16), 4), QwtPlotCurve::Dots);
}


void PlotWidget::plotPDFCurve(const Distribution & distribution, const QPen pen)
{
  updateScaleParameters(distribution);

  setTitle(tr("PDF"));
  setAxisTitle(QwtPlot::yLeft, tr("Density"));
  setAxisTitle(QwtPlot::xBottom, tr("X"));

  plotCurve(distribution.drawPDF().getDrawable(0).getData(), pen);
}


void PlotWidget::plotCDFCurve(const Distribution & distribution, const QPen pen)
{
  updateScaleParameters(distribution);

  setTitle(tr("CDF"));
  setAxisTitle(QwtPlot::yLeft, tr("CDF"));
  setAxisTitle(QwtPlot::xBottom, tr("X"));

  plotCurve(distribution.drawCDF().getDrawable(0).getData(), pen);
}


void PlotWidget::plotHistogram(const NumericalSample & sample, bool cdf, int barNumber)
{
  const int size = (int)sample.getSize();

  // compute bar number
  if (barNumber <= 0)
    barNumber = ceil(1.0 + log(std::max(size, 1)) / log(2));

  // compute data

  double width = (sample.getMax()[0] - sample.getMin()[0]) / barNumber;
  if (width < 1e-12)
    return;

  NumericalPoint histogramData(barNumber);
  for (int i=0; i<size; ++i)
  {
    int index = static_cast< int >((sample[i][0] - sample.getMin()[0]) / width);
    // x=xmax -> index=barnumber, so bound it
    index = std::min(index, barNumber-1);
    
    ++ histogramData[index];
  }
  double inverseArea = 1. / (size*width);
  for (UnsignedInteger i = 0; i < barNumber; ++i)
    histogramData[i] *= inverseArea;

  //  if CDF
  double sum = 1.;
  if (cdf)
  {
    sum = histogramData[0];
    for (int i=1; i<barNumber; i++)
    {
      sum += histogramData[i];
      histogramData[i] += histogramData[i-1];
    }
  }

  // create histogram
  QwtPlotHistogram * histogram = new QwtPlotHistogram;
  histogram->setBrush(QBrush(DefaultHistogramColor));

  QVector<QwtIntervalSample> samples(barNumber);
  for (int i=0; i<barNumber; i++)
  {
    QwtInterval interval(sample.getMin()[0]+i*width, sample.getMin()[0]+(i+1)*width);
    samples[i] = QwtIntervalSample(histogramData[i]/sum, interval);
  }

  histogram->setData(new QwtIntervalSeriesData(samples));

  // draw
  histogram->attach(this);
  replot();
}

void PlotWidget::drawBoxPlot(double median, double lowerQuartile, double upperQuartile,
                             double lowerBound, double upperBound, NumericalPoint outliers_)
{
  // draw median
  double xMedian[2] = {0.9, 1.1};
  double yMedian[2] = {median, median};
  plotCurve(xMedian, yMedian, 2, QPen(Qt::red));

  // draw box
  QwtPlotShapeItem *item = new QwtPlotShapeItem;

  QRectF rect(QPointF(0.9, lowerQuartile), QSizeF(0.2, upperQuartile-lowerQuartile));
  
  QPainterPath path;
  path.addRect(rect);
  item->setShape(path);
  item->setPen(QPen(Qt::blue));

  item->attach(this);

  // draw whiskers
  double xWhiskers[2] = {1., 1.};
  double yLower[2] = {lowerBound, lowerQuartile};
  plotCurve(xWhiskers, yLower, 2, QPen(Qt::black, 2, Qt::DashLine));

  double yUpper[2] = {upperQuartile, upperBound};
  plotCurve(xWhiskers, yUpper, 2, QPen(Qt::black, 2, Qt::DashLine));


  double xWhiskersBars[2] = {0.95, 1.05};
  double yLowerWhiskersBar[2] = {lowerBound, lowerBound};
  plotCurve(xWhiskersBars, yLowerWhiskersBar, 2);

  double yUpperWhiskersBar[2] = {upperBound, upperBound};
  plotCurve(xWhiskersBars, yUpperWhiskersBar, 2);

  // draw outliers
  const int dim = outliers_.getDimension();
  double * xOutliers = new double[dim];
  double * yOutliers = new double[dim];

  for (int i=0; i<dim; ++i)
  {
    xOutliers[i] = 1.;
    yOutliers[i] = outliers_[i];
  }

  plotCurve(xOutliers, yOutliers, dim, QPen(Qt::blue), QwtPlotCurve::NoCurve, new QwtSymbol(QwtSymbol::Cross, Qt::NoBrush, QPen(Qt::blue), QSize(5, 5)));

//   TODO think about:
//   QwtScaleDiv scaleDiv(0.5, 1.5);
//   QList< double > ticks;
//   scaleDiv.setTicks(QwtScaleDiv::NoTick, ticks);
//   setAxisScaleDiv(QwtPlot::xBottom, scaleDiv);

  setAxisScale(QwtPlot::xBottom, 0.5, 1.5, 0.5);
  replot();
}


void PlotWidget::updateScaleParameters(const Distribution & distribution)
{
  double mean = distribution.getMean()[0];
  double stepSize = 0.0;
  const double qmin = ResourceMap::GetAsNumericalScalar("DistributionImplementation-QMin");
  const double qmax = ResourceMap::GetAsNumericalScalar("DistributionImplementation-QMax");
  double x1 = distribution.computeQuantile(qmin)[0];
  double x2 = distribution.computeQuantile(qmax)[0];
  const double delta = 2.0 *(x2 - x1) *(1.0 - 0.5 *(qmax - qmin));
  x1 -= delta;
  x2 -= delta;

  DistributionScaleEngine xScaleEngine;
  xScaleEngine.setReference(mean);
  xScaleEngine.setAttribute(QwtScaleEngine::Symmetric, true);
  xScaleEngine.autoScale(3, x1, x2, stepSize);
  setAxisScale(QwtPlot::xBottom, x1, x2, stepSize);
}


}
