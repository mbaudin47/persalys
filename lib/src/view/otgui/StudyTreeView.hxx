// StudyTreeView.hxx

#ifndef STUDYTREEVIEW_H
#define STUDYTREEVIEW_H

#include "otgui/StudyTreeViewModel.hxx"

#include <QTreeView>
#include <QMdiSubWindow>
#include <QAction>

namespace OTGUI {
class GraphConfigurationWidget;
class StudyTreeView : public QTreeView
{
  Q_OBJECT

public:
  StudyTreeView(QWidget * parent = 0);

protected:
  void buildActions();

public slots:
  void createNewOTStudy();
  void createNewPhysicalModel();
  void createNewProbabilisticModel();
  void createNewDesignOfExperiment();
  void createNewLimitState();
  void createNewParametricAnalysis();
  void createNewCentralTendency();
  void createNewSensitivityAnalysis();
  void createNewThresholdExceedance();
  void onCustomContextMenu(const QPoint & point);
  void selectedItemChanged(const QModelIndex & index);
  void runDesignOfExperiment();
  void runParametricAnalysis();
  void runCentralTendency();
  void runSensitivityAnalysis();
  void runReliabilityAnalysis();
  void createParametricAnalysisResult(AnalysisItem *);
  void createCentralTendencyResult(AnalysisItem *);
  void createSensitivityAnalysisResult(AnalysisItem * item);
  void createReliabilityAnalysisResult(AnalysisItem*);
  void createAnalysisConnection(AnalysisItem*);
  void createNewPhysicalModelWindow(PhysicalModelItem * item);
  void createNewProbabilisticModelWindow(ProbabilisticModelItem * item);
  void createNewDesignOfExperimentWindow(DesignOfExperimentItem* item);
  void createNewLimitStateWindow(LimitStateItem * item);
  void dumpOTStudy();
  void loadOTStudy();
signals:
  void showWindow(QMdiSubWindow*);
  void itemSelected(QStandardItem *);
  void checkIfWindowResultExists(ObserverItem *);
  void loadPythonScript(const QString & fileName);
  void graphWindowActivated(GraphConfigurationWidget*);
  void graphWindowDeactivated(GraphConfigurationWidget*);

private:
  StudyTreeViewModel * treeViewModel_;
  QAction * newOTStudyAction_;
  QAction * newPhysicalModel_;
  QAction * newProbabilisticModel_;
  QAction * newDesignOfExperiment_;
  QAction * newLimitState_;
  QAction * newParametricAnalysis_;
  QAction * newCentralTendency_;
  QAction * newSensitivityAnalysis_;
  QAction * newThresholdExceedance_;
  QAction * runDesignOfExperiment_;
  QAction * runParametricAnalysis_;
  QAction * runCentralTendency_;
  QAction * runSensitivityAnalysis_;
  QAction * runReliabilityAnalysis_;
  QAction * dumpOTStudy_;
};
}
#endif