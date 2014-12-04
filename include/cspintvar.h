#ifndef CSPINTVAR_H_
#define CSPINTVAR_H_

#include <assert.h>
#include "constants.h"
#include "cspvariable.h"

namespace TRANSPLAN
{

	class CSPIntVar: public CSPVariable
	{
		private:
			IntSet domain;
			int value;
		public:

			IntStack prunedValues;

			/*!
			 Constructor
			 */
			CSPIntVar(int index, IntSet dom, SearchState* owningState);

			/*!
			 Copy constructor
			 */
			CSPIntVar(const CSPIntVar& var);

			/*!
			 = operator overloading
			 */
			CSPIntVar& operator=(const CSPIntVar& var);

			/*!
			 Clone: returns a cloned object
			 */
			CSPIntVar* clone();

			/*!
			 Assignment of a value, equal
			 */
			CSP_MOD_EVENT eq(int new_val);

			/*!
			 Remove the value from the domain not equal
			 */
			CSP_MOD_EVENT nq(int new_val);

			/*!
			 Returns the current domain
			 */
			inline IntSet dom()
			{
				return domain;
			}

			/*!
			 Returns the size of the domain
			 */
			inline int size()
			{
				return domain.size();
			}

			/*!
			 Returns the assigned value
			 */
			inline int val()
			{
				return value;
			}

			/*!
			 Return true if a value is assigned
			 */
			inline bool assigned()
			{
				return (value != TRANSPLAN::CSP_NOT_ASSIGNED);
			}

			/*!
			 Return true if the domain size is 1
			 */
			inline bool unit()
			{
				return (domain.size() == 1);
			}

			/*!
			 Returns the unit value
			 */
			inline int unit_value()
			{
				assert(unit());
				return *(domain.begin());
			}

			/*!
			 Returns true if the value exits in the current domain
			 */
			inline bool in(int new_val)
			{
				return (domain.count(new_val) != 0);
			}

			/*!
			 Add  domain values
			 */
			inline void addToDomain(int new_val)
			{
				domain.insert(new_val);
			}

			/*!
			 Make it unassigned
			 */
			inline void makeUnassigned()
			{
				value = TRANSPLAN::CSP_NOT_ASSIGNED;
			}

	};
}
#endif
