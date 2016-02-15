//                                               -*- C++ -*-
/**
 *  @brief QStandardItem, observer of a otStudy
 *
 *  Copyright 2015-2016 EDF
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
#include "otgui/OTStudyItem.hxx"

#include "otgui/ModelEvaluationItem.hxx"
#include "otgui/CentralTendencyItem.hxx"
#include "otgui/SensitivityAnalysisItem.hxx"
#include "otgui/ReliabilityAnalysisItem.hxx"
#include "otgui/ReliabilityAnalysis.hxx"

using namespace OT;

namespace OTGUI {

OTStudyItem::OTStudyItem(OTStudy * otStudy)
  : ObserverItem(otStudy->getName().c_str(), "OTStudy")
  , otStudy_(otStudy)
{

}


OTStudyItem::~OTStudyItem()
{

}


void OTStudyItem::update(Observable * source, const String & message)
{
  if (message == "addPhysicalModel")
  {
    PhysicalModel addedPhysicalModel = otStudy_->getPhysicalModels()[otStudy_->getPhysicalModels().getSize()-1];
    addPhysicalModelItem(addedPhysicalModel);
  }
  else if (message == "addDesignOfExperiment")
  {
    DesignOfExperiment addedDesignOfExperiment = otStudy_->getDesignOfExperiments()[otStudy_->getDesignOfExperiments().getSize()-1];
    try
    {
      addDesignOfExperimentItem(addedDesignOfExperiment);
    }
    catch (std::exception & ex)
    {
      std::cerr<<"No item added for the design of experiment named " << addedDesignOfExperiment.getName() << "\n" << ex.what() << std::endl;
    }
  }
  else if (message == "addLimitState")
  {
    LimitState addedLimitState = otStudy_->getLimitStates()[otStudy_->getLimitStates().getSize()-1];
    try
    {
      addLimitStateItem(addedLimitState);
    }
    catch (std::exception & ex)
    {
      std::cerr<<"No item added for the limit state named " << addedLimitState.getName() << ex.what() << std::endl;
    }
  }
  else if (message == "addAnalysis")
  {
    Analysis addedAnalysis = otStudy_->getAnalyses()[otStudy_->getAnalyses().getSize()-1];
    try
    {
      addAnalysisItem(addedAnalysis);
    }
    catch (std::exception & ex)
    {
      std::cerr<<"No item added for the analysis named " << addedAnalysis.getName() << ex.what() << std::endl;
    }
  }
  else
  {
    throw InvalidArgumentException(HERE) << "In OTStudyItem::update: not recognized message: " << message;
  }
}


void OTStudyItem::updatePhysicalModel(const PhysicalModel & physicalModel)
{
  otStudy_->getPhysicalModelByName(physicalModel.getName()).setImplementationAsPersistentObject(physicalModel.getImplementation());
}


void OTStudyItem::updateAnalysis(const Analysis & analysis)
{
  otStudy_->getAnalysisByName(analysis.getName()).setImplementationAsPersistentObject(analysis.getImplementation());
}


void OTStudyItem::addPhysicalModelItem(PhysicalModel & physicalModel)
{
  PhysicalModelItem * newPhysicalModelItem = new PhysicalModelItem(physicalModel);
  physicalModel.addObserver(newPhysicalModelItem);
  connect(newPhysicalModelItem, SIGNAL(physicalModelChanged(PhysicalModel)), this, SLOT(updatePhysicalModel(PhysicalModel)));
  appendRow(newPhysicalModelItem);
  Item * item = new Item(tr("Deterministic study"), "DeterministicStudy");
  item->setEditable(false);
  newPhysicalModelItem->appendRow(item);
  item = new Item(tr("Probabilistic study"), "ProbabilisticStudy");
  item->setEditable(false);
  newPhysicalModelItem->appendRow(item);

  if (physicalModel.hasStochasticInputs())
  {
    ProbabilisticModelItem * newProbabilisticModelItem = new ProbabilisticModelItem(physicalModel);
    physicalModel.addObserver(newProbabilisticModelItem);
    item->appendRow(newProbabilisticModelItem);
    connect(newPhysicalModelItem, SIGNAL(physicalModelChanged(PhysicalModel)), newProbabilisticModelItem, SLOT(updatePhysicalModel(PhysicalModel)));
    emit newProbabilisticModelItemCreated(newProbabilisticModelItem);
  }

  item = new Item(tr("Designs of experiment"), "DesignOfExperimentList");
  item->setEditable(false);
  newPhysicalModelItem->appendRow(item);

  emit newPhysicalModelItemCreated(newPhysicalModelItem);
}


void OTStudyItem::addDesignOfExperimentItem(DesignOfExperiment & design)
{
  DesignOfExperimentItem * newItem = new DesignOfExperimentItem(design);
  design.addObserver(newItem);
  design.getPhysicalModel().addObserver(newItem);
  for (int i=0; i<rowCount(); ++i)
    if (child(i)->text().toStdString() == design.getPhysicalModel().getName())
    {
      child(i)->child(2)->appendRow(newItem);
      emit newDesignOfExperimentItemCreated(newItem);
      return;
    }
  throw InvalidArgumentException(HERE) << "No physical model matches the given name " << design.getPhysicalModel().getName();
}


void OTStudyItem::addLimitStateItem(LimitState & limitState)
{
  LimitStateItem * newItem = new LimitStateItem(limitState);
  limitState.addObserver(newItem);
  limitState.getPhysicalModel().addObserver(newItem);
  for (int i=0; i<rowCount(); ++i)
    if (child(i)->text().toStdString() == limitState.getPhysicalModel().getName())
    {
      if (!limitState.getPhysicalModel().hasStochasticInputs() && !child(i)->child(1)->hasChildren())
      {
        ProbabilisticModelItem * newProbabilisticModelItem = new ProbabilisticModelItem(limitState.getPhysicalModel());
        limitState.getPhysicalModel().addObserver(newProbabilisticModelItem);
        child(i)->child(1)->appendRow(newProbabilisticModelItem);
        emit newProbabilisticModelItemCreated(newProbabilisticModelItem);
      }
      child(i)->child(1)->appendRow(newItem);
      emit newLimitStateItemCreated(newItem);
      return;
    }
  throw InvalidArgumentException(HERE) << "No physical model matches the name " << limitState.getPhysicalModel().getName();
}


void OTStudyItem::addAnalysisItem(Analysis & analysis)
{
  String analysisName = analysis.getImplementation()->getClassName();
  AnalysisItem * newItem;
  if (analysisName == "ModelEvaluation")
  {
    newItem = new ModelEvaluationItem(analysis);
    addDeterministicAnalysisItem(analysis, newItem);
  }
  else if (analysisName == "MonteCarloAnalysis" || analysisName == "TaylorExpansionsMomentsAnalysis")
  {
    newItem = new CentralTendencyItem(analysis);
    addProbabilisticAnalysisItem(analysis, newItem);
  }
  else if (analysisName == "SobolAnalysis" || analysisName == "SRCAnalysis")
  {
    newItem = new SensitivityAnalysisItem(analysis);
    addProbabilisticAnalysisItem(analysis, newItem);
  }
  else if (analysisName == "MonteCarloReliabilityAnalysis")
  {
    newItem = new ReliabilityAnalysisItem(analysis);
    addReliabilityAnalysisItem(analysis, newItem);
  }
  else
  {
    throw InvalidArgumentException(HERE) << "In OTStudyItem::addAnalysisItem: Impossible to add an item for the analysis of type " << analysisName;
  }
  connect(newItem, SIGNAL(analysisChanged(Analysis)), this, SLOT(updateAnalysis(Analysis)));
}


void OTStudyItem::addDeterministicAnalysisItem(Analysis & analysis, AnalysisItem * item)
{
  analysis.addObserver(item);
  for (int i=0; i<rowCount(); ++i)
    if (child(i)->text().toStdString() == analysis.getPhysicalModel().getName())
    {
      child(i)->child(0)->appendRow(item);
      emit newAnalysisItemCreated(item);
      return;
    }
  std::cerr<<"No item added for the deterministic analysis named "<<analysis.getName()<<std::endl;
}


void OTStudyItem::addProbabilisticAnalysisItem(Analysis & analysis, AnalysisItem * item)
{
  analysis.addObserver(item);
  for (int i=0; i<rowCount(); ++i)
    if (child(i)->text().toStdString() == analysis.getPhysicalModel().getName())
    {
      if (!child(i)->child(1)->hasChildren())
      {
        ProbabilisticModelItem * newProbabilisticModelItem = new ProbabilisticModelItem(analysis.getPhysicalModel());
        analysis.getPhysicalModel().addObserver(newProbabilisticModelItem);
        child(i)->child(1)->appendRow(newProbabilisticModelItem);
        emit newProbabilisticModelItemCreated(newProbabilisticModelItem);
      }
      child(i)->child(1)->appendRow(item);
      emit newAnalysisItemCreated(item);
      return;
    }
  std::cerr<<"No item added for the probabilistic analysis named "<<analysis.getName()<<std::endl;
}


void OTStudyItem::addReliabilityAnalysisItem(Analysis & analysis, AnalysisItem * item)
{
  analysis.addObserver(item);
  String limitStateName = dynamic_cast<ReliabilityAnalysis*>(&*analysis.getImplementation())->getLimitState().getName();
  for (int i=0; i<rowCount(); ++i)
    if (child(i)->text().toStdString() == analysis.getPhysicalModel().getName())
    {
      QStandardItem * probabilisticStudyItem = child(i)->child(1);
      for (int j=0; j<probabilisticStudyItem->rowCount(); ++j)
        if (probabilisticStudyItem->child(j)->text().toStdString() == limitStateName)
        {
          probabilisticStudyItem->child(j)->appendRow(item);
          emit newAnalysisItemCreated(item);
          return;
        }
    }
  std::cerr<<"No item added for the reliability analysis named "<<analysis.getName()<<std::endl;
}


void OTStudyItem::setData(const QVariant & value, int role)
{
  switch (role)
  {
    case Qt::EditRole:
      otStudy_->setName(value.toString().toStdString());
      ObserverItem::setData(value, role);
      break;
  }
}


OTStudy* OTStudyItem::getOTStudy()
{
  return otStudy_;
}


QString OTStudyItem::dumpOTStudy()
{
  return otStudy_->dump().c_str();
}
}