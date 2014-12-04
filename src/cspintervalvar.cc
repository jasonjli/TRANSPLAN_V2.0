#include "cspintervalvar.h"
#include "searchstate.h"

namespace TRANSPLAN
{

	/*!
	 Constructor
	 */
	CSPIntervalVar::CSPIntervalVar(int index, IntPair dom, SearchState* owningState) :
		CSPVariable(index, owningState), domain(dom)
	{
		isMinUpdate = false;
		isMaxUpdate = false;
	}

	/*!
	 Copy constructor
	 */
	CSPIntervalVar::CSPIntervalVar(const CSPIntervalVar& var) :
		CSPVariable(var), domain(var.domain)
	{
		isMinUpdate = false;
		isMaxUpdate = false;
	}

	/*!
	 = operator orverloading
	 */
	CSPIntervalVar& CSPIntervalVar::operator=(const CSPIntervalVar& var)
	{
		CSPVariable::operator=(var);
		this->domain = var.domain;
		return *this;
	}

	/*!
	 * Clone this variable to the targetState
	 * Only called from the SearchState->Clone method
	 */
	CSPIntervalVar* CSPIntervalVar::clone()
	{
		return new CSPIntervalVar(*this);
	}

	/*!
	 Assign to a fixed value
	 */
	CSP_MOD_EVENT CSPIntervalVar::eq(int val)
	{
		if (domain.first > val || domain.second < val)
		{
			return TRANSPLAN::FAILURE;
		}
		else
		{
			int lb_diff = val - domain.first;
			domain.first = val;
			if (lb_diff > 0)
			{
				isMinUpdate = true;
			}

			int ub_diff = domain.second - val;
			domain.second = val;
			if (ub_diff > 0)
			{
				isMaxUpdate = true;
			}

			if (lb_diff == 0 && ub_diff == 0)
				return TRANSPLAN::IGNORED;
			else
			{
				postUpdate();
				return TRANSPLAN::SUCCESS;
			}
		}
	}

	/*!
	 Greater than equal
	 */
	CSP_MOD_EVENT CSPIntervalVar::gq(int val)
	{
		if (domain.second < val)
		{
			return TRANSPLAN::FAILURE;
		}
		else
		{
			if (domain.first < val)
			{
				int lb_diff = val - domain.first;
				domain.first = val;
				if (lb_diff > 0)
				{
					isMinUpdate = true;
				}
				postUpdate();
				return TRANSPLAN::SUCCESS;
			}
			else
			{
				return TRANSPLAN::IGNORED;
			}
		}
	}

	/*!
	 Less than equal
	 */
	CSP_MOD_EVENT CSPIntervalVar::lq(int val)
	{
		if (domain.first > val)
		{
			return TRANSPLAN::FAILURE;
		}
		else
		{
			if (domain.second > val)
			{
				int ub_diff = domain.second - val;
				domain.second = val;
				if (ub_diff > 0)
				{
					isMaxUpdate = true;
				}
				postUpdate();
				return TRANSPLAN::SUCCESS;
			}
			else
			{
				return TRANSPLAN::IGNORED;
			}
		}
	}

}

