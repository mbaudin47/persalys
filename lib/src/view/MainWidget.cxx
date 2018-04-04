//                                               -*- C++ -*-
/**
 *  @brief QMainWidget, main window of the interface
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
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#include "otgui/MainWidget.hxx"

#include <QSplitter>
#include <QVBoxLayout>

namespace OTGUI
{

MainWidget::MainWidget(QWidget* parent)
  : QWidget(parent)
  , studyTree_(new StudyTreeView(this))
  , mdiArea_(new OTguiMdiArea(this))
  , graphSettingDockWidget_(new QDockWidget(tr("Graph setting"), this))
  , actions_(new OTguiActions(this))
{
  buildInterface();
}


void MainWidget::buildInterface()
{
  // main widget
  QSplitter * mainSplitter = new QSplitter(Qt::Horizontal);

  // left side of the mainSplitter
  QSplitter * leftSideSplitter = new QSplitter(Qt::Vertical);
  // - study tree
  leftSideSplitter->addWidget(studyTree_);
  leftSideSplitter->setStretchFactor(0, 8);

  // - dock widget for graph setting
  graphSettingDockWidget_->setFeatures(QDockWidget::NoDockWidgetFeatures);
  leftSideSplitter->addWidget(graphSettingDockWidget_);
  graphSettingDockWidget_->close();
  leftSideSplitter->setStretchFactor(1, 2);

  mainSplitter->addWidget(leftSideSplitter);
  mainSplitter->setStretchFactor(0, 0);

  // right side of the mainSplitter
  // - welcome page
  WelcomeWindow * welcomeWindow = new WelcomeWindow(actions_, this);
  // - MdiArea
  mdiArea_->addWelcomeWindow(welcomeWindow);
  mainSplitter->addWidget(mdiArea_);
  mainSplitter->setStretchFactor(1, 3);

  QVBoxLayout *layout = new QVBoxLayout;
  layout->addWidget(mainSplitter);
  setLayout(layout);
}


StudyTreeView * MainWidget::getStudyTree() const
{
  return studyTree_;
}


OTguiMdiArea * MainWidget::getMdiArea() const
{
  return mdiArea_;
}


OTguiActions * MainWidget::getActions() const
{
  return actions_;
}


void MainWidget::showGraphSettingDockWidget(QWidget * graph)
{
  graphSettingDockWidget_->setWidget(graph);
  graphSettingDockWidget_->show();
}


void MainWidget::hideGraphSettingDockWidget(QWidget * widgetToBeHidden)
{
  // if the current dock widget is not widgetToBeHidden : do nothing
  if (graphSettingDockWidget_->widget() == widgetToBeHidden)
    graphSettingDockWidget_->close();
}
}
