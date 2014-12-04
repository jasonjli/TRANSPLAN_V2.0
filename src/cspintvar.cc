#include <iostream>
#include "cspintvar.h"
#include "searchstate.h"


namespace TRANSPLAN
{
    /*!
      Constructor
     */
    CSPIntVar::CSPIntVar(int index, IntSet dom, SearchState* owningState)
    : CSPVariable(index, owningState)
    , domain(dom)
    {
        if (unit())
        {
            value = unit_value();
        } else
            value = TRANSPLAN::CSP_NOT_ASSIGNED;
    }

    /*!
      Copy constructor
     */
    CSPIntVar::CSPIntVar(const CSPIntVar& var)
    : CSPVariable(var)
    , domain(var.domain)
    , value(var.value)
    , prunedValues(var.prunedValues)
    {
        //
    }

    /*!
     * Clone this variable to the targetState
     * Only called from the SearchState->Clone method
     */
    CSPIntVar* CSPIntVar::clone()
    {
        return new CSPIntVar(*this);
    }

    /*!
      = operator overloading
     */
    CSPIntVar& CSPIntVar::operator=(const CSPIntVar& var)
    {
        CSPVariable::operator=(var);
        this->domain = var.domain;
        this->value = var.value;
        this->prunedValues = var.prunedValues;

        return *this;
    }

    /*!
      Assignment of a value, equal
     */
    CSP_MOD_EVENT CSPIntVar::eq(int addVal)
    {
        if (CSPIntVar::assigned())
        {
            if (CSPIntVar::value == addVal)
            {
                return TRANSPLAN::IGNORED;
            } else
            {
                return TRANSPLAN::FAILURE;
            }
        } else
        {
            if (CSPIntVar::in(addVal))
            {
                CSPIntVar::value = addVal;
                CSPIntVar::postUpdate();
                return TRANSPLAN::SUCCESS;
            } else
            {
                return TRANSPLAN::FAILURE;
            }
        }

    }

    /*!
      Remove the value from the domain not equal
     */
    CSP_MOD_EVENT CSPIntVar::nq(int removeVal)
    {
        if (CSPIntVar::assigned()) //if assigned check if we are removing the assigned value
        {
            if (CSPIntVar::value == removeVal)
            {
                return TRANSPLAN::FAILURE;
            } else
            {
                return TRANSPLAN::IGNORED;
            }
        } else
        {
            if (CSPIntVar::in(removeVal))
            {
                CSPIntVar::domain.erase(removeVal);

                //unit propagation
                if (CSPIntVar::unit())
                {
                    return CSPIntVar::eq(CSPIntVar::unit_value());
                }

                CSPIntVar::prunedValues.push(removeVal);
                CSPIntVar::postUpdate();
                return TRANSPLAN::SUCCESS;
            } else
            {
                return TRANSPLAN::IGNORED;
            }
        }
    }

}
