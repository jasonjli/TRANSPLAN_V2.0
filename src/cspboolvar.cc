#include "cspboolvar.h"
#include "searchstate.h"
#include <iostream>

namespace TRANSPLAN
{

	/*!
	 Constructor
	 */
	CSPBoolVar::CSPBoolVar(int index, SearchState* owningState) :
		CSPVariable(index, owningState), value(TRANSPLAN::CSP_NOT_ASSIGNED)
	{
		//
	}

	/*!
	 Copy constructor
	 */
	CSPBoolVar::CSPBoolVar(const CSPBoolVar& var) :
		CSPVariable(var), value(var.value)
	{
		//
	}

	/*!
	 = operator orverloading
	 */
	CSPBoolVar& CSPBoolVar::operator=(const CSPBoolVar& var)
	{
		CSPVariable::operator=(var);
		this->value = var.value;
		return *this;
	}

	/*!
	 * Clone this variable to the targetState
	 * Only called from the SearchState->Clone method
	 */
	CSPBoolVar* CSPBoolVar::clone()
	{
		return new CSPBoolVar(*this);
	}

	CSP_MOD_EVENT CSPBoolVar::eqTrue()
	{
		if (isTrue())
		{
			return TRANSPLAN::IGNORED;
		}
		else if (isFalse())
		{
			return TRANSPLAN::FAILURE;
		}
		else
		{
			value = 1;
			postUpdate();
			return TRANSPLAN::SUCCESS;
		}
	}

	CSP_MOD_EVENT CSPBoolVar::eqFalse()
	{
		if (isTrue())
		{
			return TRANSPLAN::FAILURE;
		}
		else if (isFalse())
		{
			return TRANSPLAN::IGNORED;
		}
		else
		{
			value = 0;
			postUpdate();
			return TRANSPLAN::SUCCESS;
		}
	}

}
