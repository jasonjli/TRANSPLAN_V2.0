#include "cspauxsetvar.h"
#include "searchstate.h"

namespace TRANSPLAN
{

    /*!
      Constructor
     */
    CSPAuxSetVar::CSPAuxSetVar(int index, IntSet dom, SearchState* owningState)
    : CSPVariable(index, owningState)
    , domain(dom)
    {
    }

    /*!
      Copy constructor
     */
    CSPAuxSetVar::CSPAuxSetVar(const CSPAuxSetVar& var)
    : CSPVariable(var)
    , domain(var.domain)
    {
        //
    }

    /*!
      = operator orverloading
     */
    CSPAuxSetVar& CSPAuxSetVar::operator=(const CSPAuxSetVar& var)
    {
        CSPVariable::operator=(var);
        this->domain = var.domain;
        return *this;
    }

    /*!
     * Clone this variable to the targetState
     * Only called from the SearchState->Clone method
     */
    CSPAuxSetVar* CSPAuxSetVar::clone()
    {
        return new CSPAuxSetVar(*this);
    }

    /*!
      Insert a value in the domain
     */
    CSP_MOD_EVENT CSPAuxSetVar::insert(int val)
    {
        if (!in(val))
        {
            domain.insert(val);
            postUpdate();
            return TRANSPLAN::SUCCESS;
        }

        return TRANSPLAN::IGNORED;
    }

    /*!
      Remove the value from the domain not equal
     */
    CSP_MOD_EVENT CSPAuxSetVar::remove(int val)
    {
        if (in(val))
        {
            domain.erase(val);

            postUpdate();
            return TRANSPLAN::SUCCESS;
        }
        return TRANSPLAN::IGNORED;
    }

    /*!
      Remove all value from the domain
     */
    CSP_MOD_EVENT CSPAuxSetVar::clear()
    {
        if (size() != 0)
        {
            domain.clear();
            postUpdate();
            return TRANSPLAN::SUCCESS;
        }
        return TRANSPLAN::IGNORED;
    }


}
