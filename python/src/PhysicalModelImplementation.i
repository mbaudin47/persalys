// SWIG file

%{
#include "otgui/PhysicalModelImplementation.hxx"
%}



%include otgui/PhysicalModelImplementation.hxx
namespace OTGUI {

%extend PhysicalModelImplementation { PhysicalModelImplementation(const PhysicalModelImplementation & other) { return new OTGUI::PhysicalModelImplementation(other); } 

std::string __repr__() {
  return "PhysicalModelImplementation";
}

} }
