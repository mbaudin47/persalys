// SWIG file

%{
#include "otgui/FixedDesignOfExperiment.hxx"
%}

%include FixedDesignOfExperiment_doc.i

%include otgui/FixedDesignOfExperiment.hxx
namespace OTGUI {

%extend FixedDesignOfExperiment { FixedDesignOfExperiment(const FixedDesignOfExperiment & other) { return new OTGUI::FixedDesignOfExperiment(other); } 

} }
