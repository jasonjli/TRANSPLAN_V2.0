#include "cspauxsetmonotonicdecvar.h"
#include "searchstate.h"

namespace TRANSPLAN
{

    /*!
      Constructor
     */
    CSPAuxSetMonoDecVar::CSPAuxSetMonoDecVar(int index, IntSet dom, SearchState* owningState)
    : CSPVariable(index, owningState)
    , domain(dom)
    {
    }

    /*!
      Copy constructor
     */
    CSPAuxSetMonoDecVar::CSPAuxSetMonoDecVar(const CSPAuxSetMonoDecVar& var)
    : CSPVariable(var)
    , domain(var.domain)
    , deletedElements(var.deletedElements)
    {
        //
    }

    /*!
      = operator orverloading
     */
    CSPAuxSetMonoDecVar& CSPAuxSetMonoDecVar::operator=(const CSPAuxSetMonoDecVar& var)
    {
        CSPVariable::operator=(var);
        this->domain = var.domain;
        this->deletedElements = var.deletedElements;
        return *this;
    }

    /*!
     * Clone this variable to the targetState
     * Only called from the SearchState->Clone method
     */
    CSPAuxSetMonoDecVar* CSPAuxSetMonoDecVar::clone()
    {
        return new CSPAuxSetMonoDecVar(*this);
    }

    /*!
      Remove the value from the domain not equal
     */
    CSP_MOD_EVENT CSPAuxSetMonoDecVar::remove(int val)
    {
        if (in(val))
        {
            domain.erase(val);
            deletedElements.push(val);
            postUpdate();
            return TRANSPLAN::SUCCESS;
        }
        return TRANSPLAN::IGNORED;
    }

    /*
    void CSPAuxSetMonoDecVar::post()
    {
      for (int i= 0; i < propagators.size(); ++i)
      {
        PropagatorManager::attach(propagators[i]);
      }
    }
     */
}
