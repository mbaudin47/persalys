//                                               -*- C++ -*-
/**
 *  @brief QwtPlot for all the graphics of the interface
 *
 *  Copyright 2015-2019 EDF-Phimeca
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
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#include "persalys/PlotWidget.hxx"

#include "persalys/CustomScaleEngine.hxx"
#include "persalys/CustomScaleDraw.hxx"
#include "persalys/GraphConfigurationWidget.hxx"
#include "persalys/UIntSpinBox.hxx"
#include "persalys/FileTools.hxx"
#include "persalys/QtTools.hxx"

#include <QMenu>
#include <QFileDialog>
#include <QImageWriter>
#include <QMessageBox>
#include <QApplication>
#include <QWizard>
#include <QGroupBox>
#include <QGridLayout>
#include <QLabel>

#include <qwt_plot_layout.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_histogram.h>
#include <qwt_plot_magnifier.h>
#include <qwt_plot_picker.h>
#include <qwt_legend.h>
#include <qwt_column_symbol.h>
#include <qwt_plot_renderer.h>
#include <qwt_picker_machine.h>
#include <qwt_scale_widget.h>
#include <qwt_plot_marker.h>

using namespace OT;

namespace PERSALYS
{

// -- custom QwtPlotCurve with Rtti_PlotUserItem = 1001
class PERSALYS_API StaticPlotCurveItem : public QwtPlotCurve
{
public:
  StaticPlotCurveItem() {};
  virtual int rtti() const
  {
    return 1001;
  };
};


// -- class PlotWidget --

const QColor PlotWidget::DefaultHistogramColor = QColor(127, 172, 210);

PlotWidget::PlotWidget(const QString &plotTypeName, const bool disableZoom, QWidget *parent)
  : QwtPlot(parent)
  , plotTypeName_(plotTypeName)
{
  if (!disableZoom)
  {
    // panning with the left mouse button
    (void) new QwtPlotPanner(canvas());

    // zoom in/out with the wheel
    QwtPlotMagnifier * magnifier = new QwtPlotMagnifier(canvas());
    // set to NoButton to have not interference with the right click of the context menu
    magnifier->setMouseButton(Qt::NoButton);
    // to zoom with the right direction of the wheel
    magnifier->setWheelFactor(1.1);
  }

  // show coordinates
  ( void ) new QwtPlotPicker(QwtPlot::xBottom, QwtPlot::yLeft,
                             QwtPlotPicker::CrossRubberBand, QwtPicker::AlwaysOn, canvas());

  setCanvasBackground(Qt::white);
  plotLayout()->setAlignCanvasToScales(true);

  // QwtScaleDraw with locale insensitive labels
  setAxisScaleDraw(xBottom, new CustomScaleDraw);
  setAxisScaleDraw(yLeft, new CustomScaleDraw);

  clear();

  // context menu
  setContextMenuPolicy(Qt::CustomContextMenu);
  connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenu(QPoint)));
}


// show the context menu when right clicking
void PlotWidget::contextMenu(const QPoint & pos)
{
  QMenu contextMenu(this);
  QAction * exportPlotAction = new QAction(tr("Export plot"), this);
  connect(exportPlotAction, SIGNAL(triggered(bool)), this, SLOT(exportPlot()));
  contextMenu.addAction(new QAction(tr("Export plot"), &contextMenu));
  contextMenu.popup(mapToGlobal(pos));
}


void PlotWidget::exportPlot()
{
  // wizard to choose image size
  QWizard sizeSelectionWizard(this);
  sizeSelectionWizard.setWindowIcon(QIcon(":/images/OT_icon16x16.png"));
  sizeSelectionWizard.setButtonText(QWizard::CancelButton, tr("Cancel"));
  sizeSelectionWizard.setButtonText(QWizard::FinishButton, tr("OK"));
  sizeSelectionWizard.setOption(QWizard::NoDefaultButton, true);
  sizeSelectionWizard.setOption(QWizard::NoBackButtonOnStartPage, true);
  QWizardPage * page = new QWizardPage;
  QVBoxLayout * pageLayout = new QVBoxLayout(page);
  QGroupBox * groupBox = new QGroupBox(tr("Size"));
  QGridLayout * groupBoxLayout = new QGridLayout(groupBox);
  groupBoxLayout->addWidget(new QLabel(tr("Image resolution")), 0, 0);
  UIntSpinBox * spinBoxW = new UIntSpinBox;
  spinBoxW->setSingleStep(100);
  spinBoxW->setValue(512);
  groupBoxLayout->addWidget(spinBoxW, 0, 1);
  groupBoxLayout->addWidget(new QLabel("x"), 0, 2, Qt::AlignHCenter);
  UIntSpinBox * spinBoxH = new UIntSpinBox;
  spinBoxH->setSingleStep(100);
  spinBoxH->setValue(512);
  groupBoxLayout->addWidget(spinBoxH, 0, 3);
  pageLayout->addWidget(groupBox);
  sizeSelectionWizard.addPage(page);

  // get image size
  if (!sizeSelectionWizard.exec())
    return;
  const int imageWidth = spinBoxW->value();
  const int imageHeight = spinBoxH->value();

  // save image
  QString fileName = QFileDialog::getSaveFileName(this, tr("Export plot"),
                     FileTools::GetCurrentDir() + QDir::separator() + plotTypeName_,
                     tr("PNG (*.png);; JPEG (*.jpg *.jpeg);; BMP (*.bmp);; PPM (*.ppm);; XBM (*.xbm);; XPM (*.xpm);; TIFF (*.tiff);; SVG (*.svg);; PDF (*.pdf);; PS (*.ps)"));

  if (!fileName.isEmpty())
  {
    QString format = QFileInfo(fileName).suffix().toLower();
    FileTools::SetCurrentDir(fileName);

    if (format == "")
    {
      fileName += ".png";
      format = QString("png");
    }
    if (format == "ps" || format == "pdf" || format == "svg")
    {
      QwtPlotRenderer * renderer = new QwtPlotRenderer();
      renderer->renderDocument(this, fileName, QSizeF(imageWidth, imageHeight));
    }
    else
    {
      // QwtPlotRenderer supports all the listed formats
      // but renderDocument doesn't emit error message...
      if (QImageWriter::supportedImageFormats().indexOf(format.toLatin1()) >= 0)
      {
        QPixmap pixmap(imageWidth, imageHeight);
        pixmap.fill();
        QwtPlotRenderer renderer;

        renderer.renderTo(this, pixmap);

        bool saveOperationSucceed = pixmap.save(fileName, format.toLatin1());
        if (!saveOperationSucceed)
          QMessageBox::warning(QApplication::activeWindow(), tr("Warning"), tr("Impossible to export the plot."));
      }
      else
      {
        QMessageBox::warning(QApplication::activeWindow(), tr("Warning"), tr("Format not supported."));
      }
    }
  }
}


void PlotWidget::plotCurve(QVector<QPointF> points, const QPen pen, QwtPlotCurve::CurveStyle style, QwtSymbol* symbol, QString title, bool isStatic)
{
  QwtPlotCurve * curve;
  if (isStatic)
    curve = new StaticPlotCurveItem;
  else
    curve = new QwtPlotCurve;
  curve->setSamples(points);
  curve->setPen(pen);
  curve->setStyle(style);
  if (symbol)
    curve->setSymbol(symbol);
  if (!title.isEmpty())
  {
    curve->setTitle(title);
    if (symbol)
      curve->setLegendAttribute(QwtPlotCurve::LegendShowSymbol, true);
    else
      curve->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
  }
  else
    curve->setItemAttribute(QwtPlotItem::Legend, false);
  curve->attach(this);
  replot();
}


void PlotWidget::plotCurve(double * x, double * y, int size, const QPen pen, QwtPlotCurve::CurveStyle style, QwtSymbol* symbol, QString title)
{
  QwtPlotCurve * curve = new QwtPlotCurve;
  curve->setSamples(x, y, size);
  curve->setPen(pen);
  curve->setStyle(style);
  if (symbol)
    curve->setSymbol(symbol);
  if (!title.isEmpty())
  {
    curve->setTitle(title);
  }
  else
  {
    curve->setItemAttribute(QwtPlotItem::Legend, false);
  }
  curve->attach(this);

  replot();
}


void PlotWidget::plotCurve(const Sample & data, const QPen pen, QwtPlotCurve::CurveStyle style, QwtSymbol* symbol, QString title)
{
  const int size = data.getSize();
  double * x = new double[size];
  double * y = new double[size];

  for (int i = 0; i < size; ++ i)
  {
    x[i] = data(i, 0);
    y[i] = data(i, 1);
  }
  plotCurve(x, y, size, pen, style, symbol, title);
  delete[] x;
  delete[] y;
}


void PlotWidget::plotScatter(const Sample & input, const Sample & output,
                             const QPen pen, QString Xtitle, QString Ytitle)
{
  if (input.getDimension()*input.getSize()*output.getDimension()*output.getSize() == 0)
  {
    qDebug() << "In plotScatter: a sample is empty";
    return;
  }
  if (input.getDimension()*output.getDimension() != 1)
  {
    qDebug() << "In plotScatter: the samples must have a dimension of 1";
    return;
  }
  if (output.getSize() != input.getSize())
  {
    qDebug() << "In plotScatter: the 2 samples must have the same size";
    return;
  }

  double *xData = const_cast<double*>(&(input[0][0]));
  double *yData = const_cast<double*>(&(output[0][0]));

  plotCurve(xData, yData, input.getSize(), pen, QwtPlotCurve::Dots);

  setAxisTitle(QwtPlot::xBottom, Xtitle);
  setAxisTitle(QwtPlot::yLeft, Ytitle);
}


void PlotWidget::plotPDFCurve(const Distribution & distribution, const QPen pen)
{
  setTitle(tr("PDF"));
  setAxisTitle(QwtPlot::yLeft, tr("Density"));
  setAxisTitle(QwtPlot::xBottom, tr("X"));

  updateScaleParameters(distribution);
  const Sample dataPDF(distribution.drawPDF().getDrawable(0).getData());
  plotCurve(dataPDF, pen);
  // Add margin at the top to avoid to cut the curve
  const double yMax = dataPDF.getMax()[1];
  if (axisInterval(QwtPlot::yLeft).maxValue() < (yMax * (1 + 0.02)))
    setAxisScale(QwtPlot::yLeft, 0, yMax * (1 + 0.02));
  replot();
}


void PlotWidget::plotCDFCurve(const Distribution & distribution, const QPen pen)
{
  setTitle(tr("CDF"));
  setAxisTitle(QwtPlot::yLeft, tr("CDF"));
  setAxisTitle(QwtPlot::xBottom, tr("X"));

  updateScaleParameters(distribution);
  const Sample dataCDF(distribution.drawCDF().getDrawable(0).getData());
  plotCurve(dataCDF, pen);
  // Add margin at the top to avoid to cut the curve
  const double yMax = dataCDF.getMax()[1];
  if (axisInterval(QwtPlot::yLeft).maxValue() < (yMax * (1 + 0.02)))
    setAxisScale(QwtPlot::yLeft, 0, yMax * (1 + 0.02));
  replot();
}


void PlotWidget::plotQuantileCurve(const Distribution & distribution, const QPen pen)
{
  setTitle(tr("Quantile"));
  setAxisTitle(QwtPlot::yLeft, tr("Quantile"));
  setAxisTitle(QwtPlot::xBottom, tr("X"));

  updateScaleParameters(distribution);
  const Sample dataQuantile(distribution.drawQuantile().getDrawable(0).getData());
  plotCurve(dataQuantile, pen);
  // Add margin at the top to avoid to cut the curve
  double yMax = dataQuantile.getMax()[1];
  if (axisInterval(QwtPlot::yLeft).maxValue() < (yMax * (1 + 0.02)))
    yMax = yMax * (1 + 0.02);
  setAxisScale(QwtPlot::yLeft, dataQuantile.getMin()[1], yMax * (1 + 0.02));
  setAxisScale(QwtPlot::xBottom, -0.01, 1.01);
  replot();
}


void PlotWidget::plotSurvivalCurve(const Distribution & distribution, const QPen pen)
{
  setTitle(tr("Survival function"));
  setAxisTitle(QwtPlot::yLeft, tr("Survival function"));
  setAxisTitle(QwtPlot::xBottom, tr("X"));

  updateScaleParameters(distribution);
  const Scalar xMin = distribution.computeQuantile(ResourceMap::GetAsScalar("Distribution-QMin"))[0];
  const Scalar xMax = distribution.computeQuantile(ResourceMap::GetAsScalar("Distribution-QMax"))[0];
  const UnsignedInteger pointNumber = ResourceMap::GetAsUnsignedInteger("Distribution-DefaultPointNumber");
  const Scalar delta = 2.0 * (xMax - xMin) * (1.0 - 0.5 * (ResourceMap::GetAsScalar("Distribution-QMax" ) - ResourceMap::GetAsScalar("Distribution-QMin")));
  const Sample data(distribution.computeComplementaryCDF(xMin - delta, xMax + delta, pointNumber));
  plotCurve(data, pen);
  // Add margin at the top to avoid to cut the curve
  double yMax = data.getMax()[1];
  if (axisInterval(QwtPlot::yLeft).maxValue() < (yMax * (1 + 0.02)))
    yMax = yMax * (1 + 0.02);
  setAxisScale(QwtPlot::yLeft, data.getMin()[1], yMax * (1 + 0.02));
  replot();
}


// graphType = 0 -> PDF
// graphType = 1 -> CDF
// graphType = 2 -> other
void PlotWidget::plotHistogram(const Sample & sample, const UnsignedInteger graphType, int barNumber, QString title)
{
  if (graphType > 2)
    throw InvalidArgumentException(HERE) << "Type of graph not known " << graphType;

  const int size = (int)sample.getSize();
  if (size == 0) throw InvalidArgumentException(HERE) << "Cannot draw an histogram based on an empty sample.";

  // compute bar number
  if (barNumber <= 0)
    barNumber = ceil(1.0 + log(std::max(size, 1)) / log(2));

  // compute data
  double sampleMin = sample.getMin()[0];
  double width = (sample.getMax()[0] - sampleMin) / barNumber;
  if (width < 1e-12)
    return;

  Point histogramData(barNumber);

  for (int i = 0; i < size; ++i)
  {
    int index = static_cast< int >((sample(i, 0) - sampleMin) / width);
    // x=xmax -> index=barnumber, so bound it
    index = std::min(index, barNumber - 1);
    if (!(index > barNumber || index < 0))
      ++ histogramData[index];
  }

  // if PDF or CDF
  if (graphType < 2)
  {
    double inverseArea = 1. / (size * width);
    for (int i = 0; i < barNumber; ++i)
      histogramData[i] *= inverseArea;
  }

  //  if CDF
  double sum = 1.;
  if (graphType == 1)
  {
    sum = histogramData[0];
    for (int i = 1; i < barNumber; i++)
    {
      sum += histogramData[i];
      histogramData[i] += histogramData[i - 1];
    }
  }

  // create histogram
  QwtPlotHistogram * histogram = new QwtPlotHistogram;
  histogram->setBrush(QBrush(DefaultHistogramColor));

  QVector<QwtIntervalSample> samples(barNumber);
  for (int i = 0; i < barNumber; i++)
  {
    QwtInterval interval(sampleMin + i * width, sampleMin + (i + 1)*width);
    samples[i] = QwtIntervalSample(histogramData[i] / sum, interval);
  }

  histogram->setData(new QwtIntervalSeriesData(samples));
  histogram->setTitle(title);

  // draw
  histogram->attach(this);
  replot();
}


void PlotWidget::plotSensitivityIndices(const Point& firstOrderIndices,
                                        const Point& totalIndices,
                                        const Description& inputNames,
                                        const Interval& firstOrderIndicesIntervals,
                                        const Interval& totalIndicesIntervals,
                                        const QStringList& legendNames)
{
  Q_ASSERT(legendNames.size() == 2);
  setAxisTitle(QwtPlot::yLeft, tr("Index"));
  setAxisTitle(QwtPlot::xBottom, tr("Inputs"));

  // populate bar chart
  static const char *colors[] = {"DarkOrchid", "SteelBlue"};

  const UnsignedInteger size = firstOrderIndices.getSize();

  double *xData = new double[size];
  double *yData = new double[size];

  double width = 0.;
  if (totalIndices.getSize())
    width = 0.1;

  double yMin = 0.;
  double yMax = 0.;
  for (UnsignedInteger i = 0 ; i < size ; ++i)
  {
    xData[i] = (i - width);
    yData[i] = firstOrderIndices[i];
    yMin = std::min(yMin, firstOrderIndices[i]);
    yMax = std::max(yMax, firstOrderIndices[i]);
    //qDebug() << "x= " << xData[i] << " , y= " << yData[i];
    if (firstOrderIndicesIntervals.getDimension() == size)
    {
      double xInterval[2] = {(i - width), (i - width)};
      double yInterval[2] = {firstOrderIndicesIntervals.getLowerBound()[i], firstOrderIndicesIntervals.getUpperBound()[i]};
      plotCurve(xInterval, yInterval, 2, QPen(colors[0]));

      yMin = std::min(yMin, firstOrderIndicesIntervals.getLowerBound()[i]);
      yMax = std::max(yMax, firstOrderIndicesIntervals.getUpperBound()[i]);
    }
  }

  plotCurve(xData, yData, size, QPen(Qt::black), QwtPlotCurve::NoCurve, new QwtSymbol(QwtSymbol::Ellipse, QBrush(colors[0]), QPen(colors[0]), QSize(5, 5)), legendNames[0]);
  delete[] xData;
  delete[] yData;

  if (totalIndices.getSize())
  {
    xData = new double[size];
    yData = new double[size];

    for (UnsignedInteger i = 0 ; i < size ; ++i)
    {
      xData[i] = (i + width) ;
      yData[i] = totalIndices[i];
      yMin = std::min(yMin, totalIndices[i]);
      yMax = std::max(yMax, totalIndices[i]);
      //qDebug() << "x= " << xData[i] << " , y= " << yData[i];
      if (totalIndicesIntervals.getDimension() == size)
      {
        double xInterval[2] = {(i + width), (i + width)};
        double yInterval[2] = {totalIndicesIntervals.getLowerBound()[i], totalIndicesIntervals.getUpperBound()[i]};
        plotCurve(xInterval, yInterval, 2, QPen(colors[1]));

        yMin = std::min(yMin, totalIndicesIntervals.getLowerBound()[i]);
        yMax = std::max(yMax, totalIndicesIntervals.getUpperBound()[i]);
      }
    }
    plotCurve(xData, yData, size, QPen(Qt::black), QwtPlotCurve::NoCurve, new QwtSymbol(QwtSymbol::Rect, QBrush(colors[1]), QPen(colors[1]), QSize(5, 5)), legendNames[1]);

    insertLegend(new QwtLegend(), QwtPlot::BottomLegend);
    delete[] xData;
    delete[] yData;
  }

  // scales
  setAxisScale(QwtPlot::xBottom, -0.5, firstOrderIndices.getSize() - 0.5, 1.0);
  setAxisMaxMinor(QwtPlot::xBottom, 0);
  setAxisScaleDraw(QwtPlot::xBottom, new CustomHorizontalScaleDraw(QtOT::DescriptionToStringList(inputNames)));

  // rescale to avoid to cut points
  yMin = yMin - (std::abs(0.05 * yMin) < 0.01 ? 0.05 : std::abs(0.05 * yMin));
  yMax = yMax + std::abs(0.05 * yMax);
  setAxisScale(QwtPlot::yLeft, yMin, yMax);

  // horizontal line y = 0
  QwtPlotMarker * hMarker = new QwtPlotMarker;
  hMarker->setLineStyle(QwtPlotMarker::HLine);
  hMarker->setLinePen(QPen(Qt::darkGray, 1));
  hMarker->attach(this);

  replot();
}


void PlotWidget::updateScaleParameters(const Distribution & distribution)
{
  double mean = distribution.getMean()[0];
  double stepSize = 0.0;
  const double qmin = ResourceMap::GetAsScalar("Distribution-QMin");
  const double qmax = ResourceMap::GetAsScalar("Distribution-QMax");
  double x1 = distribution.computeQuantile(qmin)[0];
  double x2 = distribution.computeQuantile(qmax)[0];
  const double delta = 2.0 * (x2 - x1) * (1.0 - 0.5 * (qmax - qmin));
  x1 -= delta;
  x2 -= delta;

  CustomScaleEngine xScaleEngine;
  xScaleEngine.setReference(mean);
  xScaleEngine.setAttribute(QwtScaleEngine::Symmetric, true);
  xScaleEngine.autoScale(3, x1, x2, stepSize);
  setAxisScale(QwtPlot::xBottom, x1, x2, stepSize);
}


void PlotWidget::clear()
{
  detachItems();
  setAxisAutoScale(QwtPlot::xBottom);
  enableAxis(QwtPlot::xBottom);
  setAxisAutoScale(QwtPlot::yLeft);
  enableAxis(QwtPlot::yLeft);
  // TODO initialize grid
//   grid_ = new QwtPlotGrid;
  replot();
}


void PlotWidget::replot()
{
  QwtPlot::replot();
  emit plotChanged();
}


QVector<PlotWidget*> PlotWidget::GetListScatterPlots(const Sample& inS,
    const Sample& outS,
    const Sample& notValidInS,
    const QStringList inNames,
    const QStringList inAxisNames,
    const QStringList outNames,
    const QStringList outAxisNames
                                                    )
{
  QVector<PlotWidget*> listScatterPlotWidgets;

  const UnsignedInteger nbInputs = inS.getSize() ? inS.getDimension() : 0;
  const UnsignedInteger nbOutputs = outS.getSize() ? outS.getDimension() : 0;
  const QPen pen(Qt::blue, 4);
  const QPen notValidPen(Qt::red, 4);

  // in rank
  Sample inSrank;
  if (nbInputs)
    inSrank = inS.rank() / inS.getSize();
  Sample notValidInSrank;
  if (notValidInS.getSize())
    notValidInSrank = notValidInS.rank() / notValidInS.getSize();
  // out rank
  Sample outSrank;
  if (nbOutputs)
    outSrank = outS.rank() / outS.getSize();

  for (UnsignedInteger j = 0; j < nbInputs; ++j)
  {
    for (UnsignedInteger i = 0; i < nbOutputs; ++i)
    {
      PlotWidget * plot = new PlotWidget(tr("scatterplot"));
      plot->plotScatter(inS.getMarginal(j), outS.getMarginal(i), pen, inAxisNames[j], outAxisNames[i]);
      plot->setTitle(tr("Scatter plot:") + " " + outNames[i] + " " + tr("vs") + " " + inNames[j]);
      listScatterPlotWidgets.append(plot);

      // ranks
      plot = new PlotWidget(tr("scatterplot"));
      plot->plotScatter(inSrank.getMarginal(j), outSrank.getMarginal(i), pen, inAxisNames[j], outAxisNames[i]);
      plot->setTitle(tr("Scatter plot:") + " " + outNames[i] + " " + tr("vs") + " " + inNames[j]);
      listScatterPlotWidgets.append(plot);
    }
    for (UnsignedInteger i = 0; i < nbInputs; ++i)
    {
      if (i != j)
      {
        PlotWidget * plot = new PlotWidget(tr("scatterplot"));
        plot->plotScatter(inS.getMarginal(j), inS.getMarginal(i), pen, inAxisNames[j], inAxisNames[i]);
        if (notValidInS.getSize())
          plot->plotScatter(notValidInS.getMarginal(j), notValidInS.getMarginal(i), notValidPen, inAxisNames[j], inAxisNames[i]);
        plot->setTitle(tr("Scatter plot:") + " " + inNames[i] + " " + tr("vs") + " " + inNames[j]);
        listScatterPlotWidgets.append(plot);

        // ranks
        plot = new PlotWidget(tr("scatterplot"));
        plot->plotScatter(inSrank.getMarginal(j), inSrank.getMarginal(i), pen, inAxisNames[j], inAxisNames[i]);
        if (notValidInS.getSize())
          plot->plotScatter(notValidInSrank.getMarginal(j), notValidInSrank.getMarginal(i), notValidPen, inAxisNames[j], inAxisNames[i]);
        plot->setTitle(tr("Scatter plot:") + " " + inNames[i] + " " + tr("vs") + " " + inNames[j]);
        listScatterPlotWidgets.append(plot);
      }
    }
  }
  for (UnsignedInteger j = 0; j < nbOutputs; ++j)
  {
    for (UnsignedInteger i = 0; i < nbOutputs; ++i)
    {
      if (i != j)
      {
        PlotWidget * plot = new PlotWidget(tr("scatterplot"));
        plot->plotScatter(outS.getMarginal(j), outS.getMarginal(i), pen, outAxisNames[j], outAxisNames[i]);
        plot->setTitle(tr("Scatter plot:") + " " + outNames[i] + " " + tr("vs") + " " + outNames[j]);
        listScatterPlotWidgets.append(plot);

        // ranks
        plot = new PlotWidget(tr("scatterplot"));
        plot->plotScatter(outSrank.getMarginal(j), outSrank.getMarginal(i), pen, outAxisNames[j], outAxisNames[i]);
        plot->setTitle(tr("Scatter plot:") + " " + outNames[i] + " " + tr("vs") + " " + outNames[j]);
        listScatterPlotWidgets.append(plot);
      }
    }
    for (UnsignedInteger i = 0; i < nbInputs; ++i)
    {
      PlotWidget * plot = new PlotWidget(tr("scatterplot"));
      plot->plotScatter(outS.getMarginal(j), inS.getMarginal(i), pen, outAxisNames[j], inAxisNames[i]);
      plot->setTitle(tr("Scatter plot:") + " " + inNames[i] + " " + tr("vs") + " " + outNames[j]);
      listScatterPlotWidgets.append(plot);

      // ranks
      plot = new PlotWidget(tr("scatterplot"));
      plot->plotScatter(outSrank.getMarginal(j), inSrank.getMarginal(i), pen, outAxisNames[j], inAxisNames[i]);
      plot->setTitle(tr("Scatter plot:") + " " + inNames[i] + " " + tr("vs") + " " + outNames[j]);
      listScatterPlotWidgets.append(plot);
    }
  }
  return listScatterPlotWidgets;
}
}
