// SWIG file

%{
#include "otgui/ImportedDesignOfExperiment.hxx"
%}

%ignore OTGUI::ImportedDesignOfExperiment::setDesignOfExperiment;

%include ImportedDesignOfExperiment_doc.i

%include otgui/ImportedDesignOfExperiment.hxx
namespace OTGUI {

%extend ImportedDesignOfExperiment { ImportedDesignOfExperiment(const ImportedDesignOfExperiment & other) { return new OTGUI::ImportedDesignOfExperiment(other); } 

} }