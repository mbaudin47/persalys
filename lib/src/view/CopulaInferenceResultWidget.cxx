//                                               -*- C++ -*-
/**
 *  @brief QWidget with a list of copulas and their PDF-CDF/parameters
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
#include "otgui/CopulaInferenceResultWidget.hxx"

#include "otgui/RadioButtonDelegate.hxx"
#include "otgui/CopulaParametersTabWidget.hxx"
#include "otgui/ResizableStackedWidget.hxx"
#include "otgui/InferenceResultWidget.hxx" // for Q_DECLARE_METATYPE(OT::Distribution)
#include "otgui/TranslationManager.hxx"

#include <QScrollArea>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QHeaderView>

using namespace OT;

namespace OTGUI
{

CopulaInferenceResultWidget::CopulaInferenceResultWidget(const CopulaInferenceSetResult& currentSetResult,
    const Sample& sample,
    const bool displayPDF,
    QWidget* parent)
  : QWidget(parent)
  , currentSetResult_(currentSetResult)
  , sample_(sample)
  , displayPDF_(displayPDF)
  , distTableView_(0)
  , distTableModel_(0)
{
  buildInterface();
}


void CopulaInferenceResultWidget::buildInterface()
{
  QVBoxLayout * mainLayout = new QVBoxLayout(this);

  QScrollArea * scrollArea = new QScrollArea;
  scrollArea->setWidgetResizable(true);

  QWidget * distTabListWidget = new QWidget;
  QHBoxLayout * distTabListWidgetLayout = new QHBoxLayout(distTabListWidget);

  // -- copulas table
  QGroupBox * distGroupBox = new QGroupBox(tr("Tested copulas"));
  QVBoxLayout * distGroupBoxLayout = new QVBoxLayout(distGroupBox);

  // --- table view
  distTableView_ = new ResizableTableViewWithoutScrollBar;
  RadioButtonDelegate * delegate = new RadioButtonDelegate(1, distTableView_);
  distTableView_->setItemDelegateForColumn(0, delegate);
  distTableView_->setSelectionBehavior(QAbstractItemView::SelectRows);
  distTableView_->verticalHeader()->hide();
  distTableView_->horizontalHeader()->hide();

  // --- table model
  distTableModel_ = new CustomStandardItemModel(currentSetResult_.getTestedDistributions().getSize() + 1, 1, distTableView_);
  // --- fill table
  // horizontal header
  distTableModel_->setNotEditableHeaderItem(0, 0, tr("Copulas"));
  // distributions list
  for (UnsignedInteger i = 0; i < currentSetResult_.getTestedDistributions().getSize(); ++i)
  {
    String distributionName = currentSetResult_.getTestedDistributions()[i].getImplementation()->getClassName();
    distributionName = distributionName.substr(0, distributionName.find("Copula"));
    distTableModel_->setNotEditableItem(i + 1, 0, TranslationManager::GetTranslatedDistributionName(distributionName));
    const QVariant aVariant = QVariant::fromValue(currentSetResult_.getTestedDistributions()[i]);
    distTableModel_->setData(distTableModel_->index(i + 1, 0), aVariant, Qt::UserRole);
  }
  distTableView_->setModel(distTableModel_);
  distTableModel_->setData(distTableModel_->index(0 + 1, 0), true, Qt::CheckStateRole);
  distTableView_->resizeToContents();

  connect(distTableView_, SIGNAL(clicked(QModelIndex)), this, SLOT(updateParametersTable(QModelIndex)));
  connect(distTableView_, SIGNAL(clicked(QModelIndex)), this, SLOT(updateRadioButtonsDistributionTable(QModelIndex)));
  distTableView_->selectRow(0 + 1);

  distGroupBoxLayout->addWidget(distTableView_);
  distGroupBoxLayout->addStretch();

  distTabListWidgetLayout->addWidget(distGroupBox);

  // -- for each copula, display PDF-CDF/parameters
  ResizableStackedWidget * paramStackWidget = new ResizableStackedWidget;
  for (UnsignedInteger i = 0; i < currentSetResult_.getTestedDistributions().getSize(); ++i)
  {
    // if valid copula
    if (currentSetResult_.getErrorMessages()[i].empty())
    {
      CopulaParametersTabWidget * paramWidget = new CopulaParametersTabWidget(currentSetResult_.getTestedDistributions()[i],
          sample_,
          currentSetResult_.getKendallPlotData()[i],
          this);
      paramStackWidget->addWidget(paramWidget);
    }
    // if not valid copula
    else
    {
      QTabWidget * paramWidget = new QTabWidget;

      // PDF/CDF tab
      QWidget * aWidget = new QWidget;
      paramWidget->addTab(aWidget, tr("PDF/CDF"));
      paramWidget->setTabEnabled(0, false);
      // Kendall tab
      aWidget = new QWidget;
      paramWidget->addTab(aWidget, tr("Kendall plot"));
      paramWidget->setTabEnabled(1, false);
      // Parameters tab
      const QString message = QString("<font color=red>%1</font>").arg(QString::fromUtf8(currentSetResult_.getErrorMessages()[i].c_str()));
      aWidget = new QWidget;
      QVBoxLayout * aWidgetLayout = new QVBoxLayout(aWidget);
      QLabel * errorMessageLabel = new QLabel(message);
      errorMessageLabel->setWordWrap(true);
      aWidgetLayout->addWidget(errorMessageLabel);
      aWidgetLayout->addStretch();
      paramWidget->addTab(aWidget, tr("Parameters"));

      paramStackWidget->addWidget(paramWidget);
    }
  }
  connect(this, SIGNAL(distributionChanged(int)), paramStackWidget, SLOT(setCurrentIndex(int)));

  distTabListWidgetLayout->addWidget(paramStackWidget, 1);

  scrollArea->setWidget(distTabListWidget);
  mainLayout->addWidget(scrollArea);
}


void CopulaInferenceResultWidget::updateRadioButtonsDistributionTable(const QModelIndex& current)
{
  // check
  if (!distTableModel_ || current.row() < 1)
    return;

  // set CheckStateRole of distTableModel_
  // loop begins at 1 because the first row is the table title
  for (int i = 1; i < distTableModel_->rowCount(); ++i)
  {
    if (distTableModel_->index(i, 0).row() == current.row())
      distTableModel_->setData(distTableModel_->index(i, 0), true, Qt::CheckStateRole);
    else
      distTableModel_->setData(distTableModel_->index(i, 0), false, Qt::CheckStateRole);
  }
}


void CopulaInferenceResultWidget::updateParametersTable(const QModelIndex& current)
{
  // check
  if (!distTableModel_)
    return;
  if (current.row() < 1)
    return;

  emit distributionChanged(current.row() - 1);
}
}
