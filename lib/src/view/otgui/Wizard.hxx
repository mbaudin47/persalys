//                                               -*- C++ -*-
/**
 *  @brief Specific QWizard for otgui
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
#ifndef OTGUI_WIZARD_HXX
#define OTGUI_WIZARD_HXX

#include "otgui/Analysis.hxx"
#include "otgui/DesignOfExperiment.hxx"
#include "otgui/DesignOfExperimentDefinitionItem.hxx"

#include <QWizard>
#include <QMetaType> // mandatory to specify it to avoid windows compilation problem

Q_DECLARE_METATYPE(OTGUI::DesignOfExperiment)
Q_DECLARE_METATYPE(OTGUI::DesignOfExperimentDefinitionItem*)

namespace OTGUI
{
class OTGUI_API Wizard : public QWizard
{
  Q_OBJECT

public:
  Wizard(QWidget * parent = 0);

public slots:
  void showHelp();
signals:
  void analysisChanged(const Analysis & analysis);

private:
  static std::map<QString, QString> DocLinks_;
  static void InitializeDocLInks();
  QString getDocLink();
};
}
#endif
