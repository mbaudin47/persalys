// SWIG file

%{
#include "otgui/PythonPhysicalModel.hxx"
%}

%include PythonPhysicalModel_doc.i

%ignore *::resetCallsNumber;

%include otgui/PythonPhysicalModel.hxx
namespace OTGUI {

%extend PythonPhysicalModel { PythonPhysicalModel(const PythonPhysicalModel & other) { return new OTGUI::PythonPhysicalModel(other); } 

} }
