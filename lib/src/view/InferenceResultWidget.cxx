//                                               -*- C++ -*-
/**
 *  @brief QWidget for the inference results
 *
 *  Copyright 2015-2018 EDF-Phimeca
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
#include "otgui/InferenceResultWidget.hxx"

#include "otgui/ResizableStackedWidget.hxx"
#include "otgui/RadioButtonDelegate.hxx"
#include "otgui/WidgetBoundToDockWidget.hxx"
#include "otgui/GraphConfigurationWidget.hxx"
#include "otgui/TranslationManager.hxx"

#include <openturns/VisualTest.hxx>

#include <QScrollArea>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QDesktopServices>
#include <QUrl> // for qt4

using namespace OT;

namespace OTGUI
{

InferenceResultWidget::InferenceResultWidget(const bool displayPDF_QQPlot, QWidget* parent)
  : QWidget(parent)
  , displayPDF_QQPlot_(displayPDF_QQPlot)
  , tabWidget_(0)
  , currentFittingTestResult_()
  , distTableView_(0)
  , distTableModel_(0)
  , distParamTableView_(0)
  , distParamTableModel_(0)
  , infoButton_(0)
  , analysisErrorMessageLabel_(0)
  , pdfPlot_(0)
  , cdfPlot_(0)
  , qqPlot_(0)
{
  buildInterface();
}


void InferenceResultWidget::buildInterface()
{
  QVBoxLayout * mainLayout = new QVBoxLayout(this);

  QScrollArea * scrollArea = new QScrollArea;
  scrollArea->setWidgetResizable(true);

  QWidget * distTabListWidget = new QWidget;
  QHBoxLayout * distTabListWidgetLayout = new QHBoxLayout(distTabListWidget);

  // -- distributions table
  QGroupBox * distGroupBox = new QGroupBox(tr("Distributions"));
  QVBoxLayout * distGroupBoxLayout = new QVBoxLayout(distGroupBox);

  // --- table view
  distTableView_ = new ResizableTableViewWithoutScrollBar;
  RadioButtonDelegate * delegate = new RadioButtonDelegate(2, distTableView_);
  distTableView_->setItemDelegateForColumn(0, delegate);
  distTableView_->setSelectionMode(QAbstractItemView::NoSelection);
  distTableView_->verticalHeader()->hide();
  distTableView_->horizontalHeader()->hide();

  connect(distTableView_, SIGNAL(clicked(QModelIndex)), this, SLOT(updateParametersTable(QModelIndex)));
  connect(distTableView_, SIGNAL(clicked(QModelIndex)), this, SLOT(updateGraphs(QModelIndex)));
  connect(distTableView_, SIGNAL(clicked(QModelIndex)), this, SLOT(updateRadioButtonsDistributionTable(QModelIndex)));

  // --- table model
  distTableModel_ = new CustomStandardItemModel(0, 4, distTableView_);
  distTableView_->setModel(distTableModel_);

  distGroupBoxLayout->addWidget(distTableView_);
  distGroupBoxLayout->addStretch();

  distTabListWidgetLayout->addWidget(distGroupBox);

  // -- distribution parameters table
  // --- table view
  distParamTableView_ = new ResizableTableViewWithoutScrollBar;
  distParamTableView_->horizontalHeader()->hide();
  distParamTableView_->verticalHeader()->hide();
  distParamTableView_->setTabKeyNavigation(false);

  // --- table model
  distParamTableModel_ = new CustomStandardItemModel(4, 2, distParamTableView_);
  distParamTableView_->setModel(distParamTableModel_);

  pdfPlot_ = new PlotWidget(tr("distributionPDF"));

  // -- pdf/cdf qqPlot
  if (displayPDF_QQPlot_)
  {
    tabWidget_ = new QTabWidget;

    // tab PDF/CDF
    WidgetBoundToDockWidget * plotWidget = new WidgetBoundToDockWidget(this);
    QVBoxLayout * plotWidgetLayout = new QVBoxLayout(plotWidget);
    ResizableStackedWidget * pdf_cdfStackedWidget = new ResizableStackedWidget;

    // --- pdf
    pdf_cdfStackedWidget->addWidget(pdfPlot_);
    // --- cdf
    cdfPlot_ = new PlotWidget(tr("distributionCDF"));
    pdf_cdfStackedWidget->addWidget(cdfPlot_);
    // --- GraphConfigurationWidget
    QVector<PlotWidget*> listpdf_cdfPlot;
    listpdf_cdfPlot.append(pdfPlot_);
    listpdf_cdfPlot.append(cdfPlot_);
    GraphConfigurationWidget * pdf_cdfPlotSettingWidget = new GraphConfigurationWidget(listpdf_cdfPlot,
        QStringList(),
        QStringList(),
        GraphConfigurationWidget::PDF,
        this);
    pdf_cdfPlotSettingWidget->hide();
    plotWidget->setDockWidget(pdf_cdfPlotSettingWidget);
    connect(pdf_cdfPlotSettingWidget, SIGNAL(currentPlotChanged(int)), pdf_cdfStackedWidget, SLOT(setCurrentIndex(int)));

    plotWidgetLayout->addWidget(pdf_cdfStackedWidget);
    tabWidget_->addTab(plotWidget, tr("PDF/CDF"));

    // tab QQ plot
    plotWidget = new WidgetBoundToDockWidget(this);
    plotWidgetLayout = new QVBoxLayout(plotWidget);

    // --- qq plot
    qqPlot_ = new PlotWidget(tr("qqPlot"));
    plotWidgetLayout->addWidget(qqPlot_);
    // --- GraphConfigurationWidget
    GraphConfigurationWidget * qqPlotSettingWidget = new GraphConfigurationWidget(qqPlot_,
        QStringList(),
        QStringList(),
        GraphConfigurationWidget::NoType,
        this);
    qqPlotSettingWidget->hide();
    plotWidget->setDockWidget(qqPlotSettingWidget);

    tabWidget_->addTab(plotWidget, tr("Q-Q Plot"));

    // tab Parameters
    QWidget * paramWidget = new QWidget;
    QVBoxLayout * paramGroupBoxLayout = new QVBoxLayout(paramWidget);
    paramGroupBoxLayout->addWidget(distParamTableView_);
    // button to open the OT documentation
    infoButton_ = new QToolButton;
    infoButton_->setIcon(QIcon(":/images/documentinfo.png"));
    infoButton_->setToolTip(tr("Open the OpenTURNS documentation"));
    connect(infoButton_, SIGNAL(clicked()), this, SLOT(openUrl()));
    paramGroupBoxLayout->addWidget(infoButton_);
    analysisErrorMessageLabel_ = new QLabel;
    analysisErrorMessageLabel_->setWordWrap(true);
    paramGroupBoxLayout->addWidget(analysisErrorMessageLabel_);
    paramGroupBoxLayout->addStretch();

    tabWidget_->addTab(paramWidget, tr("Parameters"));

    distTabListWidgetLayout->addWidget(tabWidget_, 1);
  }
  else
  {
    QGroupBox * paramGroupBox = new QGroupBox(tr("Distribution parameters"));
    QVBoxLayout * paramGroupBoxLayout = new QVBoxLayout(paramGroupBox);
    paramGroupBoxLayout->addWidget(distParamTableView_);
    analysisErrorMessageLabel_ = new QLabel;
    analysisErrorMessageLabel_->setWordWrap(true);
    paramGroupBoxLayout->addWidget(analysisErrorMessageLabel_);
    paramGroupBoxLayout->addWidget(pdfPlot_);
    paramGroupBoxLayout->addStretch();
    distTabListWidgetLayout->setSizeConstraint(QLayout::SetFixedSize);
    distTabListWidgetLayout->addWidget(paramGroupBox, 1);
  }

  scrollArea->setWidget(distTabListWidget);
  mainLayout->addWidget(scrollArea);
}


void InferenceResultWidget::updateDistributionTable(const double level, const InferenceResult& result, const QString& variableName)
{
  // check
  if (!(distTableModel_ && distTableView_))
    return;
  if (variableName.isEmpty())
    return;

  // reset
  distTableModel_->clear();
  analysisErrorMessageLabel_->clear();

  // horizontal header
  distTableModel_->setNotEditableHeaderItem(0, 0, tr("Distribution"));
  distTableModel_->setNotEditableHeaderItem(0, 1, tr("Bayesian\nInformation\nCriterion"));
  distTableModel_->setData(distTableModel_->index(0, 1), tr("Lower BIC value is better"), Qt::ToolTipRole);
  distTableModel_->setNotEditableHeaderItem(0, 2, tr("Kolmogorov-Smirnov"));
  distTableModel_->setNotEditableHeaderItem(1, 2, tr("p-value"));
  distTableModel_->setNotEditableHeaderItem(1, 3, tr("Acceptation") + "\n(" + QString::number(level) + ")");
  distTableView_->setSpan(0, 0, 2, 1);
  distTableView_->setSpan(0, 1, 2, 1);

  // table
  // -- get results of the variable
  currentFittingTestResult_ = result.getFittingTestResultForVariable(variableName.toStdString());
  // BIC values
  Point bicValues(currentFittingTestResult_.getBICResults());
  // number of tests
  const UnsignedInteger nbTests = bicValues.getSize();

  // -- sort indices list
  Indices indices(nbTests);
  if (nbTests > 1)
  {
    indices.fill();
    std::sort(std::begin(indices),
              std::end(indices),
              [&](UnsignedInteger i1, UnsignedInteger i2) {return bicValues[i1] < bicValues[i2];});
  }

  // -- fill table
  for (UnsignedInteger i = 0; i < nbTests; ++i)
  {
    const int cellRow = i + 2; // because of 2 rows of titles
    const QString distName = TranslationManager::GetTranslatedDistributionName(currentFittingTestResult_.getTestedDistributions()[indices[i]].getImplementation()->getClassName());
    distTableModel_->setNotEditableItem(cellRow, 0, distName);
    distTableModel_->setData(distTableModel_->index(cellRow, 0), (int)indices[i], Qt::UserRole);
    if (currentFittingTestResult_.getErrorMessages()[indices[i]].empty())
    {
      distTableModel_->setNotEditableItem(cellRow, 1, bicValues[indices[i]], 3);
      distTableModel_->setNotEditableItem(cellRow, 2, currentFittingTestResult_.getKolmogorovTestResults()[indices[i]].getPValue(), 3);
      // if accepted
      if (currentFittingTestResult_.getKolmogorovTestResults()[indices[i]].getBinaryQualityMeasure())
      {
        distTableModel_->setNotEditableItem(cellRow, 3, tr("yes"), Qt::green);
      }
      else
      {
        distTableModel_->setNotEditableItem(cellRow, 3, tr("no"), QColor());
      }
    }
    else
    {
      distTableModel_->setNotEditableItem(cellRow, 1, "-");
      distTableModel_->setNotEditableItem(cellRow, 2, "-");
      distTableModel_->setNotEditableItem(cellRow, 3, tr("failed"), Qt::red);
    }
  }
  // -- update parameters table
  const int firstDistIndex = 0 + 2; // because of the 2 rows of titles
  distTableView_->selectRow(firstDistIndex);
  updateParametersTable(distTableModel_->index(firstDistIndex, 0));
  updateGraphs(distTableModel_->index(firstDistIndex, 0));
  distTableModel_->setData(distTableModel_->index(firstDistIndex, 0), true, Qt::CheckStateRole);

  // resize
  distTableView_->resizeColumnsToContents();
  int titleWidth = distTableView_->horizontalHeader()->sectionSize(1);
  // first: clear item at (0,2) because the text is too wide:
  // resizeColumnsToContents takes into account the text of item at (0,2)
  // to resize the column 2, even if there is a setSpan(0, 2, 1, 2)
  distTableModel_->setItem(0, 2, new QStandardItem);
  distTableView_->resizeToContents();
  distTableModel_->setNotEditableHeaderItem(0, 2, tr("Kolmogorov-Smirnov"));
  distTableView_->setSpan(0, 2, 1, 2);
  const int subTitlesWidth = distTableView_->horizontalHeader()->sectionSize(1) + distTableView_->horizontalHeader()->sectionSize(2);
  const int widthCorrection = titleWidth - subTitlesWidth;
  if (widthCorrection > 0)
  {
    // fix the table width
    distTableView_->horizontalHeader()->resizeSection(2, distTableView_->horizontalHeader()->sectionSize(2) + widthCorrection);
    distTableView_->setMinimumWidth(distTableView_->minimumWidth() + widthCorrection);
  }
}


void InferenceResultWidget::updateRadioButtonsDistributionTable(QModelIndex current)
{
  // check
  if (!distTableModel_ || current.row() < 2)
    return;

  // set CheckStateRole of distTableModel_
  for (int i = 2; i < distTableModel_->rowCount(); ++i)
  {
    if (distTableModel_->index(i, 0).row() == current.row())
      distTableModel_->setData(distTableModel_->index(i, 0), true, Qt::CheckStateRole);
    else
      distTableModel_->setData(distTableModel_->index(i, 0), false, Qt::CheckStateRole);
  }
}


void InferenceResultWidget::updateParametersTable(QModelIndex current)
{
  // check
  if (!(distTableModel_ && distParamTableModel_ && distParamTableView_ && analysisErrorMessageLabel_ && pdfPlot_))
    return;
  if (current.row() < 2)
    return;

  emit currentDistributionChanged();

  // reset
  distParamTableModel_->clear();
  analysisErrorMessageLabel_->clear();

  // if the selected distribution is not valid (failed inference)
  if (current.isValid())
  {
    const QVariant variant = distTableModel_->data(distTableModel_->index(current.row(), 0), Qt::UserRole);
    const int resultIndex = variant.value<int>();
    const String testResultMessage = currentFittingTestResult_.getErrorMessages()[resultIndex];
    if (!testResultMessage.empty())
    {
      const QString message = QString("<font color=red>%1</font>").arg(QString::fromUtf8(testResultMessage.c_str()));
      analysisErrorMessageLabel_->setText(message);
      analysisErrorMessageLabel_->show();
      distParamTableView_->hide();
      if (infoButton_)
        infoButton_->hide();
      return;
    }
  }

  // if the distribution is valid
  analysisErrorMessageLabel_->hide();
  distParamTableView_->show();
  if (infoButton_)
    infoButton_->show();

  distParamTableModel_->setColumnCount(2);

  // fill table
  // -- used font for titles
  QFont font;
  font.setBold(true);

  Distribution distribution;
  int row = -1;

  if (current.isValid())
  {
    // -- get distribution
    const QVariant variant = distTableModel_->data(distTableModel_->index(current.row(), 0), Qt::UserRole);
    distribution = currentFittingTestResult_.getTestedDistributions()[variant.value<int>()];

    // -- set parameters
    distParamTableModel_->setNotEditableItem(++row, 0, tr("Parameters"));
    distParamTableView_->setSpan(row, 0, 1, 2);
    distParamTableModel_->setData(distParamTableModel_->index(row, 0), font, Qt::FontRole);

    for (UnsignedInteger i = 0; i < distribution.getParameterDescription().getSize(); ++i)
    {
      const QString param(TranslationManager::GetTranslatedDistributionParameterName(distribution.getParameterDescription()[i]));
      distParamTableModel_->setNotEditableHeaderItem(++row, 0, param);
      distParamTableModel_->setNotEditableItem(row, 1, distribution.getParameter()[i]);
    }
  }
  distParamTableModel_->setNotEditableItem(++row, 0, tr("Moments"));
  distParamTableView_->setSpan(row, 0, 1, 2);
  distParamTableModel_->setData(distParamTableModel_->index(row, 0), font, Qt::FontRole);

  distParamTableModel_->setNotEditableHeaderItem(++row, 0, tr("Mean"));
  distParamTableModel_->setNotEditableHeaderItem(++row, 0, tr("Standard deviation"));
  distParamTableModel_->setNotEditableHeaderItem(++row, 0, tr("Skewness"));
  distParamTableModel_->setNotEditableHeaderItem(++row, 0, tr("Kurtosis"));

  if (current.isValid())
  {
    row = distribution.getParameterDescription().getSize() + 1;

    // -- set moments
    distParamTableModel_->setNotEditableItem(++row, 1, distribution.getMean()[0]);
    distParamTableModel_->setNotEditableItem(++row, 1, distribution.getStandardDeviation()[0]);
    try
    {
      distParamTableModel_->setNotEditableItem(++row, 1, distribution.getSkewness()[0]);
    }
    catch (std::exception & ex)
    {
      distParamTableModel_->setNotEditableItem(++row, 1, "-");
    }
    try
    {
      distParamTableModel_->setNotEditableItem(++row, 1, distribution.getKurtosis()[0]);
    }
    catch (std::exception & ex)
    {
      distParamTableModel_->setNotEditableItem(++row, 1, "-");
    }
  }
  else
  {
    for (UnsignedInteger i = 0; i < 4; ++i)
      distParamTableModel_->setNotEditableItem(1 + i, 1, "-");
  }
  // resize
  distParamTableView_->resizeToContents();
}


void InferenceResultWidget::updateGraphs(QModelIndex current)
{
  // check
  if (!(current.isValid() && distTableModel_ && currentFittingTestResult_.getValues().getSize() && pdfPlot_))
    return;
  if (current.row() < 2)
    return;

  // reset
  pdfPlot_->clear();
  if (cdfPlot_ && qqPlot_)
  {
    cdfPlot_->clear();
    qqPlot_->clear();
  }

  // update
  const QVariant variant = distTableModel_->data(distTableModel_->index(current.row(), 0), Qt::UserRole);
  const int resultIndex = variant.value<int>();

  const bool isInferredDistribution = currentFittingTestResult_.getErrorMessages()[resultIndex].empty();

  if (tabWidget_)
  {
    tabWidget_->setTabEnabled(0, isInferredDistribution);
    tabWidget_->setTabEnabled(1, isInferredDistribution);
  }

  if (!isInferredDistribution)
    return;

  // -- get distribution
  const Distribution distribution = currentFittingTestResult_.getTestedDistributions()[resultIndex];
  const QString distName = TranslationManager::GetTranslatedDistributionName(distribution.getImplementation()->getName());

  // update pdf
  pdfPlot_->plotHistogram(currentFittingTestResult_.getValues());
  pdfPlot_->plotPDFCurve(distribution);
  pdfPlot_->setTitle(tr("PDF") + ": " + distName);

  // update cdf / qqplot
  if (!(cdfPlot_ && qqPlot_))
    return;

  // -- cdf
  cdfPlot_->plotHistogram(currentFittingTestResult_.getValues(), 1);
  cdfPlot_->plotCDFCurve(distribution);
  cdfPlot_->setTitle(tr("CDF") + ": " + distName);

  // -- qq plot
  Graph qqPlotGraph(VisualTest::DrawQQplot(currentFittingTestResult_.getValues(), distribution));
  qqPlot_->setTitle(tr("Q-Q Plot") + ": " + distName);
  qqPlot_->setAxisTitle(QwtPlot::yLeft, tr("Data quantiles"));
  qqPlot_->setAxisTitle(QwtPlot::xBottom, tr("%1 theoretical quantiles").arg(distName));
  qqPlot_->plotCurve(qqPlotGraph.getDrawable(1).getData(), QPen(Qt::blue, 5), QwtPlotCurve::Dots);
  qqPlot_->plotCurve(qqPlotGraph.getDrawable(0).getData());
}


Distribution InferenceResultWidget::getDistribution() const
{
  // check
  if (!(distTableModel_ && distTableView_))
    throw InternalException(HERE) << "Error in InferenceResultWidget::getDistribution";

  // get current distribution
  // loop begins at 2 because the two first rows are the table titles
  QModelIndex selectedDistributionIndex;
  for (int i = 2; i < distTableModel_->rowCount(); ++i)
    if (distTableModel_->data(distTableModel_->index(i, 0), Qt::CheckStateRole).toBool())
      selectedDistributionIndex = distTableModel_->index(i, 0);

  Distribution distribution;
  if (selectedDistributionIndex.isValid())
  {
    const QVariant variant = distTableModel_->data(selectedDistributionIndex, Qt::UserRole);
    return currentFittingTestResult_.getTestedDistributions()[variant.value<int>()];
  }

  return distribution;
}


bool InferenceResultWidget::isSelectedDistributionValid() const
{
  // check
  if (!(distTableModel_ && distTableView_))
    return false;

  // get current distribution
  // loop begins at 2 because the two first rows are the table titles
  QModelIndex selectedDistributionIndex;
  for (int i = 2; i < distTableModel_->rowCount(); ++i)
    if (distTableModel_->data(distTableModel_->index(i, 0), Qt::CheckStateRole).toBool())
      selectedDistributionIndex = distTableModel_->index(i, 1);

  Q_ASSERT(selectedDistributionIndex.isValid());

  return distTableModel_->data(selectedDistributionIndex) != "-";
}


void InferenceResultWidget::openUrl()
{
  // check
  if (!(distTableModel_ && distTableView_))
    return;

  const String distName = getDistribution().getImplementation()->getClassName();

  // open url
  const QString link = "http://openturns.github.io/openturns/master/user_manual/_generated/openturns." + QString(distName.c_str()) + ".html";
  QDesktopServices::openUrl(QUrl(link));
}
}
