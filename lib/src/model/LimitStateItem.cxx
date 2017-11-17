//                                               -*- C++ -*-
/**
 *  @brief QStandardItem, observer of a limit state
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
#include "otgui/LimitStateItem.hxx"

#include "otgui/MonteCarloReliabilityAnalysis.hxx"
#include "otgui/OTStudyItem.hxx"

using namespace OT;

namespace OTGUI
{

LimitStateItem::LimitStateItem(const LimitState & limitState)
  : OTguiItem(QString::fromUtf8(limitState.getName().c_str()), "LimitState")
  , Observer("LimitState")
  , limitState_(limitState)
{
  limitState_.addObserver(this);
  limitState_.getPhysicalModel().addObserver(this);

  buildActions();
}


void LimitStateItem::buildActions()
{
  // new threshold exceedance action
  newThresholdExceedance_ = new QAction(tr("Threshold exceedance"), this);
  newThresholdExceedance_->setStatusTip(tr("Create a new threshold exceedance"));
  connect(newThresholdExceedance_, SIGNAL(triggered()), this, SLOT(createNewThresholdExceedance()));

  // remove limit state action
  removeLimitState_ = new QAction(QIcon(":/images/window-close.png"), tr("Remove"), this);
  removeLimitState_->setStatusTip(tr("Remove the limit state"));
  connect(removeLimitState_, SIGNAL(triggered()), this, SLOT(removeLimitState()));

  // add actions
  appendSeparator(tr("Analysis"));
  appendAction(newThresholdExceedance_);
  appendSeparator();
  appendAction(removeLimitState_);
}


void LimitStateItem::setData(const QVariant & value, int role)
{
  // rename
  if (role == Qt::EditRole)
    limitState_.getImplementation()->setName(value.toString().toLocal8Bit().data());

  QStandardItem::setData(value, role);
}


LimitState LimitStateItem::getLimitState() const
{
  return limitState_;
}


void LimitStateItem::update(Observable* source, const String & message)
{
  // emit signals to LimitStateWindow

  if (message == "outputNumberChanged" ||
      message == "outputNameChanged" ||
      message == "outputSelectionChanged"
     )
  {
    emit outputListChanged();
  }
  else if (message == "operatorChanged")
  {
    emit operatorChanged();
  }
  else if (message == "thresholdChanged")
  {
    emit thresholdChanged();
  }
  else if (message == "limitStateRemoved")
  {
    emit removeRequested(row());
  }
}


void LimitStateItem::appendAnalysisItem(Analysis& analysis)
{
  // new item
  AnalysisItem * newItem = new AnalysisItem(analysis);

  // connections
  connect(newItem, SIGNAL(analysisInProgressStatusChanged(bool)), this, SLOT(setAnalysisInProgress(bool)));
  // - signal for the PhysicalModelDiagramItem
  connect(newItem, SIGNAL(analysisInProgressStatusChanged(bool)), this, SIGNAL(analysisInProgressStatusChanged(bool)));
  if (getParentOTStudyItem())
    connect(newItem, SIGNAL(analysisInProgressStatusChanged(bool)), getParentOTStudyItem(), SLOT(setAnalysisInProgress(bool)));

  // append item
  appendRow(newItem);

  // emit signal to StudyTreeView to create a window
  emit newAnalysisItemCreated(newItem);
}


void LimitStateItem::createNewThresholdExceedance()
{
  // check
  if (!limitState_.getPhysicalModel().isValid())
  {
    emit emitErrorMessageRequested(tr("The physical model must have inputs AND at least one selected output."));
    return;
  }
  if (!limitState_.getPhysicalModel().hasStochasticInputs())
  {
    emit emitErrorMessageRequested(tr("The physical model must have stochastic inputs."));
    return;
  }
  if (!limitState_.isValid())
  {
    emit emitErrorMessageRequested(tr("The limit state is not valid."));
    return;
  }

  // new analysis
  MonteCarloReliabilityAnalysis analysis(getParentOTStudyItem()->getOTStudy().getAvailableAnalysisName("reliability_"), limitState_);
  // emit signal to StudyTreeView to open a wizard
  emit analysisRequested(this, analysis);
}


void LimitStateItem::removeLimitState()
{
  // check
  if (analysisInProgress_)
  {
    emit emitErrorMessageRequested(tr("Can not remove a limit state when an analysis is running."));
    return;
  }
  // remove
  if (getParentOTStudyItem())
    getParentOTStudyItem()->getOTStudy().remove(LimitState(limitState_));
}
}
