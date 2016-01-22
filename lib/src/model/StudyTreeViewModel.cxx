#include "otgui/StudyTreeViewModel.hxx"

#include "otgui/OTStudy.hxx"
#include "otgui/AnalyticalPhysicalModel.hxx"

namespace OTGUI {

StudyTreeViewModel::StudyTreeViewModel()
  : QStandardItemModel()
  , Observer()
{
}


StudyTreeViewModel::~StudyTreeViewModel()
{
  
}


void StudyTreeViewModel::update(Observable * source, const std::string & message)
{
  if (message == "addStudy")
  {
    OTStudy * otStudy = static_cast<OTStudy*>(source);
    addOTStudyItem(otStudy);
  }
}


void StudyTreeViewModel::createNewOTStudy()
{
  OTStudy * newStudy = new OTStudy(OTStudy::GetAvailableOTStudyName());
}


void StudyTreeViewModel::addOTStudyItem(OTStudy * otStudy)
{
  OTStudyItem * otStudyItem = new OTStudyItem(otStudy);
  connect(otStudyItem, SIGNAL(newPhysicalModelItemCreated(PhysicalModelItem*)), this, SIGNAL(newPhysicalModelCreated(PhysicalModelItem*)));
  connect(otStudyItem, SIGNAL(newProbabilisticModelItemCreated(ProbabilisticModelItem*)), this, SIGNAL(newProbabilisticModelCreated(ProbabilisticModelItem*)));
  connect(otStudyItem, SIGNAL(newDesignOfExperimentItemCreated(DesignOfExperimentItem*)), this, SIGNAL(newDesignOfExperimentCreated(DesignOfExperimentItem*)));
  connect(otStudyItem, SIGNAL(newLimitStateItemCreated(LimitStateItem*)), this, SIGNAL(newLimitStateCreated(LimitStateItem*)));
  connect(otStudyItem, SIGNAL(newAnalysisItemCreated(AnalysisItem*)), this, SIGNAL(newAnalysisCreated(AnalysisItem*)));
  otStudy->addObserver(otStudyItem);
  invisibleRootItem()->appendRow(otStudyItem);
}


void StudyTreeViewModel::addPhysicalModelItem(const QModelIndex & parentIndex)
{
  OTStudyItem * parentItem = static_cast<OTStudyItem*>(itemFromIndex(parentIndex));
  AnalyticalPhysicalModel newPhysicalModel(parentItem->getOTStudy()->getAvailablePhysicalModelName());
  parentItem->getOTStudy()->addPhysicalModel(newPhysicalModel);
}


void StudyTreeViewModel::addProbabilisticModelItem(const QModelIndex & parentIndex)
{
  PhysicalModelItem * parentItem = static_cast<PhysicalModelItem*>(itemFromIndex(parentIndex)->parent());
  ProbabilisticModelItem * newProbabilisticModelItem = new ProbabilisticModelItem(parentItem->getPhysicalModel());
  parentItem->getPhysicalModel().addObserver(newProbabilisticModelItem);
  connect(parentItem, SIGNAL(physicalModelChanged(PhysicalModel)), newProbabilisticModelItem, SLOT(updatePhysicalModel(PhysicalModel)));
  itemFromIndex(parentIndex)->appendRow(newProbabilisticModelItem);
  emit newProbabilisticModelCreated(newProbabilisticModelItem);
}


void StudyTreeViewModel::addLimitStateItem(const QModelIndex & parentIndex)
{
  PhysicalModelItem * parentItem = static_cast<PhysicalModelItem*>(itemFromIndex(parentIndex)->parent());
  PhysicalModel physicalModel = parentItem->getPhysicalModel();
  OTStudyItem * studyItem = getOTStudyItem(parentIndex);
  LimitState newLimitState(studyItem->getOTStudy()->getAvailableLimitStateName(), physicalModel, physicalModel.getOutputs()[0].getName(), OT::Less(), 0.);
  studyItem->getOTStudy()->addLimitState(newLimitState);
}


OTStudyItem* StudyTreeViewModel::getOTStudyItem(const QModelIndex & childIndex)
{
  QModelIndex seekRoot = childIndex;
  while(seekRoot.parent() != QModelIndex())
    seekRoot = seekRoot.parent();

  return static_cast<OTStudyItem*>(itemFromIndex(seekRoot));
}
}