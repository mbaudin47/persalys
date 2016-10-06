//                                               -*- C++ -*-
/**
 *  @brief Specific QWizard for the analyses
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
#ifndef OTGUI_ANALYSISWIZARD_HXX
#define OTGUI_ANALYSISWIZARD_HXX

#include "otgui/OTguiWizard.hxx"
#include "otgui/OTStudy.hxx"

namespace OTGUI {
class AnalysisWizard : public OTguiWizard
{
public:
  AnalysisWizard(const Analysis & analysis, QWidget * parent=0);

  // analysis accessor
  Analysis getAnalysis() const;

protected:
  Analysis analysis_;
};
}
#endif