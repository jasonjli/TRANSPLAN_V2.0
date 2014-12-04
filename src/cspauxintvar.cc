#include "cspauxintvar.h"
#include "searchstate.h"

namespace TRANSPLAN
{

    /*!
      Constructor
     */
    CSPAuxIntVar::CSPAuxIntVar(int index, int value, SearchState* owningState)
    : CSPVariable(index, owningState)
    , value(value)
    {
    }

    /*!
      Copy constructor
     */
    CSPAuxIntVar::CSPAuxIntVar(const CSPAuxIntVar& var)
    : CSPVariable(var)
    , value(var.value)
    {
        //
    }

    /*!
      = operator orverloading
     */
    CSPAuxIntVar& CSPAuxIntVar::operator=(const CSPAuxIntVar& var)
    {
        CSPVariable::operator=(var);
        this->value = var.value;
        return *this;
    }

    /*!
     * Clone this variable to the targetState
     * Only called from the SearchState->Clone method
     */
    CSPAuxIntVar* CSPAuxIntVar::clone()
    {
        return new CSPAuxIntVar(*this);
    }

    /*!
      Assignment of a value, equal
     */
    CSP_MOD_EVENT CSPAuxIntVar::assign(int val)
    {
        if (value != val)
        {
            value = val;
            postUpdate();
            TRANSPLAN::SUCCESS;
        } else
            return TRANSPLAN::IGNORED;
    }

}
