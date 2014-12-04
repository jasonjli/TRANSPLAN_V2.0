#include "cspmaxcountervar.h"
#include "searchstate.h"

namespace TRANSPLAN
{

    /*!
      Constructor
     */
    CSPMaxCounterVar::CSPMaxCounterVar(int index, int upper_bound, IntSet element_set, SearchState* owningState)
    : CSPVariable(index, owningState)
    , domain(element_set)
    {
        int s = element_set.size() - upper_bound;
        if (s < 0)
            max_size = 0;
        else
            max_size = s;
    }

    /*!
      Copy constructor
     */
    CSPMaxCounterVar::CSPMaxCounterVar(const CSPMaxCounterVar& mc)
    : CSPVariable(mc)
    , domain(mc.domain)
    , max_size(mc.max_size)
    {
        //
    }

    /*!
      = operator overloading
     */
    CSPMaxCounterVar& CSPMaxCounterVar::operator=(const CSPMaxCounterVar& mc)
    {
        CSPVariable::operator=(mc);
        this->max_size = mc.max_size;
        this->domain = mc.domain;

        return *this;
    }

    /*!
     * Clone this variable to the targetState
     * Only called from the SearchState->Clone method
     */
    CSPMaxCounterVar* CSPMaxCounterVar::clone()
    {
        return new CSPMaxCounterVar(*this);
    }

    CSP_MOD_EVENT CSPMaxCounterVar::increament(int trans_index)
    {
        if (max())
        {
            return TRANSPLAN::IGNORED;
        } else
        {
            domain.erase(trans_index);
            postUpdate();
            return TRANSPLAN::SUCCESS;
        }
    }
}
