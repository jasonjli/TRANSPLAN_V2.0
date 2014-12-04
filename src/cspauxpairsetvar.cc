#include "cspauxpairsetvar.h"
#include "searchstate.h"

namespace TRANSPLAN
{

    /*!
      Constructor
     */
    CSPAuxPairSetVar::CSPAuxPairSetVar(int index, SearchState* owningState)
    : CSPVariable(index, owningState)
    {
    }

    /*!
      Copy constructor
     */
    CSPAuxPairSetVar::CSPAuxPairSetVar(const CSPAuxPairSetVar& var)
    : CSPVariable(var)
    , set(var.set)
    , addedElements(var.addedElements)
    {
        //
    }

    /*!
      = operator orverloading
     */
    CSPAuxPairSetVar& CSPAuxPairSetVar::operator=(const CSPAuxPairSetVar& var)
    {
        CSPVariable::operator=(var);
        this->set = var.set;
        this->addedElements = var.addedElements;

        return *this;
    }

    /*!
     * Clone this variable to the targetState
     * Only called from the SearchState->Clone method
     */
    CSPAuxPairSetVar* CSPAuxPairSetVar::clone()
    {
        return new CSPAuxPairSetVar(*this);
        ;
    }

    /*!
      Assignment of a value, equal
     */
    CSP_MOD_EVENT CSPAuxPairSetVar::insert(IntPair val)
    {
        if (!in(val))
        {
            addedElements.push(val);
            set.push_back(val);

            postUpdate();

            return TRANSPLAN::SUCCESS;
        } else
            return TRANSPLAN::IGNORED;
    }
    /*
    void CSPAuxPairSet::post()
    {
      for (int i= 0; i < propagators.size(); ++i)
      {
        PropagatorManager::attach(propagators[i]);
      }
    }
     */
}
