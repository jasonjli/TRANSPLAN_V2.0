#include "cspauxsetmonotonicincvar.h"
#include "searchstate.h"

namespace TRANSPLAN
{

    /*!
      Constructor
     */
    CSPAuxSetMonoIncVar::CSPAuxSetMonoIncVar(int index, IntSet dom, SearchState* owningState)
    : CSPVariable(index, owningState)
    , domain(dom)
    {
    }

    /*!
      Copy constructor
     */
    CSPAuxSetMonoIncVar::CSPAuxSetMonoIncVar(const CSPAuxSetMonoIncVar& var)
    : CSPVariable(var)
    , domain(var.domain)
    , addedElements(var.addedElements)
    {
    }

    /*!
      = operator overloading
     */
    CSPAuxSetMonoIncVar& CSPAuxSetMonoIncVar::operator=(const CSPAuxSetMonoIncVar& var)
    {
        CSPVariable::operator=(var);
        this->domain = var.domain;
        this->addedElements = var.addedElements;
        return *this;
    }

    /*!
     * Clone this variable to the targetState
     * Only called from the SearchState->Clone method
     */
    CSPAuxSetMonoIncVar* CSPAuxSetMonoIncVar::clone()
    {
        return new CSPAuxSetMonoIncVar(*this);
    }

    /*!
      Insert a value in the domain
     */
    CSP_MOD_EVENT CSPAuxSetMonoIncVar::insert(int val)
    {
        if (!in(val))
        {
            domain.insert(val);
            addedElements.push(val);

            postUpdate();
            return TRANSPLAN::SUCCESS;
        }
        return TRANSPLAN::IGNORED;
    }

}
