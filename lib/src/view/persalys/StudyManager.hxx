//                                               -*- C++ -*-
/**
 *  @brief QObject managing study objects
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
#ifndef PERSALYS_STUDYMANAGER_HXX
#define PERSALYS_STUDYMANAGER_HXX

#include "persalys/SubWindow.hxx"
#include "persalys/AnalysisWizard.hxx"
#include "persalys/StudyItem.hxx"
#include "persalys/DataModelDefinitionItem.hxx"
#include "persalys/DesignOfExperimentDefinitionItem.hxx"
#include "persalys/MainWidget.hxx"

namespace PERSALYS
{
class PERSALYS_API StudyManager : public QObject
{
  Q_OBJECT

public:
  StudyManager(MainWidget * mainWidget, QObject * parent = 0);

public slots:
  void setAnalysisInProgress(bool);
  void showErrorMessage(QString);

  // create objects
  void createStudy();
  void openAnalysisWizard(Item* item, const Analysis& analysis, const bool isGeneralWizard = false);
  void openDesignOfExperimentEvaluationWizard(const Analysis& analysis);
  void openDesignOfExperimentEvaluationWizard(const PhysicalModel& model);
  void openExtractDataFieldWizard(const Analysis& analysis);

  // create windows
  void createStudyWindow(StudyItem* item);
  void createDataModelDiagramWindow(DataModelDiagramItem*);
  void createDataModelWindow(DataModelDefinitionItem* item);
  void createPhysicalModelDiagramWindow(PhysicalModelDiagramItem* item);
  void createMeshWindow(MeshItem* item);
  void createFieldModelDiagramWindow(PhysicalModelDiagramItem* item);
  void createPhysicalModelWindow(PhysicalModelDefinitionItem* item);
  void createProbabilisticModelWindow(ProbabilisticModelItem* item);
  void createDesignOfExperimentWindow(DesignOfExperimentDefinitionItem* item, const bool createConnections = true);
  void createLimitStateWindow(LimitStateItem* item);
  void createAnalysisWindow(AnalysisItem* item, const bool createConnections = true);

  // modify objects
  void modifyDesignOfExperiment(DesignOfExperimentDefinitionItem* item);
  void modifyAnalysis(AnalysisItem* item);

  // import/export/save/open/close study
  void importPythonScript();
  void exportPythonScript();
  void saveCurrent();
  void saveAsCurrent();
  bool save(StudyItem* item);
  bool saveAs(StudyItem* item);
  void open(const QString& fileName = "");
  bool close(StudyItem* item);
  bool closeCurrent();
  bool closeAll();
signals:
  void recentFilesListChanged(const QString& recentFileName);
  void analysisInProgressStatusChanged(bool analysisInProgress);
  void commandExecutionRequested(const QString& command);

protected:
  void updateView(SubWindow * window);

private:
  MainWidget * mainWidget_;
  bool analysisInProgress_;
};
}
#endif