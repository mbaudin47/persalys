// SWIG file

%module(docstring="otguibase module") otguibase
%feature("autodoc","1");

%{
#include "OT.hxx"
#include "PythonWrappingFunctions.hxx"
%}

// Prerequisites needed
%include typemaps.i
%include exception.i
%ignore *::load(OT::Advocate & adv);
%ignore *::save(OT::Advocate & adv) const;

%import base_module.i
%import uncertainty_module.i



// define OTGUITypedInterfaceObjectHelper
%define OTGUITypedInterfaceObjectImplementationHelper(Interface,Implementation)

%template(Implementation ## TypedInterfaceObject)           OT::TypedInterfaceObject<OTGUI::Implementation>;

%typemap(in) const OTGUI::Interface &
{
  void * ptr = 0;
  if (SWIG_IsOK(SWIG_ConvertPtr($input, (void **) &$1, $1_descriptor, 0)))
  {
    // From interface class, ok
  }
  else if (SWIG_IsOK(SWIG_ConvertPtr($input, &ptr, SWIGTYPE_p_OTGUI__ ## Implementation, 0)))
  {
    // From Implementation*
    OTGUI::Implementation * p_impl = reinterpret_cast< OTGUI::Implementation * >( ptr );
    $1 = new OTGUI::Interface( &*p_impl );
  }
  else if (SWIG_IsOK(SWIG_ConvertPtr($input, &ptr, SWIG_TypeQuery("OT::Pointer<OTGUI::Implementation> *"), 0)))
  {
    // From Pointer<Implementation>
    OT::Pointer<OTGUI::Implementation> * p_impl = reinterpret_cast< OT::Pointer<OTGUI::Implementation> * >( ptr );
    $1 = new OTGUI::Interface( **p_impl );
  }
}

%typemap(typecheck,precedence=SWIG_TYPECHECK_POINTER) const OTGUI::Interface &
{
  $1 = SWIG_IsOK(SWIG_ConvertPtr($input, NULL, $1_descriptor, 0))
    || SWIG_IsOK(SWIG_ConvertPtr($input, NULL, SWIGTYPE_p_OTGUI__ ## Implementation, 0))
    || SWIG_IsOK(SWIG_ConvertPtr($input, NULL, SWIG_TypeQuery("OT::Pointer<OTGUI::Implementation> *"), 0));
}
%enddef

%define OTGUITypedInterfaceObjectHelper(Interface)
OTGUITypedInterfaceObjectImplementationHelper(Interface,Interface ## Implementation)
%enddef



// The new classes
%include OTGuiprivate.hxx
%include Observer.i
%include Observable.i

%include Variable.i
%include Input.i
%include Output.i
%include PhysicalModelImplementation.i
%include AnalyticalPhysicalModel.i
%include PythonPhysicalModel.i
%include PhysicalModel.i
#ifdef OTGUI_HAVE_YACS
%include YACSPhysicalModel.i
#endif
%include LimitStateImplementation.i
%include LimitState.i
%include AnalysisImplementation.i
%include Analysis.i
%include ModelEvaluationResult.i
%include ModelEvaluation.i
%include SimulationAnalysis.i
%include SimulationAnalysisResult.i
%include DesignOfExperimentImplementation.i
%include DesignOfExperiment.i
%include MonteCarloResult.i
%include QuadraticCumulResult.i
%include MonteCarloAnalysis.i
%include QuadraticCumulAnalysis.i
%include SobolResult.i
%include SobolAnalysis.i
%include SRCResult.i
%include SRCAnalysis.i
%include ReliabilityAnalysis.i
%include MonteCarloReliabilityAnalysis.i
%include OTStudy.i

