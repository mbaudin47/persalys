//                                               -*- C++ -*-
/**
 *  @brief QMainWindow, main window of the interface
 *
 *  Copyright 2015-2016 EDF-Phimeca
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
#ifndef OTGUI_MAINWINDOW_HXX
#define OTGUI_MAINWINDOW_HXX

#include "otgui/StudyTreeView.hxx"

#include <QMainWindow>
#include <QDockWidget>

namespace OTGUI {
class OTGUI_API MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow();

public slots:
  void showGraphConfigurationTabWidget(QWidget*);
  void exitApplication();

protected:
  void buildInterface();

private:
  StudyTreeView * studyTree_;
  QDockWidget * configurationDock_;
};
}
#endif